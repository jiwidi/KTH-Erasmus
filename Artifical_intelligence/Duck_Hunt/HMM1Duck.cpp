#include <iostream>
#include <vector>
#include "Matrix.cpp"

using namespace std;

vector <double> elementWise(const vector<double>& v1,const vector<double>& v2){
    vector<double> resul(v1.size());
    for (int i=0; i<v1.size();i++){
        resul[i]=v1[i]*v2[i];
    }
    return resul;
}

double maxAlpha(const Matrix& A, const Matrix& B, const vector<double>& pi, const vector<int>& obs){
	vector<double> alpha,alpham;
   	Matrix Btrans=B.transpose();
   	alpham=elementWise(pi,Btrans.getvector(obs[0]));
	

   	//Recursive alpha 
   	for (int i=1; i< obs.size();i++){
   		int currentObs=obs[i];

   		alpham=A.transpose()*alpham;

   		Matrix Btrans=B.transpose();
		vector <double> bi=Btrans.getvector(currentObs);
		alpha=elementWise(alpham,bi);
   	   	alpham=alpha;
	}
	
    double result = max_element(alpha.begin(), alpha.end());

    return result;
}
