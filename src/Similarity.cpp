/*
 * Similarity.cpp
 *
 *  Created on: Jun 8, 2017
 *      Author: kmajdoub
 */


#include "Similarity.h"
#include "Tools.h"

using namespace cv;
using namespace std;


double Similarity::klDiv(vector<double> &p1, vector<double> &p2)
{
	double Dkl = 0  ;
	if(p1.size()!=p2.size() )
	{
		cout <<p1.size()<<" != "<<p2.size()<< " vectors must have same size"<<endl;
		return -1;
	}
	for (unsigned int i = 0; i < p1.size(); i++)
	{
		if(p2[i] !=  0 && p1[i] != 0)
			Dkl+=p1[i]*log(p1[i]/p2[i]);
	}
	return Dkl;
}

double Similarity::jfDiv(vector<double> &p1, vector<double> &p2)
{
	return 0.5*(klDiv(p1,p2)+klDiv(p2,p1));
}

double Similarity::histSim(vector<int> &h1,vector<int> &h2)
{
	vector<double> p1, p2;
	Tools::toNormHist(h1,p1);
	Tools::toNormHist(h2,p2);
	return jfDiv(p1,p2);

}
double Similarity::blockHistSim(vector<int> &v1,vector<int> &v2, int nbrBlock, int nbrBin)
{

	if(v1.size() != v2.size())
	{
		cout << "vectors must have same size"<<endl;;
		return -1;
	}

	double sim = 0.;

	for( int i =0; i <  nbrBlock*nbrBlock; i++)
	{
		vector<int>::const_iterator first1 = v1.begin()+i*nbrBin;
		vector<int>::const_iterator last1 =  v1.begin()+(i+1)*nbrBin;
		vector<int> h1(first1 , last1);
		vector<int>::const_iterator first2 = v2.begin()+i*nbrBin;
		vector<int>::const_iterator last2 =  v2.begin()+(i+1)*nbrBin;
		vector<int> h2(first2 , last2);
		sim += histSim(h1,h2);
	}
	return sim/(nbrBlock*nbrBlock);
}

double Similarity::ZMStandardSim(vector<complex<double> > &v1 , vector<complex<double> > &v2)
{
	double sim = 0 ;
	int N = v1.size();

	for(int i=0; i<N; i++)
	{
		sim+=fabs(abs(v1[i])-abs(v2[i]));
	}
	return sim/N;
}


