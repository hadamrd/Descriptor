/*
 * HuMomDescriptor.cpp
 *
 *  Created on: May 23, 2017
 *      Author: kmajdoub
 */

#include "HuMomDescriptor.h"

using namespace cv;
using namespace std;


HuMomDescriptor::HuMomDescriptor(int transformType): _transformType(transformType)
{}

void HuMomDescriptor::calcFeatures(const cv::Mat& src, std::vector<double> &result)
{
	Mat tmpImage ;
	Moments mom;

	if ( _transformType == EDGES )
	{
		Tools::toEdgesImg(src, tmpImage);
		mom = moments(tmpImage);
	}
	else if (_transformType == BINARY)
	{
		mom = moments(src, true);
	}
	else
	{
		mom = moments(src, false);
	}

	double hu[7];
	HuMoments(mom, hu);
	result.clear();
	for (unsigned int k=0; k<7; k++ )
		result.push_back(hu[k]);
}


HuMomDescriptor::~HuMomDescriptor(){};
