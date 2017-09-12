#include <iostream>
#include <vector>
#include <tuple>
#include <limits>
#include <cmath>
#include "Matrix.cpp"

using namespace std;

vector <double> elementWise(const vector<double>& v1,const vector<double>& v2){
    vector<double> resul(v1.size());
    for (int i=0; i<v1.size();i++){
        resul[i]=v1[i]*v2[i];
    }
    return resul;
}

tuple<vector<double>,vector<double>> alphapass(const Matrix& A, const Matrix& B, const vector<double>& pi, const vector<int>& obs, const int it){
    
    Matrix Btrans=B.transpose();
    int N = Btrans.getm();

    vector<double> alpha(N), alpham(N); //just need to keep the actual alphat and alphat-1
    vector<double> c(it,0); 
    
   
    //Compute alpha0(i)
    alpha=elementWise(pi,Btrans.getvector(obs[0]));

    for(int i=0; i<N; i++){
        c[0]=c[0]+alpha[i];
    }
    
    //Scale alpha0(i)
    c[0]=1/c[0];
    
    for(int i=0; i<N; i++){
        alpha[i]=c[0]*alpha[i];
    }

    //Compute alphat(i)

    for(int t=1; t<it; t++){
        alpham=alpha;
        c[t]=0;
        for(int i=0; i<N; i++){
            alpha[i]=0;
            for(int j=0; j<N; j++){
                alpha[i]=alpha[i]+alpham[j]*A.getelement(j,i);
            }
            vector <double> bo=Btrans.getvector(obs[t]);
            alpha[i]=alpha[i]*bo[i];
            c[t]=c[t]+alpha[i];
        }
        
        //Scale alphat(i)
        c[t]=1/c[t];
        for(int i=0; i<N; i++){
            alpha[i]=c[t]*alpha[i];
        }
    }

    tuple<vector<double>,vector<double>> result (alpha,c);

    return result;
}



vector<double> betapass (const Matrix& A, const Matrix& B, const vector<int>& obs, const int it, const vector<double>& c){
    
    Matrix Btrans=B.transpose();
    int N = Btrans.getm();   

    vector<double> beta(N), betaM(N); 
    
    //Scale betat-1(i)
    for(int i=0; i<N; i++){
        beta[i]=c[it-1];
    }

    //Compute betat(i)
    for(int t=it-2; t>=0; t--){
        betaM=beta;
        vector <double> bo=Btrans.getvector(obs[t+1]);
        for(int i=0; i<N; i++){
            beta[i]=0;
            for(int j=0; j<N; j++){
                beta[i]=beta[i]+A.getelement(i,j)*bo[j]*betaM[j];
            }
            beta[i]=c[t]*beta[i];
        }
    }
    
    return beta;

}


tuple<vector<Matrix>,Matrix> gammaDigamma (const Matrix& A, const Matrix& B, const vector<double>& pi, const vector<int>& obs, const int it, const vector<double>& c){
    
    Matrix Btrans=B.transpose();
    int N = Btrans.getm();

    Matrix aux(N,N);
    vector<Matrix> digamma(it,aux);    // digamma is a vector of matrix to save it for all t
    Matrix gamma(it,N); //gamma is a matrix because we need to save it for all t
    
    double denom;
    for(int t=0; t<it-1; t++){
        denom=0;
        vector<double> alphat = get<0> (alphapass(A,B,pi,obs,t+1));
        vector<double> betat1 = betapass(A,B,obs,t+2,c);
        vector<double> bo=Btrans.getvector(obs[t+1]);
        for(int i=0; i<N; i++){
            for (int j=0; j<N; j++){
                 denom = denom + alphat[i] * A.getelement(i,j) * bo[j] + betat1[j];
            }
        }
        for(int i=0; i<N; i++){
            gamma.addelement(0,t,i);
            for(int j=0; j<N; j++){
                digamma[t].addelement((alphat[i] * A.getelement(i,j) * bo[j] + betat1[j]) / denom,i,j);
                double auxil = gamma.getelement(t,i);
                gamma.addelement(auxil + (digamma[t].getelement(i,j)),t,i);
            }
        }   
    }


    //Special case for gammat-1(i)
    denom = 0;
    vector<double> alphatmin1= get<0>(alphapass(A,B,pi,obs,it));
    for (int i=0; i<N; i++){
        denom = denom + alphatmin1[i];
    }
    for (int i=0; i<N; i++){
        gamma.addelement((alphatmin1[i])/denom,it-1,i);  
    }

    tuple<vector<Matrix>, Matrix> result (digamma, gamma);

    return result;

}

