/*
 * main.cpp
 *
 *  Created on: May 9, 2017
 *      Author: kmajdoub
 */

#include "Extractor.h"



using namespace cv;
using namespace std;

#define ABS "abs"
#define NON "non"
template<typename T>
void mShow(vector<T> v,string flag = NON )
{
	for (unsigned int k =0; k<v.size(); k++)
	{
		if(flag == NON)
			cout<< v[k] << " , ";
		else if(flag == ABS)
			cout << abs(v[k]) << " , " ;
	}
	cout << endl;
}






	int main( int argc, char** argv )
	{
		cout << rand()<<endl;
		cout <<rand()<<endl;

//		Mat image = imread(argv[2], 0);
//		if(  !image.data )
//		{
//		  printf( "No image data \n" );
//		  return -1;
//		}



//****load database and calc Mahalanobis distance between database and a Hu moments features vector
//		Mat image = imread(argv[1], 0);
//		HuMomDescriptor myDesc;
//		DscDataBase<double> mdbs(argv[2]);
//		cout << "database loaded" <<endl;
//		vector<double> V;
//		myDesc.calcFeatures(image, V);
//		double sim = Similarity::mahalanobisDist(V, mdbs);
//		cout <<"similarity : " << sim <<endl;
//******************************************************************


//*********load a database from folder aplying myDesc to images in the folder
//		HuMomDescriptor myDesc;
//		DscDataBase<double> mdbs;
//		String folderPath = "/home/kmajdoub/workspace/stage/batDataBase";
//		String imgExtention = "*.jpg";
//
//		mdbs.loadFromFolder(folderPath,imgExtention,myDesc);
//		cout << "load ended" << endl;
//		mdbs.save(argv[1]);
//****************************************************************************


//*******************apply ART desc to a pre-loaded database*******************
/////load and calc descriptor for art test /////////////
//		DscDataBase<complex<double> > mdbs1(argv[1]);
//		ArtDescriptor myDesc(5,5, Size(240,320),EDGES );
//		mdbs1.calcDescriptor(myDesc);
//		mdbs1.save(argv[2]);


//********load art 5x5 me and matlab return mean and var***********************************************
//		DscDataBase<complex<double> > mdbs1(argv[1]), mdbs2(argv[2]);
//		mean_var m_v = Similarity::meanVarSimDataBase(mdbs1,mdbs2,Similarity::normL1<complex<double> >) ;
//		cout << "mean : " << m_v.mean <<endl;
//		cout << "var : " << m_v.var <<endl;
//**************************************************************************************************



//***********test validation of implementation for BlockHist.. and ARP....
//		we first charge matlab database then we apply our descriptor
////    ArpDescriptor myDesc( 4 , 8 , Size(240,320) , EDGES);
//		BlockHistDescriptor myDesc(3,10);
//		DscDataBase<int> mdbs(argv[1]);
//		cout << "data base charged " << endl;
//		mdbs.calcDescriptor(myDesc);
//		cout << "descriptor extracted " << endl;
//		mdbs.save(argv[2]);
////    calc var and mean between matlab and c++
//**********************************************************************************

//*******load two databases one produced by our implementation of ART and the other produced by matlab
//*******calculate mean and variance between the tow
//		DscDataBase<complex<double> > mdbs1(argv[1]), mdbs2(argv[2]);
//		mean_var m_v = Similarity::meanVarSimDataBase(mdbs1, mdbs2, Similarity::normL1<complex<double> >);
//		cout << "mean : " << m_v.mean <<endl;
//		cout << "var : " << m_v.var <<endl;
//***********************************************************************************









////******hu moments test**************good
//	Mat img = imread(argv[1],0);
//	vector<double> features;
//	HuMomDescriptor myDesc;
//	myDesc.calcFeatures(image, features);
//	mShow(features);
//**************************************


//**********zernik moments simple test*************good
//Mat img = imread(argv[1],0);
//int order = 4;
//ZMDescriptor zmDes(order, img.size());
//vector<complex<double> > f;
//zmDes.calcFeatures(img, f);
//mShow(f,ABS);
//*************************************************


//***********************zernik descritor test******************good
//charge two images
//	Mat im1 = imread(argv[1],0), im2=imread(argv[2],0);
//	int order = atoi(argv[3]);
//create ZMDescriptor
//	ZMDescriptor zmDes(order, im1.size());
//
//	vector<complex<double> > f1, f2 ;
//	zmDes.calcFeatures(im1, f1);
//	zmDes.calcFeatures(im2, f2);
//	mShow(f1);
//	mShow(f2);

//similarity between the tow images
//	cout << ZMStandardSim(f1,f2) << endl;
//reconstruction of the image
//	Mat dst;
//	zmDes.reconstructImage(features,dst);





//*********************HISTO 9*10 test *********************good
//
//	vector<unsigned int> features;
//	Descriptor mbhd(3,10);
//	mbhd.calcFeatures(image, features);
//	mShow(features);




//***********art descriptor test****************************good
//	ArtDescriptor artDes(5,5, image.size(),EDGES );
//	artDes.generateArtBasis();
//	vector<complex<double> > features ;
//	artDes.calcFeatures(image, features);
//	mshowVector(features);






//****************arp descriptor test************************good

//	vector<int> features ;
//	ArpDescriptor marp( 4 , 8 , image.size() , EDGES);
//	marp.calcFeatures(image,features);
//
//	for( int p = 0; p < 8; p++)
//	{
//			for( int r = 0; r < 4; r++)
//				cout << features[p+r*8] << " , ";
//	}
//	cout << endl;




    return 0;
}
