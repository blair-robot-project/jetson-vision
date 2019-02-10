import math

from networktables import NetworkTables
from src.component.threshholder import Threshholder
from src.component.contour_finder import ContourFinder
from src.component.double_contour_finder import DoubleContourFinder
from src.component.corner_finder import CornerFinder
from src.component.tilted_corner_finder import TiltedCornerFinder
from src.component.corner_checker import CornerChecker
from src.component.double_corner_checker import DoubleCornerChecker
from src.component.pose_estimator import PoseEstimator
import cv2 as cv
import numpy as np

# The (x,y,z) points for the corners of the vision targets, in the order top, right, bottom, left, in inches
left_points = np.array([[-5.936295, 0, 0], [-4, 0.50076, 0], [-5.37709, 5.825572, 0], [-7.313385, 5.324812, 0]], np.float32)
right_points = np.array([[5.936295, 0, 0], [7.313385, 5.324812, 0], [5.37709, 5.825572, 0], [4, 0.50076, 0]], np.float32)

# Define the threshholder. You may have to change the HSV bounds depending on lighting and which camera you're using.
# thresh = Threshholder(gaussian_size=5, erode_size=7, lower_hsv_bound=(40, 135, 55), upper_hsv_bound=(60, 255, 160))
thresh = Threshholder(gaussian_size=1, erode_size=10, lower_hsv_bound=(40, 0, 30), upper_hsv_bound=(90, 255, 200))

# Define the contour finder to find the outlines of the vision target.
# contour_finder = ContourFinder(mode=cv.CHAIN_APPROX_SIMPLE)
contour_finder = DoubleContourFinder(mode=cv.CHAIN_APPROX_SIMPLE)

# Set up the NetworkTables connection
print("Connecting to server")
NetworkTables.initialize(server='10.4.49.2')
print("Connected!")

table = NetworkTables.getTable("SmartDashboard").getSubTable('jetson-vision')

# Set up the VideoCapture we use as input, either as a pre-recorded video or a live camera stream.
# cap = cv.VideoCapture("/home/nvidia/Downloads/my_video-1.mkv")  # Pre-recorded video
# cap = cv.VideoCapture("/media/LinHDD/Videos/my_video-1.mkv")  # Pre-recorded video
cap = cv.VideoCapture(0)  # Live camera feed. The number here corresponds to the /dev/video number.
# cap = cv.VideoCapture("/media/LinHDD/Images/vision/tilted.png")  # Pre-recorded image.

# Get the image width and height.
width = cap.get(cv.CAP_PROP_FRAME_WIDTH)
height = cap.get(cv.CAP_PROP_FRAME_HEIGHT)

# Define the corner finder to extract corners from contours.
# corner_finder = CornerFinder(width, height)
clockwise_corner_finder = TiltedCornerFinder(bias_clockwise=True)
ccw_corner_finer = TiltedCornerFinder(bias_clockwise=False)

# Define the corner checker, which makes sure that points aren't too close to the edge and that boxes aren't too small.
corner_checker = DoubleCornerChecker(width, height, min_diag=30, border=20)

# Open the camera calibration file (generated with another program)
fs = cv.FileStorage("lifecam-calib.yml", cv.FILE_STORAGE_READ)
if not fs.isOpened():
    print("Could not open config file!")
    exit(-1)

# Read in the camera parameters from the file.
camera_matrix = fs.getNode("camera_matrix").mat()
dist_coeff = fs.getNode("distortion_coefficients").mat()

# Create a pose estimator object to run SolvePnP.
left_pose_estimator = PoseEstimator(left_points, camera_matrix, dist_coeff)
right_pose_estimator = PoseEstimator(right_points, camera_matrix, dist_coeff)
both_pose_estimator = PoseEstimator(np.concatenate((left_points, right_points)), camera_matrix, dist_coeff)

# Create windows for viewing the output.
#cv.namedWindow("Video Capture", cv.WINDOW_NORMAL)
#cv.namedWindow("Threshholded", cv.WINDOW_NORMAL)

# Loop until the users presses 'q'
while cv.waitKey(15) != ord('q'):
    # Read in the next frame from the camera/video
    _, frame = cap.read()

    # frame = cv.flip(frame, 1)

    # Only do things if the frame has content.
    if frame is not None and len(frame) != 0:

        # Threshhold to find just the retroreflective tape, then show the threshholded image
        threshed = thresh.threshhold(frame)
        #cv.imshow("Threshholded", threshed)

        # Find the most complex contour the camera can see.
        contours = contour_finder.get_two_complicated_contours(threshed)

        # Only go on if we found a contour
        if contours is not None and len(contours) > 0:
            if len(contours) == 1:
                # Find the corners of the contour, assuming it's the left target
                left_corners = clockwise_corner_finder.get_corners(contours[0])
                right_corners = None

                # If the left corner is above the right corner, then the rectangle we're looking at the right target.
                # This assumes a rectangular target that's taller than it is wide and is at a small angle.
                if left_corners[3][1] < left_corners[1][1]:
                    right_corners = ccw_corner_finer.get_corners(contours[0])
                    left_corners = None
            else:
                left_corners = clockwise_corner_finder.get_corners(contours[0])
                right_corners = ccw_corner_finer.get_corners(contours[1])

            left_valid, right_valid = corner_checker.check_corners(left_corners, right_corners)

            if left_valid and right_valid:
                pose_mat = both_pose_estimator.get_pose(np.concatenate((left_corners, right_corners)))
            elif left_valid:
                pose_mat = left_pose_estimator.get_pose(left_corners)
            elif right_valid:
                pose_mat = right_pose_estimator.get_pose(right_corners)

#            if left_valid:
#                # Top (Blue)
#                frame = cv.circle(frame, (left_corners[0][0], left_corners[0][1]), 5, (255, 0, 0))
#                # Right (Red)
#                frame = cv.circle(frame, (left_corners[1][0], left_corners[1][1]), 5, (0, 0, 255))
#                # Bottom (white)
#                frame = cv.circle(frame, (left_corners[2][0], left_corners[2][1]), 5, (255, 255, 255))
#                # Left (yellow)
#                frame = cv.circle(frame, (left_corners[3][0], left_corners[3][1]), 5, (0, 255, 255))
#            if right_valid:
#                # Top (Blue)
#                frame = cv.drawMarker(frame, (right_corners[0][0], right_corners[0][1]), (255, 0, 0))
#                # Right (Red)
#                frame = cv.drawMarker(frame, (right_corners[1][0], right_corners[1][1]), (0, 0, 255))
#                # Bottom (white)
#                frame = cv.drawMarker(frame, (right_corners[2][0], right_corners[2][1]), (255, 255, 255))
#                # Left (yellow)
#                frame = cv.drawMarker(frame, (right_corners[3][0], right_corners[3][1]), (0, 255, 255))

            if left_valid or right_valid:
                # Transfer pose info from output matrix to NetworkTables, after converting to feet
                table.putNumber("x", pose_mat[0][3]/12.)
                table.putNumber("y", pose_mat[1][3]/12.)
                table.putNumber("z", pose_mat[2][3]/12.)
                # print(str(pose_mat))
                # print(str(left_valid)+", "+str(right_valid))
                # print("Z: "+str(pose_mat[2][3]))
                table.putNumber("yaw", math.atan2(pose_mat[1][0], pose_mat[0][0]))

        # Display the frame after possibly drawing on it
        #cv.imshow("Video Capture", frame)

cap.release()
#cv.destroyAllWindows()
print("Should exit now.")
