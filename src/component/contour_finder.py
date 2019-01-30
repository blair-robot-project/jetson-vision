import cv2 as cv


class ContourFinder:

    def __init__(self, mode):
        self.mode = mode

    def get_complicated_contour(self, in_mat):
        # Find the contours in the threshholded image
        _, contours, _ = cv.findContours(in_mat, cv.RETR_EXTERNAL, self.mode)

        # Exit immediately if there aren't any
        if len(contours) == 0:
            return ()

        # Get the contour with the most points defining it, meaning it's the most complicated.
        longest = contours[0]
        for contour in contours[1:]:
            if self.contour_comparator(contour, longest):
                longest = contour
        return longest

    @staticmethod
    def contour_comparator(a, b):
        return len(a) > len(b)
