import numpy as np


class CornerChecker:

    def __init__(self, width, height, min_diag=30, border=0):
        self.min_diag = min_diag
        self.maxX = width - border
        self.minX = border
        self.maxY = height - border
        self.minY = border

    def check_corners(self, corners):
        # Make sure they're unique
        if np.array_equal(corners[0], corners[1]) or np.array_equal(corners[0], corners[2]) or np.array_equal(
                corners[0], corners[3]) or np.array_equal(corners[1], corners[2]) or np.array_equal(corners[1], corners[
                3]) or np.array_equal(corners[2], corners[3]):
            # print("Repeated corners")
            return False
        # Check that each corner is at least a certain distance from the diagonally opposite corner, and that it's
        # not too close to the edge.
        return np.linalg.norm(corners[1] - corners[3]) > self.min_diag and np.linalg.norm(
            corners[0] - corners[2]) > self.min_diag and corners[0][0] > self.minX and corners[2][0] > self.minX and \
               corners[0][1] > self.minY and corners[1][1] > self.minY and corners[1][0] < self.maxX and corners[3][0] \
               < self.maxX and corners[2][1] < self.maxY and corners[2][1] < self.maxY
