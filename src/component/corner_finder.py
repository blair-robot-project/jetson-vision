import numpy as np
import math


class CornerFinder:

    def __init__(self, width, height):
        # The coordinates of the screen corners in the order: Bottom left, bottom right, top left, top right
        self.corners = [np.array([0, 0]), np.array([width, 0]), np.array([0, height]), np.array([width, height])]

    def get_corners(self, contour):
        # We can find the corners of a quadrilateral with sides parallel to the edge of the screen b finding the
        # points on the contour that are closest to each corner of the screen.

        # Initialize the minimum distance to each corner, and which point is at that distance.
        min_dist = [math.inf, math.inf, math.inf, math.inf]
        to_ret = [None, None, None, None]

        # Check distances for every point
        for point in contour:
            for i in range(4):
                # norm is the generalized version of the distance formula.
                dist = np.linalg.norm(point[0] - self.corners[i])
                if dist < min_dist[i]:
                    min_dist[i] = dist
                    to_ret[i] = np.array(point[0], dtype=np.float32)
        return to_ret
