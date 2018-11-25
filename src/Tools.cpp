/*
 * Tools.cpp
 *
 *  Created on: Jun 8, 2017
 *      Author: kmajdoub
 */
#include "Tools.h"
using namespace cv;
using namespace std;

void Tools::toEdgesImg(const cv::Mat&src, cv::Mat& dst, double lt, double ht )
{
	cv::GaussianBlur(src, dst ,cv::Size(5,5) ,sqrt(2));
	cv::Canny(dst , dst, lt*298, ht*298);
}

 void Tools::toSquareImg(const cv::Mat& src, cv::Mat& dst, int size)
{
	int height  = src.rows, width = src.cols;

	int y = (size - height) / 2 ;
	int x = (size - width ) / 2 ;

	dst = cv ::Mat :: zeros(size, size, src.type());
	src.copyTo(dst(cv::Rect(x, y, width, height)));

}
 double Tools::getThreshVal_Otsu_8u( const cv::Mat& _src )
{
    cv::Size size = _src.size();
    if ( _src.isContinuous() )
    {
        size.width *= size.height;
        size.height = 1;
    }
    const int N = 256;
    int i, j, h[N] = {0};
    for ( i = 0; i < size.height; i++ )
    {
        const uchar* src = _src.data + _src.step*i;
        for ( j = 0; j <= size.width - 4; j += 4 )
        {
            int v0 = src[j], v1 = src[j+1];
            h[v0]++; h[v1]++;
            v0 = src[j+2]; v1 = src[j+3];
            h[v0]++; h[v1]++;
        }
        for ( ; j < size.width; j++ )
            h[src[j]]++;
    }

    double mu = 0, scale = 1./(size.width*size.height);
    for ( i = 0; i < N; i++ )
        mu += i*h[i];

    mu *= scale;
    double mu1 = 0, q1 = 0;
    double max_sigma = 0, max_val = 0;

    for ( i = 0; i < N; i++ )
    {
        double p_i, q2, mu2, sigma;

        p_i = h[i]*scale;
        mu1 *= q1;
        q1 += p_i;
        q2 = 1. - q1;

        if ( std::min(q1,q2) < FLT_EPSILON || std::max(q1,q2) > 1. - FLT_EPSILON )
            continue;

        mu1 = (mu1 + i*p_i)/q1;
        mu2 = (mu - q1*mu1)/q2;
        sigma = q1*q2*(mu1 - mu2)*(mu1 - mu2);
        if ( sigma > max_sigma )
        {
            max_sigma = sigma;
            max_val = i;
        }
    }
    return max_val;
}

 void Tools::toBinaryImg(const cv::Mat& _src, cv::Mat& dst, double binCoeff , bool withOtsu )
{
	double otsuCoeff = 1;
	if( withOtsu)
		otsuCoeff = getThreshVal_Otsu_8u( _src );
	threshold( _src, dst, binCoeff*otsuCoeff, 255, 0 );

}

 void Tools::toNormHist(const std::vector<int> &hist, std::vector<double> &nrmHist)
{
	double sumHist = 0;
	for (unsigned int k=0; k<hist.size(); k++) sumHist+=hist[k];
	for (unsigned int k=0; k<hist.size(); k++) nrmHist.push_back( hist[k]/sumHist);
}

 void Tools::getPolarCoordinate(cv::Mat_<double>& radius, cv::Mat_<double>& angle, int dim, bool circle )
{
	double Xi, Yj, D;
	radius = cv::Mat_<double>(dim, dim);
	angle  = cv::Mat_<double>(dim, dim);

	if(circle == INNER_CIRCLE)
		D = dim ;
	else if(circle == OUTER_CIRCLE)
		D = sqrt(2) * dim;
	else
		std::cout << "transformation type not well specified"<<std::endl;

	for(int i = 0; i < dim; i++)
	{
		Xi = (2 * i + 1 - dim)/D;       //Xi C [ -1 .... 1 ]  ; i: 0 .... dim-1
		for(int j = 0; j < dim; j++)
		{
			Yj = (2 * j + 1 - dim)/D;   //Yi C [ -1 .... 1 ]  ; j: 0 .... dim-1
			radius(i,j) = HYPOT(Xi, Yj);
			angle(i,j) = atan2(Yj, Xi);
		}
	}
}
