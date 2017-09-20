/* HMM.cpp - Assignment 1 AI
 * Julia Guerrero Viu && Jaime Ferrando Huertas
 * Class HMM which represents a model of a Hidden Markov Model
 */

#include "HMM.hpp"
#include <cmath>
#include <limits>

using namespace std;

// -----------Auxiliary function ------------------

vector <double> elWise(const vector<double>& v1,const vector<double>& v2){
    vector<double> resul(v1.size());
    for (int i=0; i<v1.size();i++){
        resul[i]=v1[i]*v2[i];
    }
    return resul;
}

//-------------------------------------------------

HMM::HMM(const int st, const int obs){
    Matrix newA(st,obs);
    Matrix newB(st,obs);
    
    newA.shuffle();
    newB.shuffle();

    double average = 1/(double)st;
    double ep=0.05*average; 
    double fMax = average+ep;
    double fMin = average-ep;
    double sum=1;
    for (int j=0;j<st-1;j++){
        double f = (double)rand() / RAND_MAX;
        double v= fMin + f * (fMax - fMin);
        sum-=v;
        pi.push_back(v);
    }
    pi.push_back(sum);
    
    this->A=newA;
    this->B=newB;
}

HMM::HMM(const Matrix& A, const Matrix& B, const vector<double>& pi){
    this->A=A;
    this->B=B;
    this->pi=pi;
}

int HMM::getst() const{
    return A.getn();
}

int HMM::getobs() const{
    return B.getm();
}

Matrix HMM::getA() const{
    return A;    
}

Matrix HMM::getB() const{
    return B;
}


// uses scaling to calculate alpha and avoid underflow
double HMM::hmm1(const vector<int>& obs) const{

    int nobs = obs.size();
	vector<double> alpha,alpham;
   	Matrix Btrans=B.transpose();
   	alpham=elWise(pi,Btrans.getvector(obs[0]));
    vector<double> c(nobs,0);

    for (int i=0; i<alpham.size();i++){
        c[0] = c[0] + alpham[i];    
    }
    c[0] = 1/c[0];
    for (int i=0; i<alpham.size();i++){
        alpham[i] *= c[0];    
    }

   	//Recursive alpha 
   	for (int i=1; i< nobs;i++){
   		int currentObs=obs[i];

   		alpham=A.transpose()*alpham;

   		Matrix Btrans=B.transpose();
		vector <double> bi=Btrans.getvector(currentObs);
		alpha=elWise(alpham,bi);
        c[i]= 0;
        for (int j=0; j<alpha.size();j++){
            c[i] += alpha[j];      
        }
        c[i] = 1/c[i];
        for (int j=0; j<alpha.size();j++){
            alpha[j]*= c[i];      
        }
   	   	alpham=alpha;
    
        //we scale the alpha in each time step
	}

    double sumlogc = 0;
    for (int i=0; i<nobs; i++){
        sumlogc+=log(c[i]);
    }
	double sum=0;
   	for(auto it= alpha.begin(); it!=alpha.end();it++){
   		sum+=*it;
   	}

   	return exp(log(sum) - sumlogc);
}

// ------------------------- HMM3 -----------------------

