/*
 * ZMDescriptor.h
 *
 *  Created on: May 9, 2017
 *      Author: kmajdoub
 */

#ifndef SRC_ZMDESCRIPTOR_H_
#define SRC_ZMDESCRIPTOR_H_

#include "Tools.h"
#include "Descriptor.h"
#include "gmp.h"


class ZMDescriptor : public Descriptor
{

	private:

	int _order, _nbrPoly, _size;
	cv::Mat_<double> _phi, _rho;
	std::vector<int> _nIndex, _mIndex;
	mpz_t   **_quot;
	mpz_t *_fact;
	cv::Mat_<double>  *_zbp;


	public:




	ZMDescriptor(int, cv::Size);

	~ZMDescriptor();
	/**
	 * This function generate the factorial list up to the max order
	 * and store it in the attribute _fact
	 */
	void generateFacts();
	/**
	 * This function generate the list of quotients that will serve to
	 * calculate the polynomials of the Zernik basis and store it
	 * in the attribute _quot
	 */
	void generateQuots();
	/**
	 * This function generate the Zernik basis polynomials and store it
	 * in the attribute _zbp
	 */
	void generateZbf();
	/**
	 * This function generate the indexes of Zernik polynomials
	 * The orders are stored in _nIndex and the repetitions are
	 * stored in _mIndex
	 */
	static int generatePolyIndex(std::vector<int>&, std::vector<int>&, int );
	/**
	 * This function extract zernik moments from the input image up to the order _order
	 * @param src the source image
	 * @param features the output vector
	 */
	void calcFeatures(const cv::Mat& src, std::vector<std::complex<double> > &features);
	/**
	 * This function reconstruct an image using Zernik moments up to an order
	 * @param src a vector containing the complex Zernik moments
	 * @param dest the output matrix that will contain the reconstructed image
	 */
	void reconstructImage(std::vector<std::complex<double> > &src, cv::Mat &dest);
	/**
	 * This function calculate the quotient Qnms of the Zernik polynomial using
	 * the gmp library for great numerical precision
	 * @param n the order
	 * @param the repetition
	 * @param s the s-th qutient of the polynomial
	 * @param quot the output value where the result is stored
	 */
	void  gmpGetQuot(unsigned int n,unsigned int m,unsigned int s,mpz_t& quot);

};

#endif /* SRC_ZMDESCRIPTOR_H_ */
