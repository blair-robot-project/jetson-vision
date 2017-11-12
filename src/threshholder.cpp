//
// Created by noah on 11/11/17.
//

#include "../include/threshholder.h"

using namespace cv;

/**
 * Threshhold the image in HSV using CUDA GPU optimization.
 *
 * @param in The matrix to to threshhold.
 * @return A single channel matrix where white is within the threshhold and black is not.
 */
Mat1b Threshholder::threshhold(const Mat &in){
	gpuMat.upload(in);
	cuda::cvtColor(gpuMat,gpuMat,COLOR_BGR2HSV);
	gBlur->apply(gpuMat,gpuMat);
	gpuMat.download(mat);
	inRange(mat, lowerBound, upperBound, mat);
	return mat;
}

/**
 * Set the limits of the threshhold.
 *
 * @param lowerHSV The lower bound on HSV values.
 * @param upperHSV The upper bound on HSV values.
 */
void Threshholder::setLimits(const Vec3b &lowerHSV, const Vec3b &upperHSV){
	upperBound = upperHSV;
	lowerBound = lowerHSV;
}

/**
 * Default constructor
 *
 * @param gBlur The gaussian blur to apply to the matrix before threshholding.
 * @param lowerHSV The lower bound on HSV values.
 * @param upperHSV The upper bound on HSV values.
 */
Threshholder::Threshholder(const Ptr<cuda::Filter> &gBlur, const Vec3b &lowerHSV, const Vec3b &upperHSV){
	this->gBlur = gBlur;
	this->lowerBound = lowerHSV;
	this->upperBound = upperHSV;
	this->gpuMat = cuda::GpuMat();
	this->mat = Mat();
};
