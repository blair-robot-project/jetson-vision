import cv2 as cv


class ContourFinder:

    def __init__(self, mode):
        self.mode = mode

    def get_largest_contour(self, in_mat):
        _, contours, _ = cv.findContours(in_mat, cv.RETR_EXTERNAL, self.mode)
        if len(contours) == 0:
            return ()
        # Get the biggest contour
        longest = contours[0]
        for contour in contours:
            if self.contour_comparator(contour, longest):
                longest = contour
        return longest

    @staticmethod
    def contour_comparator(a, b):
        return len(a) > len(b)
