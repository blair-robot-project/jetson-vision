import cv2 as cv
import numpy as np


class PoseEstimator:

    def __init__(self, object_corners, camera_matrix, dist_coeffs):
        self.object_corners = object_corners
        self.camera_matrix = camera_matrix
        self.dist_coeffs = dist_coeffs
        self.bottom = np.array([0, 0, 0, 1])
        self.zyx = np.array([0])
        self.final = np.array([0])

    def get_pose(self, corners):
        _, rvec, tvec = cv.solvePNP(self.object_corners, corners, self.camera_matrix, self.dist_coeffs, flags=cv.SOLVEPNP_AP3P)
        zyx, _ = cv.Rodrigues(rvec)
        final = np.concatenate((zyx, tvec), axis=1)
        final = np.concatenate((final, self.bottom), axis=0)
        return np.linalg.inv(final)


