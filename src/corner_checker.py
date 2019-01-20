import numpy as np


class CornerChecker:

    def __init__(self, width, height, minDiag=30, border=0):
        self.minDiag = minDiag
        self.maxX = width - border
        self.minX = border
        self.maxY = height - border
        self.minY = border

    def check_corners(self, corners):
        # Make sure they're unique
        if corners[0] == corners[1] or corners[0] == corners[2] or corners[0] == corners[3] or corners[1] == corners[
                2] or corners[1] == corners[3] or corners[2] == corners[3]:
            return False
        return np.linalg.norm(corners[0] - corners[3]) > self.minDiag and np.linalg.norm(
            corners[1] - corners[2]) > self.minDiag and corners[0][0] > self.minX and corners[2][0] > self.minX and \
            corners[0][1] > self.minY and corners[1][1] > self.minY and corners[1][0] < self.maxX and corners[3][0] \
            < self.maxX and corners[2][1] < self.maxY and corners[2][1] < self.maxY
