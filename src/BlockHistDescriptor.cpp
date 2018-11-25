/*
 * BlockHistDescriptor.cpp
 *
 *  Created on: May 23, 2017
 *      Author: kmajdoub
 */

#include "BlockHistDescriptor.h"

using namespace cv;
using namespace std;

BlockHistDescriptor::BlockHistDescriptor(int nbrBlock, int nbrBin):_nbrBlock(nbrBlock), _nbrBin(nbrBin){}


void BlockHistDescriptor::calcFeatures(const Mat& src, vector<int> &result)
{

	int* blckHist = new int[_nbrBin];
	for ( int k = 0; k < _nbrBin; k++) blckHist[k] = 0;
	int n;
	for (int l = 1; l <= _nbrBlock; l++)
	{
		for(int k = 1; k <= _nbrBlock; k++)
		{
			//the start and end indexes for the block
			int iStart = (k-1)*floor((src.rows-1)/_nbrBlock);
			int iEnd  = k*floor((src.rows-1)/_nbrBlock);
			int jStart = (l-1)*floor((src.cols-1)/_nbrBlock);
			int jEnd  = l*floor((src.cols-1)/_nbrBlock);
			//find the block histogram
			for ( int i = iStart; i<= iEnd; i++ )
				{
					for(int j = jStart; j<= jEnd; j++)
					{
						n = floor(src.at<uchar>(i,j)*_nbrBin/255.);
						blckHist[n]+=1;
					}
				}
			//add the block histogram to result vector and reinitialize it to zero
			for (int n = 0; n <_nbrBin; n++)
			{
				result.push_back(blckHist[n]);
				blckHist[n] = 0 ;
			}
		}
	}
	delete [] blckHist;
}
BlockHistDescriptor::~BlockHistDescriptor(){}
