/*
 * dscDataBase.h
 *
 *  Created on: May 25, 2017
 *      Author: kmajdoub
 */

#ifndef SRC_DSCDATABASE_H_
#define SRC_DSCDATABASE_H_
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <dirent.h> // for linux systems
#include <sys/stat.h> // for linux systems




template<typename sT>
struct imageData
{
  std::string imgName, imgNumber, imgClass;
  std::vector<sT> features;
};


template<typename cT>
class DscDataBase
{

	private:
		int _nbrRows, _nbrCols;
		std::vector<imageData<cT> > _data;


	public:
		//implementation of constructor
		DscDataBase<cT>():_nbrRows(0),_nbrCols(4),_data(std::vector<imageData<cT> >()){}

		~DscDataBase<cT>()
		{
			//_data.clear();
		}

		DscDataBase<cT>(std::string fileName):_nbrCols(4)
		{
			_nbrRows = 0 ;
			loadDataBase(fileName);
		}
		/**
		 * This function load the database from a .csv file
		 * @param fileName the path of the file.
		 */
		void loadDataBase(std::string fileName)
		{
			std::ifstream file ( fileName ); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
			std::string line;
			std::string token;
			imageData<cT> dbRow;


			while (file.good() )
			{
				getline ( file, dbRow.imgName, ',' );
				getline ( file, dbRow.imgClass, ',' );
				getline ( file, dbRow.imgNumber, ',' );
				getline ( file, line, '\n' );
				dbRow.features.clear();
				readLineToVect(line, ",", dbRow.features);
				_data.push_back(dbRow);
			}
			_nbrRows = _data.size()-1;

		}
		/**
		 * This function convert a complex string of the form x+yi to a complex ans
		 * return its respective complex number.
		 * @param cStr the string that represent the complex number.
		 */
		std::complex<double> stoc(std::string &cStr)
		{
			size_t pos;
			double realP =0, imgP=0;
			std::string token;
			std::complex<double> result;
			int rps = 1;
			if((cStr.substr(0,2).find("-")) != std::string::npos )
			{
				rps = -1;
				cStr.erase(0,2);
			}

			if ((pos = cStr.find("-")) == std::string::npos)
			{
				pos = cStr.find("+");
			}
			token  = cStr.substr(0,pos);
			realP  = stod(token)*rps;
			cStr.erase(0,pos);

			if((pos   = cStr.find("i")) != std::string::npos)
			{
				token = cStr.substr(0,pos);
				imgP  = stod(token);
			}
			return std::complex<double>(realP,imgP);
		}

		void addRow(imageData<cT> imgData)
		{
			_data.push_back(imgData);
			_nbrRows+=1;
		}

		void readLineToVect(std::string line, std::string delimiter, std::vector<int> &dest)
		{
			size_t pos;
			std::string token;

			while ((pos = line.find(delimiter)) != std::string::npos)
			{
				token = line.substr(0, pos);
				dest.push_back(stoi(token));
				line.erase(0, pos + delimiter.length());
			}
			dest .push_back(stoi(line));
		}

		void readLineToVect(std::string line, std::string delimiter, std::vector<double> &dest)
		{
			size_t pos;
			std::string token;

			while ((pos = line.find(delimiter)) != std::string::npos)
			{
				token = line.substr(0, pos);
				dest.push_back(stod(token));
				line.erase(0, pos + delimiter.length());
			}
			dest.push_back(stod(line));
		}

		void readLineToVect(std::string line, std::string delimiter, std::vector<std::complex<double> > &dest )
		{
			size_t pos;
			std::string token;

			while ((pos = line.find(delimiter)) != std::string::npos)
			{
				token = line.substr(0, pos);
				if(token.find("i") != std::string::npos)
					dest.push_back(stoc(token));
				else
					dest.push_back(stod(token));
				line.erase(0, pos + delimiter.length());
			}
			if(token.find("i") != std::string::npos)
				dest.push_back(stoc(line));
			else
				dest.push_back(stod(line));
		}

		imageData<cT> getData(int i)
		{
	         if( i > _nbrRows)
	         {
				std::cout << "Index out of bounds" <<std::endl;
				// return first element.
				return _data[0];
	         }
	         return _data[i];
		}

		std::string getName(int i)

		{
	         if( i > _nbrRows)
	         {
				std::cout << "Index out of bounds" <<std::endl;
				// return first element.
				return _data[0].imgName;
	         }
	         return _data[i].imgName;
		}

		std::string getNumber(int i)
		{
	         if( i > _nbrRows)
	         {
				std::cout << "Index out of bounds" <<std::endl;
				// return first element.
				return _data[0].imgNumber;
	         }
			return _data[i].imgNumber;
		}

		int getNbrRows()
		{
			return _nbrRows;
		}

		std::vector<cT> getFeatures(int index)
		{
	         if( index > _nbrRows)
	         {
				std::cout << "Index out of bounds" <<std::endl;
				// return first element.
				return _data[0].features;
	         }
	         return _data[index].features;
		}

		imageData<cT> &operator[](int i)
			{
		         if( i > _nbrRows)
		         {
					std::cout << "Index out of bounds" <<std::endl;
					// return first element.
					return _data[0];
		         }
		         return _data[i];
		     }

