/*
 * HuMomDescriptor.h
 *
 *  Created on: May 23, 2017
 *      Author: kmajdoub
 */

#ifndef SRC_HUMOMDESCRIPTOR_H_
#define SRC_HUMOMDESCRIPTOR_H_

#include "Tools.h"
#include "Descriptor.h"


class HuMomDescriptor :public Descriptor
{

private:
	int _transformType;

public:
	HuMomDescriptor(int transformType);
	HuMomDescriptor() :_transformType(BINARY){}
	~HuMomDescriptor();
	/**
	 * This function calculate the 7 Hu moments of the source image and store it
	 * in the output vector
	 * @param src the source image
	 * @param result the output vector
	 */
	void calcFeatures(const cv::Mat& src, std::vector<double> &result);



};

#endif /* SRC_HUMOMDESCRIPTOR_H_ */
