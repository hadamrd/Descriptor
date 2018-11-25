/*
 * Tools.h
 *
 *  Created on: Jun 8, 2017
 *      Author: kmajdoub
 */

#ifndef SRC_TOOLS_H_
#define SRC_TOOLS_H_

#include <cv.h>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

#define HYPOT(x,y) sqrt(x * x + y * y)
#define WITH_OTSU true
#define INNER_CIRCLE false
#define OUTER_CIRCLE true
#define BINARY_COEFF 0.8
#define CANNY_HTRESH 0.4
#define CANNY_LTRESH 0.1
#define GRAY 1
#define EDGES 2
#define BINARY 0

class Tools
{

public:
	/**
	 * This function uses the Opencv Canny filter to generate the edges map of the source image.The Canny low threshold is set to 0.1
	 * and the height threshold is set to 0.4.
	 * @param src the source matrix.
	 * @param dst the destination matrix that will contain the edges map.
	 * @param lt Canny low threshold.
	 * @param ht canny height threshold.
	 */
	static void toEdgesImg(const cv::Mat&src, cv::Mat& dst, double lt =CANNY_LTRESH, double ht=CANNY_HTRESH );
	/**
	 * This function insert the source image in a black squared image of dimensions sizexsize.
	 * @param src the source matrix.
	 * @param dst the destination matrix.
	 * @param size the size of the squared destination matrix.
	 */
	static void toSquareImg(const cv::Mat& src, cv::Mat& dst, int size);
	/**
	 * This function returns the Otsu threshold for the source image
	 * @param src the source matrix
	 */
	static double getThreshVal_Otsu_8u( const cv::Mat& src );
	/**
	 * This function binarize the source image, the binary coefficient is set by default to 0.8.
	 * The flag withOtsu indicate whether to use the Otsu thresholding or not (it is set to true by default).
	 * @param src the source matrix.
	 * @param dst the destination matrix.
	 * @param the binarization coeff.
	 * @param the Otsu flag.
	 */
	static void toBinaryImg(const cv::Mat& src, cv::Mat& dst, double binCoeff = BINARY_COEFF , bool withOtsu = WITH_OTSU );
	/**
	 * This function normalize the input integer histogram to a vector of double representing its discrete density function.
	 * @param hist the integer histogram.
	 * @param nrmHist the destination vector where the normalized histogram will be stored .
	 */
	static void toNormHist(const std::vector<int> &hist, std::vector<double> &nrmHist);
	/**
	 * This function generate the polar coordinate map between image indexes and the grid [-1,1]x[-1,1].
	 * @param the matrix where the radius(i,j) will be stored.
	 * @param the matrix where the angle(i,i) will be stored.
	 * @param the dimension of the source matrix.
	 * @param a flag indicating whether to considerate the inner or the outer circle(it is set to inner circle by default).
	 */
	static void getPolarCoordinate(cv::Mat_<double>& radius, cv::Mat_<double>& angle, int dim, bool circle = INNER_CIRCLE );

};



#endif /* SRC_TOOLS_H_ */
