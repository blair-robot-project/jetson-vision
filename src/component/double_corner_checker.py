import numpy as np
from src.component.corner_checker import CornerChecker


class DoubleCornerChecker:

    def __init__(self, width, height, min_diag=30, border=0, left_clockwise=True, right_clockwise=False):
        self.single_corner_checker = CornerChecker(width, height, min_diag=min_diag, border=border)
        self.left_clockwise = left_clockwise
        self.right_clockwise = right_clockwise

    def check_corners(self, left_corners, right_corners):
        left_valid = left_corners is not None and self.single_corner_checker.check_corners(left_corners)
        right_valid = right_corners is not None and self.single_corner_checker.check_corners(right_corners)
        # Check that they are actually rotated in the correct direction. This assumes a rectangular target that's
        # taller than it is wide and is at a small angle.
        left_valid = left_valid and self.left_clockwise == (left_corners[3][1] > left_corners[1][1])
        right_valid = right_valid and self.right_clockwise == (right_corners[3][1] > right_corners[1][1])

        # Check that they don't overlap horizontally
        if left_valid and right_valid and left_corners[1][0] > right_corners[3][0]:
            # If they overlap, keep the larger contour
            left_valid = np.linalg.norm(left_corners[1] - left_corners[3]) > \
                         np.linalg.norm(right_corners[1] - right_corners[3])
            right_valid = not left_valid
        return left_valid, right_valid
