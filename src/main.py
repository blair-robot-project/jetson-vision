import zmq
from src.threshholder import Threshholder
from src.contourfinder import ContourFinder
from src.corner_finder import CornerFinder
import cv2 as cv
import numpy as np

objPoints = np.array([[0, 0, 0], [2, 0, 0], [0, 5.75, 0], [2, 5.75, 0]], np.float32)

thresh = Threshholder(5, (66, 254, 102), (91, 255, 162))
contour_finder = ContourFinder(cv.CHAIN_APPROX_SIMPLE)

context = zmq.Context.instance()
socket = context.socket(zmq.PAIR)
print("Connecting to server")
socket.connect("tcp://10.4.49.2:5555")
print("Connected!")

# cap = cv.VideoCapture("/media/LinHDD/Videos/my_video-1.mkv")
cap = cv.VideoCapture(0)
# cap = cv.VideoCapture("/media/LinHDD/Images/vision/Screenshot from my_video-1.mkv.png")

width = cap.get(cv.CAP_PROP_FRAME_WIDTH)
height = cap.get(cv.CAP_PROP_FRAME_HEIGHT)

corner_finder = CornerFinder(width, height)
