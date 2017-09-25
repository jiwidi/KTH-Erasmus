#include "player.hpp"
#include <cstdlib>

namespace TICTACTOE
{

int minimax(const GameState &pState, char Player)
{
    std::cerr << "Calculating minimax \n";
    if (Player=='X')
    {
        int bestPossible = -99999;
        std::vector<GameState> lNextStates;
        pState.findPossibleMoves(lNextStates);
        for(int i=0;i<lNextStates.size();i++){
            if(lNextStates[i].isEOG()){
                if(lNextStates[i].isXWin()){
                    return +1;
                }
                else if(lNextStates[i].isOWin()){
                    return -1;
                }
                else return 0;
            }
            int v=minimax(lNextStates[i],'O');
            bestPossible=std::max(bestPossible,v);
        }
        return bestPossible;
    }
    else //Player='O'
    {
        int bestPossible = 99999;
        std::vector<GameState> lNextStates;
        pState.findPossibleMoves(lNextStates);
        for(int i=0;i<lNextStates.size();i++){
            if(lNextStates[i].isEOG()){
                if(lNextStates[i].isXWin()){
                    return -1;
                }
                else if(lNextStates[i].isOWin()){
                    return 1;
                }
                else return 0;
            }
            int v=minimax(lNextStates[i],'X');
            bestPossible=std::min(bestPossible,v);
        }
        return bestPossible;
    }
}   
GameState Player::play(const GameState &pState,const Deadline &pDue)
{
    //std::cerr << "Processing " << pState.toMessage() << std::endl;

    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);
    GameState move=lNextStates[rand() % lNextStates.size()];
    int v=0;
    for(int i=0;i<lNextStates.size();i++){
        if(minimax(lNextStates[i],'X')>v){
            move=lNextStates[i];
            v=minimax(lNextStates[i],'X');
        }

    }

    if (lNextStates.size() == 0) return GameState(pState, Move());

    /*
     * Here you should write your clever algorithms to get the best next move, ie the best
     * next state. This skeleton returns a random move instead.
     */

    return move;
}



/*namespace TICTACTOE*/ }
