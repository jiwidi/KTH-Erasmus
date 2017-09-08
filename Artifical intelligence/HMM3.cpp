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

vector<double> calcAlpha(Matrix A,Matrix B,vector<double> pi,vector<int> obs,int it){
	vector<double> alpha,alpham;
  Matrix Btrans=B.transpose();
  alpham=elementWise(pi,Btrans.getvector(obs[0]));
	
  if(it>obs.size()){
    it=obs.size(); 
  }

  for (int i=1; i< it;i++){
   	int currentObs=obs[i];

   	alpham=A.transpose()*alpham;
		vector <double> bi=Btrans.getvector(currentObs);
		alpha=elementWise(alpham,bi);
  	alpham=alpha;
	}
  return alpha;

}

double sumAlpha(Matrix A,Matrix B,vector<double> pi,vector<int> obs,int it){
  vector <double> alpha=calcAlpha(A,B,pi,obs,it);
  double sum=0;
  for(auto it= alpha.begin(); it!=alpha.end();it++){
      sum+=*it;
  }
  return sum;
}

vector<double> calcBeta(Matrix A,Matrix B,vector<double> pi,vector<int> obs,it){
  vector<double> beta(A.getn());
  vector<double> betam(A.getn(),1);
  Matrix Btrans=B.transpose();
  if(it>obs.size()){
    it=obs.size();
  }
  for(int i=obs.size()-2;i>=it;i--){
    beta=A.transpose()*Btrans.getvector(obs[i]);
    vector<double> aux=elementWise(beta,betam);
    betam=beta;
  }

  return beta;
}

Matrix calcDiGamma(vector<double> alpha,vector<double> beta,Matrix obs,Matrix A,Matrix B){
  Matrix resul=Matrix(obs.getn(),obs.getn());
  for(int i=0;i<obs.getn();i++){
    for(int j=0;j<obs.getn();j++){
      double aux=alpha[i]*A.getelement(i,j)*
    }
  }
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


}