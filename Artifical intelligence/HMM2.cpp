#include <iostream>
#include <vector>
#include "Matrix.cpp"
#include <algorithm>

using namespace std;

vector <double> elementWise(const vector<double>& v1,const vector<double>& v2){
    vector<double> resul(v1.size());
    for (int i=0; i<v1.size();i++){
        resul[i]=v1[i]*v2[i];
    }
    return resul;
}

vector <int> calculateStates(Matrix A,Matrix B,vector<double> pi,vector<int> obs){
	Matrix Btrans=B.transpose();
	Matrix Atrans=A.transpose();
	vector<double> deltam=elementWise(pi,Btrans.getvector(obs[0]));
	vector<int> states;
	Matrix StatesIndexes(obs.size(),A.getm());
	vector<double> delta;
	for (int i=1; i< obs.size();i++){
		delta.clear();
		for (int o=0;o<A.getn();o++){
			vector<double> currentObs=Btrans.getvector(obs[i]);
			vector<double> aux2(currentObs.size(),currentObs[o]);
			vector<double> aux=elementWise(elementWise(deltam,Atrans.getvector(o)),aux2);
			auto biggest=max_element(aux.begin(), aux.end());
			delta.push_back(*biggest);
			int indexBiggest=distance(begin(aux),biggest);
			StatesIndexes.addelement(indexBiggest,i,o);
		}
		
		deltam=delta;

		
	}
	auto deltaBiggest=max_element(delta.begin(), delta.end());
	int indexDB=distance(begin(delta),deltaBiggest);
	states.push_back(indexDB);

	for(int f=obs.size()-1;f>0;f--){
		indexDB=StatesIndexes.getelement(f,indexDB);
		states.push_back(indexDB);
	}
	reverse(states.begin(),states.end());

	//cout << "hi" ;

	return states;
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

   	vector<int> states=calculateStates(A,B,pi,obs);
   	for (int k=0;k<states.size();k++){
		cout << states[k] << " ";
	}


}