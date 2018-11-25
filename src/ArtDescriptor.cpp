/*
 * ArtDescriptor.cpp
 *
 *  Created on: May 9, 2017
 *      Author: kmajdoub
 */




#include "ArtDescriptor.h"

using namespace cv;
using namespace std;



ArtDescriptor::ArtDescriptor( int artRadial, int artAngular,Size size, int transformType): _artRadial(artRadial), _artAngular(artAngular)
{
	_transformType = transformType;
	_size = max(size.width,size.height);
	_artBasis =  new Mat_<complex<double> >*[_artRadial];
	for(int i = 0; i < _artRadial; ++i)
		_artBasis[i] = new Mat_<complex<double> >[_artAngular];
	Tools::getPolarCoordinate(_rho, _phi, _size);
	generateArtBasis();
}



void ArtDescriptor::generateArtBasis()
{
	double  temp;
	for(int r = 0; r<_artRadial; r++)
		for(int p = 0; p < _artAngular; p++)
		{
			_artBasis[r][p] = Mat_<complex<double> > :: zeros(_size,_size);
			for(int i = 0; i < _size; i++)
				for(int j = 0; j < _size; j++)
					if(_rho(i,j) <= 1)
					{
						 if(r == 0)
							temp = 1;
						 else
							temp = 2 * cos(CV_PI * r * _rho(i,j));
						 _artBasis[r][p](i,j) = temp * polar(1., _phi(i,j)*p) / (2 * CV_PI);

					}
		}
}

void ArtDescriptor::calcFeatures(const Mat& src, vector<complex<double> >  &artCoeff)
{
	double DxDy = 4./(_size*_size);
	Mat sqrImg, tempImg;
	complex<double> coeff ;

	if(_transformType == BINARY )
		Tools::toBinaryImg(src, tempImg);
	else if (_transformType == EDGES )
		Tools::toEdgesImg(src, tempImg);
	else

		tempImg = src;

	Tools::toSquareImg(tempImg, sqrImg, _size);
	normalize(sqrImg,sqrImg,0.0,1.0,NORM_MINMAX, CV_64F);
//	namedWindow( "edges squared", CV_WINDOW_AUTOSIZE  );// Create a window for display
//	imshow( "edges squared",sqrImg);
//	waitKey(0);

	for(int p = 0; p <_artAngular ; p++)
	{
		for(int r = 0; r < _artRadial; r++)
		{
			coeff = 0. ;
			for(int i = 0; i < _size; i++)
			{
				for(int j = 0; j < _size; j++)
				{
						 coeff += conj(_artBasis[r][p](i,j))*sqrImg.at<double>(i,j)* DxDy;
				}
			}
			artCoeff.push_back(coeff);
		}
	}
}

ArtDescriptor::~ArtDescriptor()
{
	for(int r = 0; r<_artRadial; r++)
	{
		for (int p = 0; p<_artAngular; p++)
		{
			_artBasis[r][p].release();
		}
	}

	_rho.release();
	_phi.release();
}
