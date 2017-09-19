#include "Player.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include "HMMDuck.hpp"

namespace ducks
{

Player::Player()
{
}

const int Ss = 9;   //number of states of each HMM (number of possible movements of the birds)
const int Os = 9;   //number of observations of each HMM (number of possible movements of the birds)

const double epsilon = 0.0001;
const int maxNumBirds = 20;


const double probToShoot = 1/(double)9;

Matrix auxAs(Ss,Ss,epsilon);  
std::vector<Matrix> As(maxNumBirds,auxAs);

Matrix auxBs(Ss,Os);
std::vector<Matrix> Bs(maxNumBirds,auxBs);

std::vector<std::vector<double>> pis(maxNumBirds);

// GUESSING
const int Sg = 5;
const int Og = 9;
const int numSpecies = 6;

Matrix auxAg(Sg,Sg,epsilon);  
std::vector<Matrix> Ag(numSpecies,auxAg);

Matrix auxBg(Sg,Og,epsilon);
std::vector<Matrix> Bg(numSpecies,auxBg);

std::vector<std::vector<double>> pig(numSpecies);

const double minConfidence=0.98;



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

Action Player::shoot(const GameState &pState, const Deadline &pDue)
{
    /*
     * Here you should write your clever algorithms to get the best action.
     * This skeleton never shoots.
     */

//    // Main information about the environment
//    int round = pState.getRound();
//    size_t numberBirds = pState.getNumBirds();

//    int timestep=(pState.getBird(0)).getSeqLength();

//    // First, we just wait without shooting until having some information about the birds

//    if (timestep < 90 && round == 0){
//        return cDontShoot;
//    }
//    
//    // Then we initialize the matrixes with the information we have
//    else if (timestep == 90 && round == 0){


//        //initialize B with random values
//        for (int i=0; i<numberBirds; i++){
//            B[i].shuffle();
//        }
//        // As the observations correspond to the states, we initialize B with the identity matrix
////        for (int i=0; i<numberBirds; i++){
////            B[i].identity();
////            for (int j=0; j<B[i].getn();j++){
////                for (int k=0; k<B[i].getm();k++){
////                    if(B[i].getelement(j,k)==0){
////                        B[i].addelement(epsilon,j,k);                    
////                    }                
////                }            
////            }
////            B[i].normalize();
////        }

//        // We initialize pi with random values
//        for (int i=0; i<numberBirds; i++){
//            double ep=0.005;
//            double average = 1/(double)Ss;
//            double fMax = average+ep;
//            double fMin = average-ep;
//            double sum=1;
//            for (int j=0;j<Ss-1;j++){
//                double f = (double)rand() / RAND_MAX;
//                double v= fMin + f * (fMax - fMin);
//                sum-=v;
//                pi[i].push_back(v);
//            }
//            pi[i].push_back(sum);
//        }
//        
//        std::vector<std::vector<EMovement>> obs(numberBirds);

//        // We initialize A after 90 observations to try to have a better estimation of probabilities        
//        for(int i=0; i<numberBirds; i++){
//            Bird crB=pState.getBird(i);
//            int nobs=crB.getSeqLength();
//            
//            for (int k=0;k<nobs;k++){
//                obs[i].push_back(crB.getObservation(k));    
//            }

////            cerr << "observations vector of bird: " << i << endl;            
////            for (int k=0;k<nobs;k++){
////                cerr << obs[i][k] << " ";    
////            }
//            cerr << endl;
//            int lastObs = getIndex(obs[i][0]);
//            int currentObs;
//            for (int k=1;k<nobs;k++){
//                 currentObs = getIndex(obs[i][k]);
//                 if(lastObs != -1){
//                    A[i].addelement(A[i].getelement(lastObs,currentObs)+1,lastObs,currentObs);
//                 }
//                 lastObs=currentObs;
//            }
//            A[i].normalize(); 
//            cerr <<"A matrix: " << A[i].print() << endl;
//            cerr << "B matrix: " << B[i].print() << endl;
//            cerr << "Pi vector: ";
//            for(int k=0;k<pi[i].size();k++){
//                cerr << pi[i][k] << " ";
//            }
//            cerr << endl;
//        }

//        return cDontShoot;
//    }
//    
//    else {

//        std::vector<std::tuple <double,int>> probShootBird(numberBirds);

//        std::vector<std::vector<int>> obsind(numberBirds);
//        vector<vector<double>> alpha(numberBirds);
//            
//        for(int i=0; i<numberBirds; i++){
//            Bird crB=pState.getBird(i);
//            int nobs=crB.getSeqLength();
//            for (int k=0;k<nobs;k++){
//                obsind[i].push_back(getIndex(crB.getObservation(k)));    
//            }
////            cerr << "antes de hmm3" << i << endl;

////            cerr <<"A matrix: " << A[i].print() << endl;
////            cerr << "B matrix: " << B[i].print() << endl;
////            cerr << "Pi vector: ";
////            for(int k=0;k<pi[i].size();k++){
////                cerr << pi[i][k] << " ";
////            }
////            cerr << endl;

//            // compute hmm3
//            tuple<Matrix,Matrix,std::vector<double>> model = hmm3(A[i],B[i],pi[i],obsind[i]);
//            A[i]=get<0>(model);
//            B[i]=get<1>(model);
//            pi[i]=get<2>(model);
//           
//            cerr <<"A matrix: " << A[i].print() << endl;
//            cerr << "B matrix: " << B[i].print() << endl;
//            cerr << "Pi vector: ";
//            for(int k=0;k<pi[i].size();k++){
//                cerr << pi[i][k] << " ";
//            }
//            cerr << endl;

//            
//            cerr << "despues de hmm3" << i << endl;
//        
//            //compute HMM1, the alphas
//            Matrix aux = get<0> (alphapass(A[i],B[i],pi[i],obsind[i], obsind[i].size()));
//            alpha[i] = aux.getvector(aux.getn()-1);


//            cerr << "alpha vector: ";
//            for(int k=0;k<alpha[i].size();k++){
//                cerr << alpha[i][k] << " ";
//            }
//            cerr << endl; 
//             cerr << "despues de hmm1" << i << endl;

//            // We take the decision (to shoot or not to shoot)
//        
//            //calculate the sum of alphas and the state with more probability
//            double sum=0;
//            int maxi=0;
//            for(int j=0;j<alpha[i].size();j++){
//                sum+=alpha[i][j];
//                if(alpha[i][j]>alpha[i][maxi]){
//                    maxi = j;                
//                }
//            }
////            cerr << "max prob" << alpha[i][maxi] << "sum "<< sum << endl;
//            //decision
//            if(alpha[i][maxi]>(probToShoot*sum)){
//                std::tuple<double,int> aux(alpha[i][maxi],maxi);
//                probShootBird[i]=aux;           
//            }
//            else {
//                std::tuple<double,int> aux(0,-1);
//                probShootBird[i]=aux;                
//            }
//        }
//        
//        //final decision, if some of the birds has probToShoot>0 we shoot at the maximum values
//        std::tuple<double,int> goalBird(0,-1);
//        int goalBirdNumber = 0;
//        for(int i=0; i<numberBirds; i++){
//            if(get<0>(probShootBird[i]) > get<0>(goalBird)){
//                goalBird=probShootBird[i];
//                goalBirdNumber = i;            
//            }
//        }
//        
//        if((get<0>(goalBird)) > 0){
//            cerr << "we shoot in timestep " << timestep << " round" << round << endl;
//            return Action(goalBirdNumber,getNextMovement(get<1>(goalBird)));
//        }
//        else{
//            cerr << "we dont shoot in timestep " << timestep << " round" << round << endl;
//            return cDontShoot;        
//        }
//       
//    }

    // This line choose not to shoot
    return cDontShoot;
}

std::vector<ESpecies> Player::guess(const GameState &pState, const Deadline &pDue)
{
    /*
     * Here you should write your clever algorithms to guess the species of each bird.
     * This skeleton makes no guesses, better safe than sorry!
     */

    int round = pState.getRound();
    size_t numberBirds = pState.getNumBirds();

    std::vector<std::vector<int>> obs(numberBirds); // vector to keep all the observations made until this timestep
                                                // we keep the movements as integers to use them as indexes for the matrixes
    // get the new observations and keep them
    for(int k=0; k<numberBirds; k++){
        Bird crB=pState.getBird(k);
        for (int j=0;j<crB.getSeqLength();j++){
            obs[k].push_back(crB.getObservation(j));    
        }
    }


    std::vector<ESpecies> lGuesses(numberBirds, SPECIES_UNKNOWN);

    int timestep=(pState.getBird(0)).getSeqLength();

    if (round <= 0){
        // try to guess all the species of the birds (try with pigeon)
        for(int i=0; i<numberBirds; i++){
            int guess = 0;
            lGuesses[i] = getSpecie(guess);        
        }  
    }
    else {
//        cerr << "round" << round << endl;
//        for(int i=0; i<numSpecies; i++){
//            cerr << "A" << i << endl;
//            cerr << Ag[i].print() << endl;
//            cerr << "B" << i << endl;
//            cerr << Bg[i].print() << endl;
//            cerr << "pi" << i << endl;
//            for (int j=0; j<Sg; j++){
//                cerr << pig[i][j] << "  ";
//            }
//            cerr << endl;
//        }

        // try to guess the specie of each bird
        for (int j=0; j<numberBirds; j++){
            std::vector<double> alphaBird (numSpecies);
            for (int i=0; i<numSpecies; i++){
                // we calculate the probability of his observation sequence with each hmm from each specie
                alphaBird[i]= calcAlpha(Ag[i],Bg[i],pig[i],obs[j],timestep);
            }
            
//            cerr << "SEE my obs" << j << endl;
//            for (int i=0; i<timestep; i++){
//                cerr << obs[j][i] << " ";
//            }
//            cerr << endl;

            cerr << "SEE my alphas" << j << endl;
            for (int i=0; i<numSpecies; i++){
                cerr << alphaBird[i] << " ";
            }
            cerr << endl;

            // get the most likely specie the bird can be from and its confidence
            double confidence=alphaBird[0];
            int guessedSpecie=0;
           
            for (int i=1; i<numSpecies; i++){
                if(alphaBird[i]>confidence){
                    confidence = alphaBird[i];
                    guessedSpecie = i;                
                }
            }

            //e^otherconf - confidence is the probability of each specie given guessedSpecie
            vector<double> otherconf(numSpecies);
            double sum = 0;
            cerr << "sum" << endl;
            for (int i=0; i<numSpecies; i++){
                otherconf[i] = exp(alphaBird[i] - confidence); 
                sum = sum + otherconf[i]; 
                cerr << "paso i: " << sum << endl;        
            }
            
            //scale the most likely one (it is a logaritmic probability)
            double logsumpi= log(sum) + confidence;
            confidence = exp(confidence - logsumpi);
            cerr << "my confidence: " << confidence << endl;
           
//            auto max = std::max_element(alphaBird.begin(), alphaBird.end());
//            double confidence = *max;
//            int guessedSpecie = std::distance(alphaBird.begin(),max);

            //we decide either to guess the specie or not depending on the confidence we have
            if (confidence >= minConfidence){
                lGuesses[j] = getSpecie(guessedSpecie);
            }
            else {
                lGuesses[j] = SPECIES_UNKNOWN;           
            }
        }
    }

    cerr << "MYGUESSES" << endl;

    for(int i=0; i<numberBirds; i++){
        cerr << lGuesses[i] << "  " << endl;       
    }

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
    int round = pState.getRound();
    size_t numberBirds = pState.getNumBirds();

   

    cerr << "Pescepcies" << endl;
    for (int i=0; i<pSpecies.size();i++){
        cerr << pSpecies[i] << endl;
    }


    if (round <= 0){
        //initialize the matrixes A,B,pi with the information from the first round or random (pSpecies and movements)
        for (int i=0; i<numSpecies; i++){
            // A
            Ag[i].shuffle();

            // B
            Bg[i].shuffle();
            
            // pi
            double average = 1/(double)Sg;
            double ep=0.05*average; 
            double fMax = average+ep;
            double fMin = average-ep;
            double sum=1;
            for (int j=0;j<Sg-1;j++){
                double f = (double)rand() / RAND_MAX;
                double v= fMin + f * (fMax - fMin);
                sum-=v;
                pig[i].push_back(v);
            }
            pig[i].push_back(sum);
        
        }
        
    }
    
        // get the new observations and keep them
        for(int k=0; k<numberBirds; k++){
            Bird crB=pState.getBird(k);
            for (int j=0;j<crB.getSeqLength();j++){
                obs[k].push_back(crB.getObservation(j));    
            }
        }

        // train the matrixes with the whole observation sequence of one the birds that correspond to an specie (if all of them deadline comes)
        for (int i=0; i<numSpecies; i++){
            bool trained = false;
            for (int j=0; j<numberBirds && not trained; j++){
                if (pSpecies[j] == i) {
                    std::tuple<Matrix,Matrix,std::vector<double>> model = hmm3(Ag[i],Bg[i],pig[i],obs[j],(pState.getBird(j)).getSeqLength());
                    Ag[i]=get<0>(model);
                    Bg[i]=get<1>(model);
                    pig[i]=get<2>(model);
                    Ag[i].avoidzeros();
                    Bg[i].avoidzeros();
                    trained = true;
                }
            }
        }
    
}


} /*namespace ducks*/
