/*
 * Similarity.h
 *
 *  Created on: Jun 7, 2017
 *      Author: kmajdoub
 */

#ifndef SRC_SIMILARITY_H_
#define SRC_SIMILARITY_H_
#include <cv.h>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include "DscDataBase.h"

struct mean_var
{
	double mean;
	double var;
};

class Similarity
{

public :
/**
 * this function calculate the Kullback–Leibler divergence between the tow discrete probability vectors.
 * @param p1 the first probability density.
 * @param p2 the second probability density.
 */
static double klDiv(std::vector<double> &p1, std::vector<double> &p2);
/**
 * this function calculate the Jeffrey divergence between the tow discrete probability vectors.
 * @param p1 the first probability density.
 * @param p2 the second probability density.
 */
static double jfDiv(std::vector<double> &p1, std::vector<double> &p2);
/**
 * This function calculate the similarity between tow block histograms using Jeffrey divergence
 * @param h1 the first histogram
 * @param h2 the second histogram
 * @param nbrBlock the number of blocks
 * @param nbrBin the number of bins
 */
static double blockHistSim(std::vector<int> &h1,std::vector<int> &h2, int nbrBlock, int nbrBin);
/**
 * This function calculate the similarity between tow histograms using Jeffrey divergence
 * @param h1 the first histogram
 * @param h2 the second histogram
 */
static double histSim(std::vector<int> &h1,std::vector<int> &h2);
/**
 * This function calculate the standard similarity between tow Zernik moments descriptors
 * the result is the normalized sum of the difference between the module of Zernik moment.
 * @param v1 the first Zernik moments vector.
 * @param v2 the second Zernik moments vector.
 */
static double ZMStandardSim(std::vector<std::complex<double> > &v1 , std::vector<std::complex<double> > &v2);

/**
 * This function calculate the norm L1 between tow vectors
 * @param v1 the first vector.
 * @param v2 the second vector.
 */
template <typename T>
static double normL1(std::vector<T> v1, std::vector<T> v2)
{
	if(v1.size() != v2.size())
	{
		std::cout << "vectors must have same size" << std::endl;
		return -1;
	}
	double nmL1 =0;
	int N = v1.size();
	for (int k =0; k<N; k++)
	{
		nmL1 += fabs(v1[k]-v2[k]);
	}
	return nmL1/N;
}

/**
 * This function calculate the norm L2 between tow vectors
 * @param v1 the first vector.
 * @param v2 the second vector.
 */
template <typename T>
static double normL2(std::vector<T> v1, std::vector<T> v2)
{
	if(v1.size() != v2.size())
	{
		std::cout << "vectors must have same size" << std::endl;
		return -1;
	}
	double nmL2 =0;
	int N = v1.size();
	for (int k =0; k<N; k++)
	{
		nmL2 += pow(abs(v1[k]-v2[k]),2);
	}
	return sqrt(nmL2)/N;
}

/**
 * This function calculate the mahalanobis distance between a vector and a database.
 * @param v the vector.
 * @param dbs the database.
 */
template<typename T>
static double mahalanobisDist(std::vector<T> v, DscDataBase<T> &dbs )
{
	double sim =0.;
	cv::Mat_<double> covM , invCovM;
	std::vector<T> meanV;
	dbs.calcMeanFeatures(meanV);
	dbs.calcCovMatrix(covM);
	invCovM = covM.inv();
	int N = v.size();

	for(int i=0; i<N; i++)
	{
		for(int j=0; j<N; j++)
		{
			sim+=(v[i]-meanV[i])*invCovM(i,j)*(v[j]-meanV[j]);
		}
	}
	return sqrt(sim);
}
/**
 * This function return the mean and variance between tow databases according to a
 * similarity measure.
 * @param dbs1 the first database.
 * @param dbs2 the second database
 * @param simMesure the similarity measure operator.
 */
template <typename T, typename simOp>
static mean_var meanVarSimDataBase(DscDataBase<T> &dbs1, DscDataBase<T> &dbs2, simOp simMesure )
{
	std::vector<double> simVect;

	if(dbs1.getNbrRows() != dbs2.getNbrRows())
	{
		std::cout<<"data bases must have same dimentions"<<std::endl;
		return mean_var();
	}

	for (int i=0; i<dbs1.getNbrRows(); i++)
	{
		simVect.push_back(simMesure(dbs1[i].features, dbs2[i].features));
	}

	return getMeanVar(simVect);
}
/**
 * This function calculate a similarity measure between tow vectors
 * @param v1 the first vector
 * @param v2 the second vector
 * @param simMesure the similarity measure function
 */
template<typename T, typename simOp>
static double getSim(std::vector<T> v1, std::vector<T> v2, simOp simMesure)
{
	return simMesure(v1,v2);
}
template<typename T, typename simOp>
/**
 * This function calculate a similarity measure between tow vectors that takes tow integer parameters
 * @param v1 the first vector
 * @param v2 the second vector
 * @param simMesure the similarity measure function
 * @param p1 the third parameter of the similarity measure
 * @param p2 the fourth parameter of the similarity measure
 */
static double getSim(std::vector<T> v1, std::vector<T> v2, simOp simMesure, int p1, int p2)
{
	return simMesure(v1,v2, p1, p2);
}
template<typename T>

/**
 * This function returns the mean and the variance of a vector
 * @param v the input vector
 */
static mean_var getMeanVar(std::vector<T> v)
{
	mean_var meanVar ={0,0};
	int N = v.size();
	for(int k=0; k<N; k++) meanVar.mean+=v[k];
	meanVar.mean=meanVar.mean/N;
	for(int k=0; k<N; k++) meanVar.var+=pow(v[k]-meanVar.mean,2);
	meanVar.var = sqrt(meanVar.var/N);
	return meanVar;
}
/**
 * This function calculate the mean and variance similarity between the vectors of tow databases
 * @param dbs1 the first database.
 * @param dbs2 the second database
 * @param simMesure the similarity measure.
 * @param p1 the third integer parameter of the similarity measure
 * @param p2 the fourth integer parameter of the similarity measure
 */
template <typename T, typename simOp>
static mean_var meanVarSimDataBase(DscDataBase<T> &dbs1, DscDataBase<T> &dbs2,simOp simMesure,int p1 , int p2 )
{
	std::vector<double> simVect;

	if(dbs1.getNbrRows() != dbs2.getNbrRows())
	{
		std::cout<<"data bases must have same dimensions"<<std::endl;
		return mean_var();
	}

	for (int i=0; i<dbs1.getNbrRows(); i++)
	{
		simVect.push_back(simMesure(dbs1[i].features,dbs2[i].features, p1, p2));
	}

	return getMeanVar(simVect);
}



};
#endif /* SRC_SIMILARITY_H_ */
