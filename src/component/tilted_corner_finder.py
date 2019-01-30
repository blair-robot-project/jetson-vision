import numpy as np
import math


class TiltedCornerFinder:

    def __init__(self, bias_clockwise=True):
        # Bias clockwise is about assuming which direction the rectangle is tilted when the image is too small to
        # detect the tilt.
        self.bias_clockwise = bias_clockwise

    def get_corners(self, contour):
        # We can find the corners of a quadrilateral with sides tilted relative to the edge of the screen by finding the
        # points on the contour that are closest to each edge of the screen.

        # Initialize the minimum/maximum x or y coordinates, and which point is at that coordinate. In openCV,
        # y is vertical, but 0 is the top of the screen and the positive Y axis points down towards the bottom. X is
        # horizontal, with the left side being zero.
        # Top, right, bottom, left
        extreme_values = [contour[0][0][1], contour[0][0][0], contour[0][0][1], contour[0][0][0]]
        to_ret = [contour[0][0], contour[0][0], contour[0][0], contour[0][0]]

        # Check distances for every point
        for wrapped_point in contour[1:]:
            point = wrapped_point[0]
            if point[1] < extreme_values[0]:
                extreme_values[0] = point[1]
                to_ret[0] = point
            elif point[1] == extreme_values[0] and self.bias_clockwise == (point[0] < to_ret[0][0]):
                to_ret[0] = point

            if point[0] > extreme_values[1]:
                extreme_values[1] = point[0]
                to_ret[1] = point
            elif point[0] == extreme_values[1] and self.bias_clockwise == (point[1] < to_ret[1][1]):
                to_ret[1] = point

            if point[1] > extreme_values[2]:
                extreme_values[2] = point[1]
                to_ret[2] = point
            elif point[1] == extreme_values[2] and self.bias_clockwise == (point[0] > to_ret[2][0]):
                to_ret[2] = point

            if point[0] < extreme_values[3]:
                extreme_values[3] = point[0]
                to_ret[3] = point
            elif point[0] == extreme_values[3] and self.bias_clockwise == (point[1] > to_ret[3][1]):
                to_ret[3] = point

        # Convert to float
        for i in range(len(to_ret)):
            to_ret[i] = np.ndarray.astype(to_ret[i], np.float32)

        return to_ret
