#include "Player.hpp"
#include <cstdlib>
#include <iostream>
#include <limits>
#include <algorithm>
#include <cmath>
#include "HMM.hpp"

namespace ducks
{

Player::Player()
{
}

const int numst = 1;
const int numsts = 5;
const int numobs = 9;
const int numSpecies = 6;
const int maxNumBirds = 20;
/* shooting */
const double confidenceShoot = 0.7;

std::vector<HMM> shootingHmm;

// confidence on each model will depend on lastPrediction
std::vector<int> lastPrediction(maxNumBirds,-1);
std::vector<int> confidence;

int lastShootingBird=-1;
std::vector<bool> birdAlive;

int total;
int correct;

/* guessing */
double ratio;

std::vector<std::vector<HMM>> model(numSpecies);

/* information for statistics */
std::vector<ESpecies> lastGuess;


// --------------- Auxiliary functions ------------------
ESpecies getSpecie(int ind){
    if(ind == 0) {
        return SPECIES_PIGEON;    
    }
    else if (ind == 1) {
        return SPECIES_RAVEN;    
    }
    else if (ind == 2) {
        return SPECIES_SKYLARK;
    }
    else if (ind == 3) {
        return SPECIES_SWALLOW;
    }
    else if (ind == 4) {
        return SPECIES_SNIPE;
    }
    else if (ind == 5) {
        return SPECIES_BLACK_STORK;
    }
    else {
        return SPECIES_UNKNOWN;
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

int numequals (const vector<ESpecies>& v1, const vector<ESpecies>& v2, const int n){
    int equal=0;    
    for (int i=0; i<n;i++){
        if (v1[i]==v2[i] && v1[i]>=0){
            equal++;
          }
    }
    return equal;
}

// --------------------------------------------------------------

Action Player::shoot(const GameState &pState, const Deadline &pDue)
{
    /*
     * Here you should write your clever algorithms to get the best action.
     * This skeleton never shoots.
     */
    int round = pState.getRound();
    size_t numberBirds = pState.getNumBirds();
    int boundary = 88 - numberBirds;

    int timestep=(pState.getBird(0)).getSeqLength();

    // we restart parameters
    if(timestep==1){
        shootingHmm.clear();
        birdAlive.clear();
        confidence.clear();
        total=0;
        correct=0;
        return cDontShoot;
    }
    
    else if(timestep < boundary){
        return cDontShoot;        
    }
    
    // in timestep 79 we train the hmm of each bird
    else if(timestep == boundary){
        for(int i=0; i<numberBirds; i++){
            birdAlive.push_back(true);
            confidence.push_back(0);
            HMM modelBird(numsts,numobs);
            std::vector<int> obsBird;
        
            Bird crB=pState.getBird(i);
            for(int j=0;j<crB.getSeqLength();j++){
                if(crB.wasAlive(j)){
                    obsBird.push_back(crB.getObservation(j));
                }
            }
            modelBird.hmm3(obsBird);
            shootingHmm.push_back(modelBird);
        }
        return cDontShoot;
    }
    
    // we calculate the most likely next movement of each bird and its probability
    else{

        std::vector<std::tuple <int,double>> probShootBird;

        for(int i=0;i<numberBirds;i++){
            std::vector<int> obsBird;
            Bird crB=pState.getBird(i);
            for(int j=0;j<crB.getSeqLength();j++){
                if(crB.wasAlive(j)){
                    obsBird.push_back(crB.getObservation(j));
                }
            }
            if(timestep%3==0){// we train the model again every three timesteps
                shootingHmm[i].hmm3(obsBird);
            }
            tuple<int,double> nextMove = shootingHmm[i].nextObs(obsBird);
            
            // if 3 lastPredictions were correct we can shoot, if not we can't (probability = 0)
            if(lastPrediction[i]==obsBird[obsBird.size()-1]){
                confidence[i]++;
            }
            if(confidence[i]<3 || lastPrediction[i]!=obsBird[obsBird.size()-1]){
                tuple<int,double>aux((get<0>(nextMove)),0);
                nextMove=aux;
            }
            
            //Try to guess to avoid shooting Black_stork
            //get the probability of this observation sequence with each of the hmm
            std::vector<double> probs(numSpecies);
            for(int j=0;j<numSpecies;j++){
                double sum=0;
                for(int k=0;k<model[j].size();k++){
                    sum = sum + model[j][k].hmm1(obsBird);
                }
                // take the mean of all hmms from that specie
                probs[j]=sum/(double)model[j].size();
            }

            //get the maximum of the probabilities and try to guess that specie
            auto it= std::max_element(probs.begin(),probs.end());
            int specie = std::distance(probs.begin(),it);
            
            // if we suspect it is a Black_stork we don't shoot in all the round
            if(specie==5){
                tuple<int,double>aux((get<0>(nextMove)),0);
                nextMove=aux;
                birdAlive[i] = false;
            }

            probShootBird.push_back(nextMove);
            lastPrediction[i] = get<0>(nextMove);
        }

        // take final decision of shooting selecting the most probable bird to shoot at
        tuple<int,double> goalBird = probShootBird[0];
        int goalBirdNumber=0;
        for(int i=1; i<numberBirds; i++){
            if(get<1>(probShootBird[i]) > get<1>(goalBird) && lastShootingBird!=i && birdAlive[i]){
                goalBird=probShootBird[i];
                goalBirdNumber = i;        
            }
        }
        if(goalBirdNumber==0 && !birdAlive[0]){
            return cDontShoot;        
        }

        if(get<1>(goalBird) > confidenceShoot){
            cerr << "Shoot at bird " << goalBirdNumber << " with movement " << get<0>(goalBird) <<  "in timestep " << timestep << endl;
            lastShootingBird = goalBirdNumber;
            total++;
            return Action(goalBirdNumber,getNextMovement(get<0>(goalBird)));
        }
        else{
            cerr << "DONT shoot in timestep " << timestep << " round" << round << endl;
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

    // in the first round, just try to guess all of the birds randomly    
    if (pState.getRound() == 0){
        for(int i=0; i<pState.getNumBirds();i++){
            lGuesses[i] = SPECIES_RAVEN;
        }
    }
    
    // next rounds we try to guess which is the most likely specie for each bird
    else {
        for (int i=0; i<pState.getNumBirds();i++){
            Bird crB = pState.getBird(i);
            
            //get the observations of the bird
            std::vector<int> obsBird;
            for(int j=0;j<crB.getSeqLength();j++){
                if(crB.wasAlive(j)){
                    obsBird.push_back(crB.getObservation(j));
                }
            }
    
            //get the probability of this observation sequence with each of the hmm
            std::vector<double> probs(numSpecies);
            for(int j=0;j<numSpecies;j++){
                double sum=0;
                for(int k=0;k<model[j].size();k++){
                    sum = sum + model[j][k].hmm1(obsBird);
                }
                // take the mean of all hmms from that specie
                probs[j]=sum/(double)model[j].size();
            }

            //get the maximum of the probabilities and try to guess that specie
            auto it= std::max_element(probs.begin(),probs.end());
            int specie = std::distance(probs.begin(),it);
            
            lGuesses[i] = getSpecie(specie);
        }
    }
    

    // keep my last guess to calculate the ratio of correct answers
    lastGuess = lGuesses;

    cerr << "Ratio shooting: " << correct/(double)total << endl;     

    cerr << "MY GUESSES" << endl;
    for(int i=0; i<pState.getNumBirds(); i++){
        cerr << lGuesses[i] << " ";    
    }  
    cerr << endl;  
    
    return lGuesses;
}

void Player::hit(const GameState &pState, int pBird, const Deadline &pDue)
{
    /*
     * If you hit the bird you are trying to shoot, you will be notified through this function.
     */
    std::cerr << "HIT BIRD!!!" << std::endl;
    birdAlive[pBird] = false;
    correct++;
    
}

void Player::reveal(const GameState &pState, const std::vector<ESpecies> &pSpecies, const Deadline &pDue)
{
    /*
     * If you made any guesses, you will find out the true species of those birds in this function.
     */

    // get the statistics
    ratio = numequals(lastGuess,pSpecies, pSpecies.size()) / (double)pSpecies.size();
    cerr << "Ratio of guessing" << ratio << endl;

    cerr << "REAL BIRDS" << endl;
    for(int i=0; i<pState.getNumBirds(); i++){
        cerr << pSpecies[i] << " ";    
    } 
    cerr << endl;
    
    //train the models with all the birds from that specie
    for (int i=0; i<pSpecies.size() && pDue.remainingMs()>150; i++){
        ESpecies crS = pSpecies[i];
        if (crS != SPECIES_UNKNOWN){
            // create a new model for the bird
            HMM newmodel(numst,numobs);
            Bird crB = pState.getBird(i);
            
            // get the observations of the bird
            std::vector<int> obsBird;
            for(int j=0;j<crB.getSeqLength();j++){
                if(crB.wasAlive(j)){
                    obsBird.push_back(crB.getObservation(j));
                }
            }
            // train the model and keep it in the specie that corresponds
            newmodel.hmm3(obsBird);
            model[crS].push_back(newmodel);
        }
    }

}


} /*namespace ducks*/
