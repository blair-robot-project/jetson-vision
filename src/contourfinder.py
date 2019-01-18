import cv2 as cv


class ContourFinder:

    def __init__(self, mode):
        self.mode = mode

    def get_largest_contour(self, in_mat):
        _, contours, _ = cv.findContours(in_mat, self.mode, cv.RETR_EXTERNAL)
        if len(contours) == 0:
            return ()
        return contours.partition(0)[0]

    @staticmethod
    def contour_comparator(a, b):
        return len(a) > len(b)
