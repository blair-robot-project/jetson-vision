import cv2 as cv
import numpy as np


class Threshholder:

    def __init__(self, gaussian_size, erode_size, lower_hsv_bound, upper_hsv_bound):
        self.gaussian_size = gaussian_size
        self.lower_hsv_bound = lower_hsv_bound
        self.upper_hsv_bound = upper_hsv_bound
        self.erode_kernel = np.ones((erode_size, erode_size), np.uint8)

    def set_limits(self, lower_hsv_bound, upper_hsv_bound):
        self.lower_hsv_bound = lower_hsv_bound
        self.upper_hsv_bound = upper_hsv_bound

    def threshhold(self, in_mat):
        # Switch to the HSV colorspace because it's easier to find the bright green retro tape
        out_mat = cv.cvtColor(src=in_mat, code=cv.COLOR_BGR2HSV)
        # Blur the image to get rid of some noise and make the threshhold more consistent
        out_mat = cv.GaussianBlur(out_mat, (self.gaussian_size, self.gaussian_size), 0, 0)
        # Threshhold the image, making everything outside of the chosen range back and everything in the range white.
        out_mat = cv.inRange(out_mat, self.lower_hsv_bound, self.upper_hsv_bound)
        # Erode and dilate the image to get rid of small dots and holes from the threshhhold
        out_mat = cv.erode(out_mat, self.erode_kernel)
        return cv.dilate(out_mat, self.erode_kernel)
