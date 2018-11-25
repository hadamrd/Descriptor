/*
 * ArtDescriptor.h
 *
 *  Created on: May 9, 2017
 *      Author: kmajdoub
 */

#ifndef SRC_ARTDESCRIPTOR_H_
#define SRC_ARTDESCRIPTOR_H_

#include "Tools.h"
#include "Descriptor.h"


class ArtDescriptor : public Descriptor
{

	private:
	cv::Mat_<std::complex<double> > **_artBasis;
	int _artRadial, _artAngular, _size, _transformType;
	cv::Mat_<double> _phi, _rho;


	public:


	ArtDescriptor( int, int, cv::Size, int transformationType = EDGES );
	~ArtDescriptor();
	/**
	 * This function generate the ART orthogonal basis basis with
	 * orders _artRadial and _artAngular
	 */
	void generateArtBasis();
	/**
	 * This function calculate the ART features of the input image and store it
	 * in the output vector.
	 * @param src the source image
	 * @param dest the output vector
	 */
	void calcFeatures(const cv::Mat& src, std::vector<std::complex<double> >&dest );


};

#endif /* SRC_ARTDESCRIPTOR_H_ */
