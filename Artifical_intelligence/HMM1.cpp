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

double calcAlpha(Matrix A,Matrix B,vector<double> pi,vector<int> obs){
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
	double sum=0;
   	for(auto it= alpha.begin(); it!=alpha.end();it++){
   		sum+=*it;
   	}
   	return sum;
}





int main(){

    int na, ma;
    cin >> na >> ma;

    Matrix A(na,ma);

    double element;

    for(int i=0; i<na; i++){
        for (int j=0; j<ma; j++){
            cin >> element;
            A.addelement(element, i, j);
        }
    }

    int nb, mb;
    cin >> nb >> mb;

    Matrix B(nb,mb);

    for(int i=0; i<nb; i++){
        for (int j=0; j<mb; j++){
            cin >> element;
            B.addelement(element, i, j);
        }
    }

    int npi, mpi;
    cin >> npi >> mpi;

    vector<double> pi(mpi);

    for(int i=0; i<mpi; i++){
            cin >> element;
            pi[i]=element;
       
   }

	int nobs;
	cin >> nobs;
  	vector<int> obs(nobs);
   	int cobs;
   	for (int i=0; i<nobs;i++){
   		cin >> cobs;
   		obs[i]=cobs;

   	}
   	//Recursive alpha 
   	double sum=calcAlpha(A,B,pi,obs);
   	cout << sum;


}