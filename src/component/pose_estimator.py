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
        # Run solvePnP then rearrange the matrix it gives to get one that's a 3x3 rotation matrix with a 3d position
        # vector concatenated to it on the right, and the vector <0,0,0,1> appended to the bottom.
        _, rvec, tvec = cv.solvePnP(self.object_corners, np.array(corners), self.camera_matrix, self.dist_coeffs)
        zyx, _ = cv.Rodrigues(rvec)
        final = np.concatenate((zyx, tvec), axis=1)
        final = np.concatenate((final, self.bottom), axis=0)
        return np.linalg.inv(final)


