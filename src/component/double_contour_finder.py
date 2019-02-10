import cv2 as cv


class DoubleContourFinder:

    def __init__(self, mode):
        self.mode = mode

    def get_two_complicated_contours(self, in_mat):
        # Find the contours in the threshholded image
        _, contours, _ = cv.findContours(in_mat, cv.RETR_EXTERNAL, self.mode)

        # Exit immediately if there aren't any
        if len(contours) == 0:
            return ()
        # Return the only contour if there's just one
        elif len(contours) == 1:
            return [contours[0]]

        # Get the two contours with the most points, meaning they're the most complicated.
        longest = []
        second = []
        for contour in contours:
            if self.contour_comparator(contour, longest):
                # longest becomes second longest, new contour becomes longest
                second = longest
                longest = contour
            elif self.contour_comparator(contour, second):
                second = contour

        # Return in order (left, right), and assume that one contour is entirely to the left of the other.
        if longest[0][0][0] < second[0][0][0]:
            return longest, second
        else:
            return second, longest

    @staticmethod
    def contour_comparator(a, b):
        return len(a) > len(b)
