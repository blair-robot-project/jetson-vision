import math

import zmq
from src.component.threshholder import Threshholder
from src.component.contour_finder import ContourFinder
from src.component.corner_finder import CornerFinder
from src.component.corner_checker import CornerChecker
from src.component.pose_estimator import PoseEstimator
import gen.python.pose_pb2 as pose
import cv2 as cv
import numpy as np

# The (x,y,z) points for the corners of the vision target, in the order Bottom left, bottom right, top left, top right
obj_points = np.array([[0, 0, 0], [2, 0, 0], [0, 5.75, 0], [2, 5.75, 0]], np.float32)

# The protobuf we're going to send to the RIO
message = pose.CameraPose()

# Define the threshholder. You may have to change the HSV bounds depending on lighting and which camera you're using.
thresh = Threshholder(gaussian_size=5, erode_size=10, lower_hsv_bound=(40, 135, 55), upper_hsv_bound=(60, 255, 160))
# thresh = Threshholder(gaussian_size=5, erode_size=10, lower_hsv_bound=(55, 0, 50), upper_hsv_bound=(90, 255, 200))

# Define the contour finder to find the outlines of the vision target.
contour_finder = ContourFinder(mode=cv.CHAIN_APPROX_SIMPLE)

# Set up the ZMQ context, which is needed to make the socket.
context = zmq.Context.instance()

# Define the ZMQ socket as a pair socket, so the RIO and Jetson can freely send information back and forth.
socket = context.socket(zmq.PAIR)

# Connect to the RIO.
print("Connecting to server")
socket.connect("tcp://10.4.49.2:5555")
print("Connected!")

# Set up the VideoCapture we use as input, either as a pre-recorded video or a live camera stream.
# cap = cv.VideoCapture("/home/nvidia/Downloads/my_video-1.mkv")  # Pre-recorded video
cap = cv.VideoCapture(0)  # Live camera feed. The number here corresponds to the /dev/video number.
# cap = cv.VideoCapture("/media/LinHDD/Images/vision/Screenshot from my_video-1.mkv.png")  # Pre-recorded image.

# Get the image width and height.
width = cap.get(cv.CAP_PROP_FRAME_WIDTH)
height = cap.get(cv.CAP_PROP_FRAME_HEIGHT)

# Define the corner finder to extract corners from contours.
corner_finder = CornerFinder(width, height)

# Define the corner checker, which makes sure that points aren't too close to the edge and that boxes aren't t00 small.
corner_checker = CornerChecker(width, height, min_diag=30, border=20)

# Open the camera calibration file (generated with another program)
fs = cv.FileStorage("lifecam-calib.yml", cv.FILE_STORAGE_READ)
if not fs.isOpened():
    print("Could not open config file!")
    exit(-1)

# Read in the camera parameters from the file.
camera_matrix = fs.getNode("camera_matrix").mat()
dist_coeff = fs.getNode("distortion_coefficients").mat()

# Create a pose estimator object to run SolvePnP.
pose_estimator = PoseEstimator(obj_points, camera_matrix, dist_coeff)

# Create windows for viewing the output.
cv.namedWindow("Video Capture", cv.WINDOW_NORMAL)
cv.namedWindow("Threshholded", cv.WINDOW_NORMAL)

# Loop until the users presses 'q'
while cv.waitKey(15) != ord('q'):
    # Read in the next frame from the camera/video
    _, frame = cap.read()

    # Only do things if the frame has content.
    if frame is not None and len(frame) != 0:
        # Display the frame
        cv.imshow("Video Capture", frame)

        # Threshhold to find just the retroreflective tape, then show the threshholded image
        threshed = thresh.threshhold(frame)
        cv.imshow("Threshholded", threshed)

        # Find the largest contour the camera can see.
        cont = contour_finder.get_largest_contour(threshed)

        # Only go on if we found a contour
        if cont is not None and len(cont) != 0:
            # Find the corners of the contour
            points = corner_finder.get_corners(cont)

            # Only go on if the corners define a sensible shape
            if corner_checker.check_corners(points):

                # Get the pose estimate from the corners
                pose_mat = pose_estimator.get_pose(points)

                # Transfer pose info from output matrix to protobuf
                message.x = pose_mat[0][3]
                message.y = pose_mat[1][3]
                message.z = pose_mat[2][3]
                message.yaw = math.atan2(pose_mat[1][0], pose_mat[0][0])
                # print("Sending pose, x: " + str(message.x) + ", y: " + str(message.y) + ", z: " + str(
                #     message.z) + ", yaw: " + str(message.yaw))

                # Send the protocol buffer. SerializeToString() doesn't actualy make a string, but rather a byte array.
                socket.send(message.SerializeToString())

cap.release()
