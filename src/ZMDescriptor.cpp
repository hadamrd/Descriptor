#include "ZMDescriptor.h"

using namespace cv;
using namespace std;


ZMDescriptor::ZMDescriptor(int order, Size size): _order(order)
{
	_size = max(size.height,size.width);

	_nbrPoly = generatePolyIndex(_nIndex, _mIndex, order);

	_zbp =  new Mat_<double>[_nbrPoly];

	_quot = new mpz_t *[_nbrPoly];
	_fact = new mpz_t[_order+1];

	for(int k = 0; k <_nbrPoly; k++)
		_quot[k] = new mpz_t [1+(_nIndex[k]-abs(_mIndex[k]))/2];

	generateFacts();
	generateQuots();
	Tools::getPolarCoordinate(_rho, _phi, _size, INNER_CIRCLE);
	generateZbf();

}

void  ZMDescriptor::gmpGetQuot(unsigned int n,unsigned int m,unsigned int s,mpz_t& quot)
{
	mpz_t denom;
	unsigned int i1,i2;
	mpz_init( denom);
	mpz_set_ui(denom,1);

	i1 = (n + abs(m))/2 - s;
	i2 = (n - abs(m))/2 - s;

	mpz_mul(denom,_fact[s],denom);
	mpz_mul(denom,_fact[i1],denom);
	mpz_mul(denom,_fact[i2],denom);
	mpz_divexact(quot,_fact[n-s],denom);
	mpz_clear(denom);

}
void ZMDescriptor::generateFacts()
{
	mpz_t tmp;
	mpz_inits(_fact[0], tmp, NULL);
	mpz_set_ui(_fact[0],1);
	for (int k = 1; k <= _order; k++)
	{
		mpz_init(_fact[k]);
		mpz_set_ui(tmp,k);
		mpz_mul(_fact[k],_fact[k-1],tmp);
	}
	mpz_clear(tmp);

}
void ZMDescriptor::generateQuots()
{
	for (int k=0; k<_nbrPoly; k++)
	{
		int n = _nIndex[k];
		int m = _mIndex[k];
		for (int s=0; s<=(n-abs(m))/2; s++)
		{
			mpz_init(_quot[k][s]);
			gmpGetQuot(n,m,s,_quot[k][s]);

		}
	}
}



int ZMDescriptor::generatePolyIndex(vector<int>& rIndex, vector<int>& pIndex, int order)
{
	int nbrPoly = 0;
		for (int  n = 0; n <= order; n++)
		{
			for(int m = 0; m <= n; m++)
			{
				if((n - abs(m)) % 2 == 0)
				{
					rIndex.push_back( n );
					pIndex.push_back( m );
					nbrPoly++;
				}
			}
		}
		return nbrPoly;
}


void  ZMDescriptor::generateZbf()
{
	mpf_t r , res, zbp;
	mpf_inits(r,res,zbp,NULL);

	for (int k= 0; k<_nbrPoly ; k++)
	{
		_zbp[k] = Mat_<double> :: zeros(_size,_size);
		int n = _nIndex[k], m = _mIndex[k];


		for (int i=0; i<_size; i++)
		{
			for(int j=0; j<_size; j++)
			{
				mpf_set_d(zbp, 0.0);
				if (_rho(i,j)<=1)
				{
					for (int s = 0; s  <= (n-abs(m))/2; s++)
					{
						mpf_set_d(r,_rho(i,j));
						mpf_pow_ui(r , r, n-2*s);
						mpf_set_z(res, _quot[k][s]);
						mpf_mul(res,r,res);

						if (s%2 == 0)
							mpf_add(zbp,zbp,res);
						else
							mpf_sub(zbp,zbp,res);

					}
					_zbp[k](i,j)=mpf_get_d(zbp);
				}
			}
		}
	}
	mpf_clears(r, res, NULL);

}


void ZMDescriptor::calcFeatures(const Mat& src, vector<complex<double> > &zerMoms)
{

	Mat sqrImg ;
	complex<double> zM;
	double DxDy = 4./(_size*_size);

	Tools::toSquareImg(src, sqrImg, _size);

	normalize(sqrImg,sqrImg,0.0,1.0,NORM_MINMAX, CV_64F);

	for(int k = 0; k < _nbrPoly; k++)
		{
		int n = _nIndex[k], m = _mIndex[k];
		zM = 0.f;
		for(int i = 0; i < _size; i++)
			for(int j = 0; j < _size; j++)
			{
				if (_rho(i,j)<=1 )
				{
						zM = zM + _zbp[k](i,j)*polar(1.,-m*_phi(i,j))*DxDy*sqrImg.at<double>(i,j);
				}
			}
		zerMoms.push_back( zM *(double)((1+n)/CV_PI));
		}

}

void ZMDescriptor::reconstructImage(vector<complex<double> > &zMom, Mat& dst)
{
	dst = Mat :: zeros(_size, _size, CV_64FC1);
	cout<<"reconstructing the image..."<<endl;
	int nbrCoeff = zMom.size();
		for (int i = 0; i < _size; i++ )
		{
			for(int j = 0; j < _size; j++)
			{
				if (_rho(i,j)<=1 )
				{
					for (int k = 0; k < nbrCoeff; k++)
						{
						if (_mIndex[k] == 0) dst.at<double>(i,j) += real(zMom[k]*_zbp[k](i,j));
						else
							dst.at<double>(i,j) += 2*_zbp[k](i,j)*real( zMom[k]*polar(1.,_mIndex[k]*_phi(i,j)));
						}
				}
			}
		}
		cout<<"pass"<<endl;

	//
//	namedWindow( "reconstruction final", CV_WINDOW_AUTOSIZE  );// Create a window for display
//	imshow( "reconstruction final",dst);
//	normalize(dst,dst,0,255,NORM_MINMAX,CV_8UC1);
//	imwrite("reconstructedImage.jpg",dst);
//	waitKey(0);



}



ZMDescriptor::~ZMDescriptor()
{
	for (int k=0; k<_nbrPoly; k++)
	{
		int n = _nIndex[k];
		int m = _mIndex[k];
		for (int s=0; s<=(n-abs(m))/2; s++)
		{
			mpz_clear(_quot[k][s]);
		}
		if ( k <= _order)
			mpz_clear(_fact[k]);
		_zbp[k].release();
	}
	_phi.release();
	_rho.release();

}


