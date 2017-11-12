//
// Created by noah on 11/11/17.
//

#ifndef JETSON_VISION_THRESHHOLDER_H
#define JETSON_VISION_THRESHHOLDER_H

#include "opencv2/opencv.hpp"

using namespace cv;

class threshholder{
public:
	/**
	 * Threshhold the image in HSV using CUDA GPU optimization.
	 *
	 * @param in The matrix to to threshhold.
	 * @return A single channel matrix where white is within the threshhold and black is not.
	 */
	Mat1b threshhold(const Mat &in);

	/**
	 * Set the limits of the threshhold.
	 *
     * @param lowerHSV The lower bound on HSV values.
     * @param upperHSV The upper bound on HSV values.
	 */
	void set_limits(const Vec3b &lowerHSV, const Vec3b &upperHSV);

	/**
	 * Default constructor
	 *
	 * @param gBlur The gaussian blur to apply to the matrix before threshholding.
     * @param lowerHSV The lower bound on HSV values.
     * @param upperHSV The upper bound on HSV values.
	 */
	threshholder(const Ptr<cuda::Filter> &gBlur, const Vec3b &lowerHSV, const Vec3b &upperHSV);

private:
	Ptr<cuda::Filter> gBlur;
	Vec3b lowerBound, upperBound;
	cuda::GpuMat gpuMat;
	Mat mat;
};

#endif //JETSON_VISION_THRESHHOLDER_H
