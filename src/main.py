import zmq
from src.threshholder import Threshholder
from src.contourfinder import ContourFinder
from src.corner_finder import CornerFinder
from src.corner_checker import CornerChecker
from  src.pose_estimator import PoseEstimator
import cv2 as cv
import numpy as np

obj_points = np.array([[0, 0, 0], [2, 0, 0], [0, 5.75, 0], [2, 5.75, 0]], np.float32)

thresh = Threshholder(5, 5, (66, 254, 102), (91, 255, 162))
contour_finder = ContourFinder(cv.CHAIN_APPROX_SIMPLE)

context = zmq.Context.instance()
socket = context.socket(zmq.PAIR)
print("Connecting to server")
socket.connect("tcp://10.4.49.2:5555")
print("Connected!")

cap = cv.VideoCapture("/media/LinHDD/Videos/my_video-1.mkv")
# cap = cv.VideoCapture(0)
# cap = cv.VideoCapture("/media/LinHDD/Images/vision/Screenshot from my_video-1.mkv.png")

width = cap.get(cv.CAP_PROP_FRAME_WIDTH)
height = cap.get(cv.CAP_PROP_FRAME_HEIGHT)

corner_finder = CornerFinder(width, height)
corner_checker = CornerChecker(width, height, 30, 20)

fs = cv.FileStorage("lifecam-calib.yml", cv.FILE_STORAGE_READ)
if not fs.isOpened():
    print("Could not open config file!")
    exit(-1)

camera_matrix = fs.getNode("camera_matrix").mat()
dist_coeff = fs.getNode("distortion_coefficients").mat()

pose_estimator = PoseEstimator(obj_points, camera_matrix, dist_coeff)

cv.namedWindow("Video Capture", cv.WINDOW_NORMAL)
cv.namedWindow("Threshholded", cv.WINDOW_NORMAL)

while cv.waitKey(1) != ord('q'):
    _, frame = cap.read()

    if frame is not None and len(frame) != 0:
        cv.imshow("Video Capture", frame)
        threshed = thresh.threshhold(frame)
        cv.imshow("Threshholded", threshed)

print(threshed)

cap.release()