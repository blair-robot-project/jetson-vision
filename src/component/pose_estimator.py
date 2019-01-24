import cv2 as cv
import numpy as np


class PoseEstimator:

    def __init__(self, object_corners, camera_matrix, dist_coeffs):
        self.object_corners = np.ndarray.astype(object_corners, np.float32)
        self.camera_matrix = np.ndarray.astype(camera_matrix, np.float32)
        self.dist_coeffs = np.ndarray.astype(dist_coeffs, np.float32)
        self.bottom = np.array([[0, 0, 0, 1]], dtype=np.float32)
        self.zyx = np.array([0])
        self.final = np.array([0])

    def get_pose(self, corners):
        # _, rvec, tvec = cv.solvePnP(self.object_corners, np.array(corners), self.camera_matrix, self.dist_coeffs, flags=cv.SOLVEPNP_P3P)
        _, rvec, tvec = cv.solvePnP(self.object_corners, np.array(corners), self.camera_matrix, self.dist_coeffs)
        zyx, _ = cv.Rodrigues(rvec)
        final = np.concatenate((zyx, tvec), axis=1)
        final = np.concatenate((final, self.bottom), axis=0)
        return np.linalg.inv(final)


