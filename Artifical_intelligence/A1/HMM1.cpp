#include <iostream>
#include <vector>
#include "Matrix.cpp"
#include <cmath>

using namespace std;

vector <double> elWise(const vector<double>& v1,const vector<double>& v2){
    vector<double> resul(v1.size());
    for (int i=0; i<v1.size();i++){
        resul[i]=v1[i]*v2[i];
    }
    return resul;
}


double calcAlpha(const Matrix& A, const Matrix& B, const vector<double>& pi, const vector<int>& obs, const int nobs){
	vector<double> alpha,alpham;
   	Matrix Btrans=B.transpose();

    // calculate alpha0
   	alpham=elWise(pi,Btrans.getvector(obs[0]));
    vector<double> c(nobs,0);

    cerr << "mis alphitas" << endl;

    for (int i=0; i<alpham.size();i++){
        cerr << alpham[i] << " "; 
    }
    // scale alpha0
    for (int i=0; i<alpham.size();i++){
        c[0] = c[0] + alpham[i];    
    }
    c[0] = 1/c[0];
    cerr << "mis alphitas escaladas" << endl;
    cerr << "escala" << c[0] << endl; 
    for (int i=0; i<alpham.size();i++){
        alpham[i] *= c[0];  
        cerr << alpham[i] << " "; 
    }

   	//Recursive alpha 
   	for (int i=1; i< nobs;i++){
   		int currentObs=obs[i];

   		alpham=A.transpose()*alpham;

   		Matrix Btrans=B.transpose();
		vector <double> bi=Btrans.getvector(currentObs);
		alpha=elWise(alpham,bi);
        
        // scaling alpha
        c[i]= 0;
        for (int j=0; j<alpha.size();j++){
            c[i] += alpha[j];      
        }
        c[i] = 1/c[i];
        cerr << "alphitas" << i << endl;
        for (int j=0; j<alpha.size();j++){
            alpha[j]*= c[i];  
            cerr << alpha[j] << " ";    
        }
   	   	alpham=alpha;
	}

    // calculate the log probability to avoid underflow

    double sumlogc = 0;
    for (int i=0; i<nobs; i++){
        sumlogc+=log(c[i]);
    }
	double sum=0;
   	for(auto it= alpha.begin(); it!=alpha.end();it++){
   		sum+=*it;
   	}
    double logar = log(sum);

    // return the actual probability

   	return exp(logar - sumlogc);
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
   	double sum=calcAlpha(A,B,pi,obs,nobs);
   	cout << sum;


}
