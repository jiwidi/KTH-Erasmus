#include "Player.hpp"
#include <cstdlib>
#include <iostream>
#include "HMM3Duck.cpp"

namespace ducks
{

Player::Player()
{
}

const int S = 9;   //number of states of each HMM (number of possible movements of the birds)
const int O = 9;   //number of observations of each HMM (number of possible movements of the birds)
const double epsilon = 0.0001;
const int maxNumBirds = 20;

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
    else if ( mov == 1) {
        return MOVE_UP;    
    }
    else if (mov == 2) {
        return MOVE_UP_RIGHT;
    }
    else if (mov == 3) {
        return MOVE_LEFT;
    }
    else if (mov == 4) {
        return MOVE_STOPPED;
    }
    else if (mov == 5) {
        return MOVE_RIGHT;
    }
    else if (mov == 6) {
        return MOVE_DOWN_LEFT;
    }
    else if (mov == 7) {
        return MOVE_DOWN;
    }
    else if (mov == 8){
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

    if (timestep < 90 && round == 1){
        return cDontShoot;
    }
    
    // Then we initialize the matrixes with the information we have
    else if (timestep == 90 && round == 1){

        // As the observations correspond to the states, we initialize B with the identity matrix
        for (int i=0; i<numberBirds; i++){
            B[i].identity();
        }

        // We initialize pi with random values
        vector<double> pi(9);
        double ep=0.005
        sum=1;
        for (int i=0;i<9-1;i++){
            double r1=((double) rand() / (RAND_MAX));
            double r2=((double) rand() / (RAND_MAX));
            double v=1/9+r1*ep-(r2*ep);
            sum= sum-v;
            pi.push_back(v);
        }
        pi.push_back(sum);
        
                
       
        std::vector<std::vector<EMovement>> obs(numberBirds);

        // We initialize A after 90 observations to try to have a better estimation of probabilities        
        for(int i=0; i<numberBirds; i++){
            Bird crB=pState.getBird(i);
            int nobs=crB.getSeqLength();
            
            for (int k=0;k<nobs;k++){
                obs[i].push_back(crb.getObservation(k));    
            }
            int lastObs = getIndex(obs[i][0]);
            int currentObs;
            for (int k=1;k<nobs;k++){
                 currentObs = getIndex(obs[i][k]);
                 if(lastObs != -1){
                    A[i].addelement(A[i].getelement(lastObs,currentObs)+1,lastObs,currentObs);
                 }
            }
            A[i].normalize(); 
        }
    }
    
    else {
        // We read the values of the matrixes

        // We compute HMM3

        // We compute HMM1
        
        // We take the decision (shoot or not to shoot)
    }


    // This line choose not to shoot
    return cDontShoot;

    //This line would predict that bird 0 will move right and shoot at it
    //return Action(0, MOVE_RIGHT);
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