		void save(char *path)
		{
		    std::ofstream file(path, std::ios::out | std::ios::ate);
		    int size = _data[0].features.size();
		    std :: string op, token ,strImg;
		    if(file)
		    {
		    	for (int i = 0; i < _nbrRows; i++)
		    	{
					file << _data[i].imgName << ",";
					file << _data[i].imgClass << ",";
					file << _data[i].imgNumber ;

					for(int j = 0; j < size; j++)
					{
						op = "";
						if(std::is_same<cT, std::complex<double> >::value)
						{
							token = std::to_string(std::real(_data[i].features[j]));
							if(std::imag(_data[i].features[j]) != 0)
							{
								strImg = std::to_string(std::imag(_data[i].features[j]));
								if(std::imag(_data[i].features[j]) > 0) op="+";
								token=token+op+strImg+"i";
							}
							file << " , "<<token;
						}
						else
						{
							file << " , "<<_data[i].features[j];
						}
					}
					file << std::endl;
		    	}
		    	file.close();
		    }
		    else
		    	std::cerr << "Path incorrect or file doesn't exist " <<std:: endl;
		}

		template <class dscT>
		void calcDescriptor(dscT& mdesc)
		{
			std::vector<cv::Mat > image(_nbrRows);
       		std::vector<cv::String> imgFile(_nbrRows);
			size_t pos = 0;
			for (int j = 0; j < _nbrRows; j++)
			{
				//////
	    		imgFile[j] = _data[j].imgName;
	    		pos        = imgFile[j].substr(2,imgFile[j].length()).find("'");
	    		imgFile[j] = imgFile[j].substr(2,pos);
	    		imgFile[j] = imgFile[j]+".MAT.jpg";
	    		//////
	    		image[j]   = cv::imread(imgFile[j].c_str(),0);
	    		//////
	    		if(!image[j].data)
	    		{
	    			std::cout << "no image data" << std ::endl;
	    		}

			}
			//std ::cout << _nbrRows << std::endl;
	    	for (int j = 0; j < _nbrRows; j++)
	    	{
	    		 std::cout << "image file = " << imgFile[j] <<std::endl;
	    		_data[j].features.clear();
	    		 mdesc.calcFeatures(image[j], _data[j].features);
	    	}
	    	std::cout << "end " << std::endl;
		}

		template <class dscT>
		void loadFromFolder(cv::String folderPath, cv::String imgExtention,dscT& mdesc )//we can add a classifier to fill the imgClass
		{

			std::vector<cv::String> imgPath;
			cv::Mat img;

			cv::glob(folderPath+"/"+imgExtention, imgPath, false);

			size_t count = imgPath.size();
			_nbrRows = count;
			_data = std::vector<imageData<cT> >(count);

			for (size_t i=0; i<count; i++)
			{
				_data[i].imgNumber = "'img_"+std::to_string(i)+"'";
				_data[i].imgClass = "'none'";
				imgNameFromPath(imgPath[i], _data[i].imgName);
				img = cv::imread(imgPath[i].c_str(),0);
				_data[i].features = std::vector<cT>();
				mdesc.calcFeatures(img, _data[i].features);
			}


		}

		void imgNameFromPath(cv::String imgPath, std::string& imgName)
		{
			size_t pos = 0;
			while ((pos = imgPath.find("/")) != std::string::npos)
			{
				imgPath.erase(0, pos + 1);
			}
			pos = imgPath.find(".");
			imgName = "'"+imgPath.substr(0,pos)+"'";

		}




		void calcCovMatrix(cv::Mat_<cT> &dst)
		{
			std::vector<cT> cvMrow, mf;
			int size = _data[0].features.size();
			dst = cv::Mat_<cT>(size,size);
			calcMeanFeatures(mf);
			for(int k=0; k<size;k++)
			{
				for(int l=0; l<size; l++)
				{
					dst(k,l) = getCov(k,l,mf[k],mf[l]);
				}
			}

		}

		cT getCov(int k, int l, cT meanK, cT meanL)
		{
			cT Ckl=0;
			for (int i=0; i< _nbrRows; i++)
			{
				Ckl+=_data[i].features[k]*_data[i].features[l];
			}
			Ckl = Ckl/_nbrRows;
			return Ckl-meanK*meanL;
		}

		void calcMeanFeatures(std::vector<cT> &dst)
		{
			int size = _data[0].features.size();
			cT mean;
			for (int i=0; i<size; i++)
			{
				for (int j= 0; j< _nbrRows; j++)
				{
					mean += _data[j].features[i];
				}
				dst.push_back(mean/_nbrRows);
			}
		}



		void showFeatures(int i)
		{
	         if( i > _nbrRows)
	         {
				std::cout << "Index out of bounds" <<std::endl;
				// return first element.;
	         }
	         else
	         {
	        	 for (unsigned int cnt =0; cnt<_data[i].features.size(); i++)
	        		 std::cout<<_data[i].features[cnt]<<",";
	        	 std::cout<<std::endl;

	         }
		}
};



#endif /* SRC_DSCDATABASE_H_ */
