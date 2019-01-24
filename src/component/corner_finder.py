import numpy as np
import math


class CornerFinder:

    def __init__(self, width, height):
        # Bottom left, bottom right, top left, top right
        self.corners = [np.array([0, 0]), np.array([width, 0]), np.array([0, height]), np.array([width, height])]

    def get_corners(self, contour):
        min_dist = [math.inf, math.inf, math.inf, math.inf]
        to_ret = [None, None, None, None]
        # print(contour[0])
        for point in contour:
            for i in range(4):
                dist = np.linalg.norm(point[0] - self.corners[i])
                if dist < min_dist[i]:
                    min_dist[i] = dist
                    to_ret[i] = np.array(point[0], dtype=np.float32)
        return to_ret