tuple<Matrix,vector<double>> HMM::alphapass(const vector<int>& obs, const int it) const {
    
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


Matrix HMM::betapass (const vector<int>& obs, const int it, const vector<double>& c) const{
    int N= A.getn();
    Matrix beta(it,N);

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


tuple<vector<Matrix>,Matrix> HMM::gammaDigamma (const vector<int>& obs, const int it, const vector<double>& c, const Matrix& alpha, const Matrix& beta) const {
    
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

vector<double> HMM::gamma(const vector<int>&obs, const int it) const{
    tuple<Matrix,vector<double>> alphaandc= this->alphapass(obs,it);
    vector<double> c = get<1>(alphaandc);
    Matrix alpha = get<0>(alphaandc);
    Matrix beta = this->betapass(obs,it,c);

    tuple<vector<Matrix>,Matrix> gammas = this->gammaDigamma(obs,it,c, alpha,beta);

    return get<1>(gammas).getvector(it-1);
}

// -----------Auxiliary function ------------------
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

//-------------------------------------------------



void HMM::hmm3(const vector<int>& obs){
   // Start of the algorithm

    int maxIters = 100; //maximum number of re-estimation iterations
    double oldLogProb = - numeric_limits<double>::max();    //minus infinity (here the minimum double that can be represented)
    double logProb=0;

    int nobs= obs.size();

    int N = B.getn(); //number of states
    int M = B.getm(); //number of observations

    tuple<Matrix,vector<double>> alphaandc= this->alphapass(obs,nobs);
    vector<double> c = get<1>(alphaandc);
    Matrix alpha = get<0>(alphaandc);
    Matrix beta = this->betapass(obs,nobs,c);

    tuple<vector<Matrix>,Matrix> gammas = this->gammaDigamma(obs,nobs,c, alpha,beta);
    tuple<Matrix,Matrix,vector<double>> estimation = reestimation (get<0>(gammas), get<1>(gammas), obs, nobs, N, M);

    this->A=get<0>(estimation);
    this->B=get<1>(estimation);
    this->pi=get<2>(estimation);

    //compute log[P(O|lambda)]
    for(int i=0; i<nobs; i++){
        logProb = logProb + log(c[i]);
    }
    logProb = -logProb;

    for (int i = 1; i < maxIters && logProb > oldLogProb; i++){

        oldLogProb = logProb;

        alphaandc= this->alphapass(obs,nobs);
        c = get<1>(alphaandc);
        alpha = get<0>(alphaandc);
        beta = this->betapass(obs,nobs,c);
        gammas = this->gammaDigamma(obs,nobs,c,alpha,beta);
        estimation = reestimation (get<0>(gammas), get<1>(gammas), obs, nobs, N, M);

        this->A=get<0>(estimation);
        this->B=get<1>(estimation);
        this->pi=get<2>(estimation);
        A.avoidzeros();
        B.avoidzeros();
    
        //compute log[P(O|lambda)]
        logProb = 0;
        for(int i=0; i<nobs; i++){
            logProb = logProb + log(c[i]);
        }
        logProb = -logProb;
    }

}

string HMM::print(){
    string a ="Transition matrix A:\n" + A.print() + "Observation matrix B:\n" + B.print() + "Initial distribution pi:\n";
    for (int i=0;i<pi.size();i++){
        a = a + " " + to_string(pi[i]);
    }
    a = a+ '\n';
    return a;
}

int HMM::calculateState(const vector<int>& obs){
	Matrix Btrans=B.transpose();
	Matrix Atrans=A.transpose();
	vector<double> deltam=elWise(pi,Btrans.getvector(obs[0]));
	vector<int> states;
	Matrix StatesIndexes(obs.size(),A.getm());
	vector<double> delta;
	for (int i=1; i< obs.size();i++){
		delta.clear();
		for (int o=0;o<A.getn();o++){
			vector<double> currentObs=Btrans.getvector(obs[i]);
			vector<double> aux2(currentObs.size(),currentObs[o]);
			vector<double> aux=elWise(elWise(deltam,Atrans.getvector(o)),aux2);
			auto biggest=max_element(aux.begin(), aux.end());
			delta.push_back(*biggest);
			int indexBiggest=distance(begin(aux),biggest);
			StatesIndexes.addelement(indexBiggest,i,o);
		}
		
		deltam=delta;

		
	}
    auto deltaBiggest=max_element(delta.begin(), delta.end());
    return *deltaBiggest;
}


tuple<int,double> HMM::nextObs(const vector<int>& obs){
    vector<double> gamma = this->gamma(obs, obs.size());
    vector<double> nextstates = A*gamma;
    Matrix Btrans=B.transpose();
    vector<double> nextobs = Btrans*nextstates;

    int imax =0;
    double max=nextobs[0];
    for(int i=1;i<nextobs.size();i++){
        if(nextobs[i]>max){
            max=nextobs[i];
            imax =i;
        }
    }
    tuple<int,double> result(imax,max);
    return result;
}
