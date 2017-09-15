#include <iostream>
#include <vector>
#include <tuple>
#include <limits>
#include <cmath>
#include "Matrix.cpp"

using namespace std;

tuple<Matrix,vector<double>> alphapass(const Matrix& A, const Matrix& B, const vector<double>& pi, const vector<int>& obs, const int it){
    
    Matrix Btrans=B.transpose();
    int N = Btrans.getm();

    Matrix alpha(it,N);
    vector<double> c(it,0); 
    
    //Compute alpha0(i)   
    vector<double> bo = Btrans.getvector(obs[0]);
    for(int i=0; i<N; i++){
        alpha.addelement(pi[i]*bo[i],0,i);
        c[0]=c[0]+alpha.getelement(0,i);
    }
    
    //Scale alpha0(i)
    c[0]=1/c[0];
    
    for(int i=0; i<N; i++){
        alpha.addelement(c[0]*alpha.getelement(0,i),0,i);
    }

    //Compute alphat(i)

    for(int t=1; t<it; t++){
        c[t]=0;
        for(int i=0; i<N; i++){
            alpha.addelement(0,t,i);
            for(int j=0; j<N; j++){
                alpha.addelement(alpha.getelement(t,i)+alpha.getelement(t-1,j)*A.getelement(j,i),t,i);
            }
            vector <double> bo=Btrans.getvector(obs[t]);
            alpha.addelement(alpha.getelement(t,i)*bo[i],t,i);
            c[t]=c[t]+alpha.getelement(t,i);
        }
        
        //Scale alphat(i)
        c[t]=1/c[t];
        for(int i=0; i<N; i++){
            alpha.addelement(c[t]*alpha.getelement(t,i),t,i);
        }
    }

    tuple<Matrix,vector<double>> result (alpha,c);

    return result;
}


Matrix betapass (const Matrix& A, const Matrix& B, const vector<int>& obs, const int it, const vector<double>& c){
    int N= A.getn();
    Matrix beta(it,N);
    
    double prueba=0;

    // beta t-1
    for(int i=0; i<N; i++){
        beta.addelement(c[it-1],it-1,i);        
    }

    // beta pass
    for(int t = it-2; t>=0; t--){
        for(int i=0; i<N; i++){
            beta.addelement(0,t,i);
            for(int j=0; j<N; j++){
                beta.addelement((beta.getelement(t,i)+(A.getelement(i,j)*B.getelement(j,obs[t+1])*beta.getelement(t+1,j))),t,i);
            }
            beta.addelement((c[t]*beta.getelement(t,i)),t,i);
        }
    }

    return beta;
}


tuple<vector<Matrix>,Matrix> gammaDigamma (const Matrix& A, const Matrix& B, const vector<double>& pi, const vector<int>& obs, const int it, const vector<double>& c, const Matrix& alpha, const Matrix& beta){
    
    int N= A.getn();

    Matrix aux(N,N);
    vector<Matrix> digamma(it,aux);    // digamma is a vector of matrix to save it for all t
    Matrix gamma(it,N); //gamma is a matrix because we need to save it for all t
    
    double denom;

    for(int t=0; t<it-1; t++){
        denom=0;
        vector<double> alphat = alpha.getvector(t);
        vector<double> betat1 = beta.getvector (t+1);

        for(int i=0; i<N; i++){
            for (int j=0; j<N; j++){
                 denom = denom + alphat[i] * A.getelement(i,j) * B.getelement(j,obs[t+1]) * betat1[j];
            }
        }
        for(int i=0; i<N; i++){
            gamma.addelement(0,t,i);
            for(int j=0; j<N; j++){
                digamma[t].addelement(((alphat[i] * A.getelement(i,j) * B.getelement(j,obs[t+1]) * betat1[j]) / denom),i,j);
                gamma.addelement(gamma.getelement(t,i) + (digamma[t].getelement(i,j)),t,i);
            }
        }   
    }


    //Special case for gammat-1(i)
    denom = 0;
    vector<double> alphatmin1= alpha.getvector(it-1);
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
            for(int t=0; t<it; t++){
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



tuple<Matrix,Matrix,vector<double>> hmm3(const Matrix& A, const Matrix& B, const vector<double>& pi, const vector<int>& obs){
    // Start of the algorithm

    int maxIters = 100; //maximum number of re-estimation iterations
    double oldLogProb = - numeric_limits<double>::max();    //minus infinity (here the minimum double that can be represented)
    double logProb=0;

    int N = B.getn(); //number of states
    int M = B.getm(); //number of possible observations

    int nobs = obs.size(); //number of observations

    tuple<Matrix,vector<double>> alphaandc= alphapass(A,B,pi,obs,nobs);
    vector<double> c = get<1>(alphaandc);
    Matrix alpha = get<0>(alphaandc);
    Matrix beta = betapass(A,B,obs,nobs,c);

    tuple<vector<Matrix>,Matrix> gammas = gammaDigamma(A,B,pi,obs,nobs,c, alpha,beta);
    tuple<Matrix,Matrix,vector<double>> estimation = reestimation (get<0>(gammas), get<1>(gammas), obs, nobs, N, M);

    A=get<0>(estimation);
    B=get<1>(estimation);
    pi=get<2>(estimation);

    //compute log[P(O|lambda)]
    for(int i=0; i<nobs; i++){
        logProb = logProb + log(c[i]);
    }
    logProb = -logProb;

    for (int i = 1; i < maxIters && logProb > oldLogProb; i++){

        oldLogProb = logProb;

        alphaandc= alphapass(A,B,pi,obs,nobs);
        c = get<1>(alphaandc);
        alpha = get<0>(alphaandc);
        beta = betapass(A,B,obs,nobs,c);
        gammas = gammaDigamma(A,B,pi,obs,nobs,c,alpha,beta);
        estimation = reestimation (get<0>(gammas), get<1>(gammas), obs, nobs, N, M);

        A=get<0>(estimation);
        B=get<1>(estimation);
        pi=get<2>(estimation);
    
        //compute log[P(O|lambda)]
        logProb = 0;
        for(int i=0; i<nobs; i++){
            logProb = logProb + log(c[i]);
        }
        logProb = -logProb;
    }
    
    tuple<Matrix,Matrix,vector<double>> result(A,B,pi);
    return result;   

}
