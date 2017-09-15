#include "Player.hpp"
#include <cstdlib>
#include <iostream>

namespace ducks
{

Player::Player()
{
}

const int S = 10;   //number of states of the HMM (number of possible movements of the birds)
const int O = 10;   //number of observations of the HMM (number of possible movements of the birds)
const double epsilon = 0.0001;


Action Player::shoot(const GameState &pState, const Deadline &pDue)
{
    /*
     * Here you should write your clever algorithms to get the best action.
     * This skeleton never shoots.
     */

    // Main information about the environment
    int round = pState.getRound();
    size_t numberBirds = pState.getNumBirds();

    Matrix auxA(S,S,epsilon);  
    std::vector<Matrix> A(numberBirds,auxA);

    Matrix auxB(S,O);
    std::vector<Matrix> B(numberBirds,auxB);

    std::vector<double> auxpi(S);
    std::vector<std::vector<double>> pi(numberBirds);
    
    std::vector<std::vector<EMovement>> obs(numberBirds);

    //Initialize the matrixes (random values)

    for(int i=0; i<numberBirds; i++){
        Bird crB=pState.getBird(i);
        int nobs=crB.getSeqLength();
        
        for (int k=0;k<nobs;k++){
            obs[i].push_back(crb.getObservation(k));    
        }
        int lastObs = getIndex(obs[i][0]); //TODO: create getIndex
        int currentObs;
        for (int k=1;k<nobs;k++){
             currentObs = getIndex(obs[i][k]);
             if(lastObs != -1){
                A[i].addelement(A[i].getelement(lastObs,currentObs)+1,lastObs,currentObs);
             }
        }
        A[i].normalize();
        
        
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
