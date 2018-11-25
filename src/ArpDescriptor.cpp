/*
 * ArpDescriptor.cpp
 *
 *  Created on: May 23, 2017
 *      Author: kmajdoub
 */

#include "ArpDescriptor.h"
using namespace cv;
using namespace std;




ArpDescriptor::ArpDescriptor(int arpRadial, int arpAngular, Size size, int transformType):_arpRadial(arpRadial),_arpAngular(arpAngular)
{
	_transformType = transformType;
	_size = max(size.height,size.width);
	Tools::getPolarCoordinate(_rho, _phi, _size, OUTER_CIRCLE);
}

void ArpDescriptor::calcFeatures(const Mat &src, vector<int> &features)
{
	int r, p;
	Mat tmpImg, sqrImg;
	features = vector<int>(_arpRadial*_arpAngular);

	if(_transformType == EDGES)
		Tools::toEdgesImg(src, tmpImg);
	else if(_transformType == BINARY)
		Tools::toBinaryImg(src, tmpImg);
	else
		tmpImg = src;

	Tools::toSquareImg(tmpImg, sqrImg, _size);
	for(int i = 0; i < _size; i++)
	{
		for(int j = 0; j < _size; j++)
		{
			r = floor(_rho(i,j) * _arpRadial);
			if(_phi(i,j) > 0)
				p = floor(_phi(i,j)/(CV_PI*2)*_arpAngular);
			else
				p = _arpAngular+floor(_phi(i,j)/(CV_PI*2)*_arpAngular);
			p = (p+_arpAngular-2)%_arpAngular;
			features[r+p*_arpRadial] += (int) (sqrImg.at<uchar>(i,j)/255);

		}
	}
}

ArpDescriptor::~ArpDescriptor()
{
	_phi.release();
	_rho.release();

}
