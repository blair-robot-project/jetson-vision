//
// Created by noah on 11/12/17.
//

#include "corner_checker.h"

/**
 * Default constructor.
 *
 * @param width The width of the screen, in pixels.
 * @param height The height of the screen, in pixels.
 * @param minDiag The minimum diagonal distance of the rectangle for it to be considered large enough
 * @param border The number of pixels around the edge of the screen where, if the corners are within those pixels, the
 *  object is on the edge of the screen and doesn't count.
 */
corner_checker::corner_checker(const int &width, const int &height, const int &minDiag, const int &border) {
	this->minDiag = minDiag;
	this->maxX = width-border;
	this->minX = border;
	this->maxY = height-border;
	this->minY = border;
}

/**
 * Check the given corners of a rectangle and check that the object is large enough and not on the edge of the screen.
 * @param corners The corners to check, in order top left, top right, bottom left, bottom right.
 * @return true if the object is a non-edge rectangle, false otherwise.
 */
bool corner_checker::check_corners(const vector<Point2f> &corners) {
	//Make sure they're unique
	if(corners[0] == corners[1] || corners[0] == corners[2] || corners[0] == corners[3] || corners[1] == corners[2] ||
	   corners[1] == corners[3] || corners[2] == corners[3]){
		return false;
	}

	//Check that the diagonals are at least a certain length and the points are a distance away from the edges.
	return !(norm(corners[0] - corners[3]) < minDiag || norm(corners[1] - corners[2]) < minDiag) &&
	       corners[0].x > minX && corners[2].x > minX && corners[0].y > minY && corners[1].y > minY &&
	       corners[1].x < maxX && corners[3].x < maxX && corners[2].y < maxY && corners[2].y < maxY;
}