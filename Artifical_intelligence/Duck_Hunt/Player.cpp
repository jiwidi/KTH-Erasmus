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


Action Player::shoot(const GameState &pState, const Deadline &pDue)
{
    /*
     * Here you should write your clever algorithms to get the best action.
     * This skeleton never shoots.
     */

    // Main information about the environment
    int round = pState.getRound();
    size_t numberBirds = pState.getNumBirds();

    Matrix auxA(S,S);  
    vector<Matrix> A(numberBirds,auxA);

    Matrix auxB(S,O);
    vector<Matrix> B(numberBirds,auxB);

    vector<double> auxpi(S);
    vector<vector<double>> pi(numberBirds);

    //Initialize the matrixes (random values)


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
