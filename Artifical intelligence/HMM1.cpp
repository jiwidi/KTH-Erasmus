#include <iostream>
#include <vector>
#include "Matrix.cpp"

using namespace std;

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
   	//Iteration i=1
   	vector<double> alpha,alpham;
   	Matrix Btrans=B.transpose();
   	alpham=elementWise(pi,Btrans.getvector(obs[0]));
	

   	//Recursive alpha 
   	for (int i=1; i< nobs;i++){
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
   	cout << sum;


}