tuple<Matrix,Matrix,vector<double>> reestimation(const vector<Matrix>& digamma, const Matrix& gamma, const vector<int>& obs, const int it, const int N, const int M){

    Matrix A(N,N);
    Matrix B(N,M);
    vector<double> pi(N);

    // reestimate pi
    for(int i=0; i<N; i++){
        pi[i] = gamma.getelement(0,i);
    }

    // reestimate A
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            double numer = 0;
            double denom = 0;
            for(int t=0; t<it-1; t++){
                numer = numer + digamma[t].getelement(i,j);
                denom = denom + gamma.getelement(t,i);            
            }
            A.addelement(numer/denom, i, j);
        }    
    }

    // reestimate B
    for(int i=0; i<N; i++){
        for(int j=0; j<M; j++){
            double numer=0;
            double denom=0;
            for(int t=0; t<it; t++){
                if (obs[t]==j){
                    numer = numer + gamma.getelement(t,i);                
                }
                denom = denom + gamma.getelement(t,i);            
            } 
            B.addelement(numer/denom, i, j);
        }
          
    }
    
    tuple<Matrix,Matrix,vector<double>> result(A,B,pi);
    return result;
}

int main(){

    //First, we parse the input and get lambda and the observations vector
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

    // Start of the algorithm

    int maxIters = 100; //maximum number of re-estimation iterations
    double oldLogProb = - numeric_limits<double>::max();
    double logProb=0;

    int N = B.getn(); //number of states
    int M = B.getm(); //number of observations

    vector<double> c = get<1>(alphapass(A,B,pi,obs,nobs));

    tuple<vector<Matrix>,Matrix> gammas = gammaDigamma(A,B,pi,obs,nobs,c);
    tuple<Matrix,Matrix,vector<double>> estimation = reestimation (get<0>(gammas), get<1>(gammas), obs, nobs, N, M);

    A=get<0>(estimation);
    B=get<1>(estimation);
    pi=get<2>(estimation);

    //compute log[P(O|lambda)]
    for(int i=0; i<nobs; i++){
        logProb = logProb + log(c[i]);
    }
    logProb = -logProb;


    for (int i = 0; i < maxIters && logProb > oldLogProb; i++){

        oldLogProb = logProb;
        vector<double> c = get<1>(alphapass(A,B,pi,obs,nobs));
        tuple<vector<Matrix>,Matrix> gammas = gammaDigamma(A,B,pi,obs,nobs,c);
        tuple<Matrix,Matrix,vector<double>> newestimation = reestimation (get<0>(gammas), get<1>(gammas), obs, nobs, N, M);

        A=get<0>(newestimation);
        B=get<1>(newestimation);
        pi=get<2>(newestimation);
    
        //compute log[P(O|lambda)]
        logProb = 0;
        for(int i=0; i<nobs; i++){
            logProb = logProb + log(c[i]);
        }
        logProb = -logProb;

        cout << "iteration" << i << endl;
        cout << logProb;
        cout << oldLogProb;
        cout << endl;

    }

    //output A, B

    cout << N << " " << N;
    for(int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            cout << " " << A.getelement(i,j);
        }    
    }
    cout << endl;

    cout << N << " " << M;
    for(int i=0; i<N; i++){
        for (int j=0; j<M; j++){
            cout << " " << B.getelement(i,j);
        }    
    }
    cout << endl;
    
}
