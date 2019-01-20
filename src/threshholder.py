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

    def threshhold(self, in_mat, out_mat):
        cv.cvtColor(in_mat, out_mat, cv.COLOR_BGR2HSV)
        cv.GaussianBlur(out_mat, out_mat, (5, 5), 0, 0)
        cv.inRange(out_mat, self.lower_hsv_bound, self.upper_hsv_bound, out_mat)
        cv.erode(out_mat, self.erode_kernel, out_mat)
        cv.dilate(out_mat, self.erode_kernel, out_mat)
