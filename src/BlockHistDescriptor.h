/*
 * BlockHistDescriptor.h
 *
 *  Created on: May 23, 2017
 *      Author: kmajdoub
 */

#ifndef SRC_BLOCKHISTDESCRIPTOR_H_
#define SRC_BLOCKHISTDESCRIPTOR_H_


#include "Tools.h"
#include "Descriptor.h"


class BlockHistDescriptor :public Descriptor
{

private:
	int _nbrBlock,_nbrBin;


public:
	BlockHistDescriptor(int nbrBlock, int nbrBin);
	~BlockHistDescriptor();
	/**
	 * This function calculate the block histogram descriptor of the input image and
	 * store it in the output vector
	 * @param src the source image
	 * @param result the output vector
	 */
	void calcFeatures(const cv::Mat& src, std::vector<int> &result);


};


#endif /* SRC_BLOCKHISTDESCRIPTOR_H_ */
