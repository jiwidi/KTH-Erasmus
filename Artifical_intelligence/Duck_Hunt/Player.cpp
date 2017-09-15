#include "Player.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <tuple>
#include "HMMDuck.hpp"

namespace ducks
{

Player::Player()
{
}

const int S = 9;   //number of states of each HMM (number of possible movements of the birds)
const int O = 9;   //number of observations of each HMM (number of possible movements of the birds)
const double epsilon = 0.0001;
const int maxNumBirds = 20;

const double probToShoot = 1/(double)9;

Matrix auxA(S,S,epsilon);  
std::vector<Matrix> A(maxNumBirds,auxA);

Matrix auxB(S,O);
std::vector<Matrix> B(maxNumBirds,auxB);

std::vector<double> auxpi(S);
std::vector<std::vector<double>> pi(maxNumBirds);


int getIndex(EMovement mov){
    if( mov == MOVE_UP_LEFT) {
        return 0;    
    }
    else if ( mov == MOVE_UP) {
        return 1;    
    }
    else if (mov == MOVE_UP_RIGHT) {
        return 2;
    }
    else if (mov == MOVE_LEFT) {
        return 3;
    }
    else if (mov == MOVE_STOPPED) {
        return 4;
    }
    else if (mov == MOVE_RIGHT) {
        return 5;
    }
    else if (mov == MOVE_DOWN_LEFT) {
        return 6;
    }
    else if (mov == MOVE_DOWN) {
        return 7;
    }
    else if (mov == MOVE_DOWN_RIGHT){
        return 8;
    }
    else {
        return -1;
    }
}


EMovement getNextMovement(int ind){
    if(ind == 0) {
        return MOVE_UP_LEFT;    
    }
    else if (ind == 1) {
        return MOVE_UP;    
    }
    else if (ind == 2) {
        return MOVE_UP_RIGHT;
    }
    else if (ind == 3) {
        return MOVE_LEFT;
    }
    else if (ind == 4) {
        return MOVE_STOPPED;
    }
    else if (ind == 5) {
        return MOVE_RIGHT;
    }
    else if (ind == 6) {
        return MOVE_DOWN_LEFT;
    }
    else if (ind == 7) {
        return MOVE_DOWN;
    }
    else if (ind == 8){
        return MOVE_DOWN_RIGHT;
    }
    else {
        return MOVE_DEAD;
    }
}


Action Player::shoot(const GameState &pState, const Deadline &pDue)
{
    /*
     * Here you should write your clever algorithms to get the best action.
     * This skeleton never shoots.
     */

    // Main information about the environment
    int round = pState.getRound();
    size_t numberBirds = pState.getNumBirds();

    int timestep=(pState.getBird(0)).getSeqLength();

    // First, we just wait without shooting until having some information about the birds

    if (timestep < 90 && round == 0){
        return cDontShoot;
    }
    
    // Then we initialize the matrixes with the information we have
    else if (timestep == 90 && round == 0){

        // As the observations correspond to the states, we initialize B with the identity matrix
        for (int i=0; i<numberBirds; i++){
            B[i].identity();
            for (int j=0; j<B[i].getn();j++){
                for (int k=0; k<B[i].getm();k++){
                    if(B[i].getelement(j,k)==0){
                        B[i].addelement(epsilon,j,k);                    
                    }                
                }            
            }
            B[i].normalize();
        }

        // We initialize pi with random values
        for (int i=0; i<numberBirds; i++){
            double ep=0.005;
            double average = 1/(double)S;
            double fMax = average+ep;
            double fMin = average-ep;
            double sum=1;
            for (int j=0;j<S-1;j++){
                double f = (double)rand() / RAND_MAX;
                double v= fMin + f * (fMax - fMin);
                sum= sum-v;
                pi[i].push_back(v);
            }
            pi[i].push_back(sum);
        }
        
        std::vector<std::vector<EMovement>> obs(numberBirds);

        // We initialize A after 90 observations to try to have a better estimation of probabilities        
        for(int i=0; i<numberBirds; i++){
            Bird crB=pState.getBird(i);
            int nobs=crB.getSeqLength();
            
            for (int k=0;k<nobs;k++){
                obs[i].push_back(crB.getObservation(k));    
            }

//            cerr << "observations vector of bird: " << i << endl;            
//            for (int k=0;k<nobs;k++){
//                cerr << obs[i][k] << " ";    
//            }
            cerr << endl;
            int lastObs = getIndex(obs[i][0]);
            int currentObs;
            for (int k=1;k<nobs;k++){
                 currentObs = getIndex(obs[i][k]);
                 if(lastObs != -1){
                    A[i].addelement(A[i].getelement(lastObs,currentObs)+1,lastObs,currentObs);
                 }
                 lastObs=currentObs;
            }
            A[i].normalize(); 
            cerr <<"A matrix: " << A[i].print() << endl;
            cerr << "B matrix: " << B[i].print() << endl;
            cerr << "Pi vector: ";
            for(int k=0;k<pi[i].size();k++){
                cerr << pi[i][k] << " ";
            }
            cerr << endl;
        }

        return cDontShoot;
    }
    
    else {

        std::vector<std::tuple <double,int>> probShootBird(numberBirds);

        std::vector<std::vector<int>> obsind(numberBirds);
        vector<vector<double>> alpha(numberBirds);
            
        for(int i=0; i<numberBirds; i++){
            Bird crB=pState.getBird(i);
            int nobs=crB.getSeqLength();
            for (int k=0;k<nobs;k++){
                obsind[i].push_back(getIndex(crB.getObservation(k)));    
            }
//            cerr << "antes de hmm3" << i << endl;

//            cerr <<"A matrix: " << A[i].print() << endl;
//            cerr << "B matrix: " << B[i].print() << endl;
//            cerr << "Pi vector: ";
//            for(int k=0;k<pi[i].size();k++){
//                cerr << pi[i][k] << " ";
//            }
//            cerr << endl;

            // compute hmm3
            tuple<Matrix,Matrix,std::vector<double>> model = hmm3(A[i],B[i],pi[i],obsind[i]);
            A[i]=get<0>(model);
            B[i]=get<1>(model);
            pi[i]=get<2>(model);
           
            cerr <<"A matrix: " << A[i].print() << endl;
            cerr << "B matrix: " << B[i].print() << endl;
            cerr << "Pi vector: ";
            for(int k=0;k<pi[i].size();k++){
                cerr << pi[i][k] << " ";
            }
            cerr << endl;

            
            cerr << "despues de hmm3" << i << endl;
        
            //compute HMM1, the alphas
            Matrix aux = get<0> (alphapass(A[i],B[i],pi[i],obsind[i], obsind[i].size()));
            alpha[i] = aux.getvector(aux.getn()-1);


            cerr << "alpha vector: ";
            for(int k=0;k<alpha[i].size();k++){
                cerr << alpha[i][k] << " ";
            }
            cerr << endl; 
             cerr << "despues de hmm1" << i << endl;

            // We take the decision (to shoot or not to shoot)
        
            //calculate the sum of alphas and the state with more probability
            double sum=0;
            int maxi=0;
            for(int j=0;j<alpha[i].size();j++){
                sum+=alpha[i][j];
                if(alpha[i][j]>alpha[i][maxi]){
                    maxi = j;                
                }
            }
//            cerr << "max prob" << alpha[i][maxi] << "sum "<< sum << endl;
            //decision
            if(alpha[i][maxi]>(probToShoot*sum)){
                std::tuple<double,int> aux(alpha[i][maxi],maxi);
                probShootBird[i]=aux;           
            }
            else {
                std::tuple<double,int> aux(0,-1);
                probShootBird[i]=aux;                
            }
        }
        
        //final decision, if some of the birds has probToShoot>0 we shoot at the maximum values
        std::tuple<double,int> goalBird(0,-1);
        int goalBirdNumber = 0;
        for(int i=0; i<numberBirds; i++){
            if(get<0>(probShootBird[i]) > get<0>(goalBird)){
                goalBird=probShootBird[i];
                goalBirdNumber = i;            
            }
        }
        
        if((get<0>(goalBird)) > 0){
            cerr << "we shoot in timestep " << timestep << " round" << round << endl;
            return Action(goalBirdNumber,getNextMovement(get<1>(goalBird)));
        }
        else{
            cerr << "we dont shoot in timestep " << timestep << " round" << round << endl;
            return cDontShoot;        
        }
       
    }

    // This line choose not to shoot
    return cDontShoot;
}

std::vector<ESpecies> Player::guess(const GameState &pState, const Deadline &pDue)
{
    /*
     * Here you should write your clever algorithms to guess the species of each bird.
     * This skeleton makes no guesses, better safe than sorry!
     */

    std::vector<ESpecies> lGuesses(pState.getNumBirds(), SPECIES_UNKNOWN);
    return lGuesses;
}

void Player::hit(const GameState &pState, int pBird, const Deadline &pDue)
{
    /*
     * If you hit the bird you are trying to shoot, you will be notified through this function.
     */
    std::cerr << "HIT BIRD!!!" << std::endl;
}

void Player::reveal(const GameState &pState, const std::vector<ESpecies> &pSpecies, const Deadline &pDue)
{
    /*
     * If you made any guesses, you will find out the true species of those birds in this function.
     */
}


} /*namespace ducks*/
