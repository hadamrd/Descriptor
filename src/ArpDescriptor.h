/*
 * ArpDescriptor.h
 *
 *  Created on: May 23, 2017
 *      Author: kmajdoub
 */

#ifndef SRC_ARPDESCRIPTOR_H_
#define SRC_ARPDESCRIPTOR_H_

#include "Descriptor.h"
#include "Tools.h"

class ArpDescriptor : public Descriptor
{

	private:
	int _arpRadial, _arpAngular, _size, _transformType;
	cv::Mat_<double> _phi, _rho;


	public:

	ArpDescriptor( int arpRadial, int arpAngular, cv::Size size, int trasformType = GRAY);
	~ArpDescriptor();
	/**
	 * This function calculate the ARP histogram of the source image and store it in the output vector.
	 * @param src the source image.
	 * @param features the output vector.
	 */
	void calcFeatures(const cv::Mat &src, std::vector<int> &features);

};

#endif /* SRC_ARPDESCRIPTOR_H_ */
