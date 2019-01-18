import cv2 as cv


class Threshholder:

    def __init__(self, gaussian_size, lower_hsv_bound, upper_hsv_bound):
        self.gaussian_size = gaussian_size
        self.lower_hsv_bound = lower_hsv_bound
        self.upper_hsv_bound = upper_hsv_bound

    def set_limits(self, lower_hsv_bound, upper_hsv_bound):
        self.lower_hsv_bound = lower_hsv_bound
        self.upper_hsv_bound = upper_hsv_bound

    def threshhold(self, in_mat, out_mat):
        cv.cvtColor(in_mat, out_mat, cv.COLOR_BGR2HSV)
        cv.GaussianBlur(out_mat, out_mat, (5, 5), 0, 0)
        cv.inRange(in_mat, self.lower_hsv_bound, self.upper_hsv_bound, out_mat)
