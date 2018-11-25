//============================================================================
// Name        : dfdf.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


//============================================================================
// Name        : MLP.cpp
// Author      : khalid&reda
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
using namespace std;
template <typename T>
void vshow(vector<T> v)
{
	for (unsigned int k=0; k<v.size();k++) cout << v[k] << ",";
	cout << endl;
}
class MLP
{

	public:
	vector<vector<vector<double> > > weights;
	vector<vector<double> > bias;
	vector<vector<double> > outs;
	vector<vector<double> > aouts;
	int nI, nO, nLayer;
		MLP():nI(0),nO(0),nLayer(0)
		{
			outs=vector<vector<double> >();
			aouts=vector<vector<double> >();
			bias=vector<vector<double> >();
		}

		double a(double x){
			return tanh(x);
		}
		double da(double x)
		{
			return 1-a(x)*a(x);
		}
		void addLayer(int ni, int no)
		{
			if (weights.empty()) nI = ni;
			vector<vector<double> > W(no,vector<double>(ni));
			bias.push_back(vector<double>(no));
			randomizeMatrix(W);
			weights.push_back(W);
			nO = no;
			nLayer++;
		}
		void amVMul(vector<vector<double> > &M,vector<double> &v,vector<double> &b ,vector<double> &res,vector<double> &ares)
		{
			int nRows = M.size(), nCols=M[0].size();

			double sum;
			for(int i=0; i<nRows; i++)
			{
				sum=0;
				for(int j=0; j<nCols; j++)
				{
					sum += M[i][j]*v[j]+b[j];

				}
				res.push_back(sum);
				cout<<"A"<<endl;
				ares.push_back(a(sum));
			}

		}
		void forward(vector<double> &inputs)
		{
			outs[0]=vector<double>(weights[0].size());
			aouts[0]=vector<double>(weights[0].size());
			amVMul(weights[0],inputs,bias[0],outs[0],aouts[0]);

			for(int i=0; i<nLayer-1; i++)
			{
				amVMul(weights[i+1],aouts[i],bias[i+1],outs[i+1],aouts[i+1]);
			}
		}
		void backward(vector<double> &targets, double nu = 0.3)
		{
			vector<vector<double> > deltas;
			vector<double> deltaOut(nO);
			//calculate outputDelta
			for (int k=0; k<nO; k++) deltaOut[k] = (aouts[nLayer-1][k]-targets[k])*da(outs[nLayer-1][k]);
			//update output weights and biases
			for (unsigned int i=0; i<weights[nLayer-1].size(); i++)
			{
				for (unsigned int j=0; j<weights[nLayer-1][0].size(); j++)
				{
					weights[nLayer-1][i][j] -= nu*deltaOut[i]*aouts[nLayer-2][j];
				}
				bias[nLayer-1][i] -= nu*deltaOut[i];
			}

			//////////////////////////////////for other layers///////////////////////////////////////////////////

			deltas.push_back(deltaOut);
			for(int k =nLayer-2; k>0 ; k--)
			{
				//calculate delta error for layer
				vector<double> hidden_deltas(weights[k].size());
				for (unsigned int j=0;j<weights[k+1][0].size(); j++)
				{
					double error = 0.0;
					for(unsigned int  i=0; i<weights[k+1].size();i++)
					{
						error +=  weights[k+1][i][j] * deltas.back()[i];
					}
					hidden_deltas[j] = error * da(outs[nLayer-k][j]);
				}
				deltas.push_back(hidden_deltas);
				//update weights for the layer
				for (unsigned int i=0; i<weights[k].size(); i++)
				{
					for (unsigned int j=0; j<weights[k][0].size(); j++)
					{
						weights[k][i][j] -= nu*hidden_deltas[i]*aouts[k-1][j];
					}
					bias[k][i] -= nu*hidden_deltas[i];
				}
			}
		}
		void runMLP(vector<vector<double> > &inputs, vector<vector<double> > &targets, int nIter)
		{

			int N = inputs.size();
			for (int k=0; k<nIter; k++)
			{
				for (int i=0; i<N;i++)
				{

					forward(inputs[i]);
					backward(targets[i]);
				}
				cout << meanSE(inputs, targets);
			}
		}
		double meanSE(vector<vector<double> > &inputs, vector<vector<double> > &targets)
		{
			int N = inputs.size();
			int M = inputs[0].size();
			double error =0;
			for(int i=0; i<N; i++)
			{
				forward(inputs[i]);
				for(int k =0; k<M;k++)
				{
					error+=0.5*(inputs[i][k]-aouts.back()[k])*(inputs[i][k]-aouts.back()[k]);
				}
			}
			return error/N;
		}
		void randomizeMatrix(vector<vector<double> > &W)
		{
			int nRows = W.size(), nCols=W[0].size();
			for(int i=0; i<nRows; i++)
			{
				for(int j=0; j<nCols; j++)
				{
					W[i][j] = (double)(rand()/RAND_MAX)-0.5;
				}
			}
		}
};
void generateDataBase(vector<vector<double> > &inputs, vector<vector<double> > &targets, int size)
{
	vector<double> inputRow;
	vector<double> targetRow;
	for (int k=0; k<size;k++)
	{
		inputRow.push_back((double)rand()/RAND_MAX);
		inputRow.push_back((double)rand()/RAND_MAX);
		if((inputRow[0]<.5 && inputRow[1]<.5)||(inputRow[0]>.5 && inputRow[1]>.5))
		{
			targetRow.push_back(1);
		}
		else
		{
			targetRow.push_back(-1);
		}
		//cout << "x1:" <<inputRow[0]<<" ; x2:"<<inputRow[1]<<" ; target:"<<targetRow[0]<<endl;
		inputs.push_back(inputRow);
		targets.push_back(targetRow);
		inputRow.clear();
		targetRow.clear();
	}
}
int main()
{
	int size = 100;
	vector<vector<double> > inputs, targets;
	generateDataBase(inputs,targets, size);
	MLP mlp;
	mlp.addLayer(2,5);
	mlp.addLayer(5,3);
	mlp.addLayer(3,1);
	mlp.runMLP(inputs,targets,1);



}



