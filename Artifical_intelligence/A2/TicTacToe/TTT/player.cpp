#include "player.hpp"
#include <cstdlib>

namespace TICTACTOE
{

bool Player::terminal(const GameState &pState){
    return true;
}
int Player::minimax(const GameState &pState, uint8_t Player)
{
    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);
    if(pState.isEOG()){
        if(Player=='X' && pState.isXWin()){
            return 1;
        }
        else if(Player=='O' && pState.isOWin()){
            return 1;
        }
        else if(pState.isDraw()){
            return 0;
        }
        else{
            return -1;
        }
    }
    //std::cerr << "Calculating minimax \n";
    else 
    {
        if (Player=='O')
        {
            int bestPossible = -99999;
            for(int i=0;i<lNextStates.size();i++){
                int v=minimax(lNextStates[i],'O');
                bestPossible=std::max(bestPossible,v);
                
            }
            return bestPossible;
        }
        else //Player='X'
        {
            int bestPossible = 99999;
            for(int i=0;i<lNextStates.size();i++){
                int v=minimax(lNextStates[i],'X');
                bestPossible=std::min(bestPossible,v);
                
            }
            return bestPossible;
        }
    }

}

int Player::evalBoard(const GameState &pState,uint8_t Player){
    int val;
    if(Player==CELL_X){
        for(int i=0;i<16;i++){
            if(pState.at(i)&CELL_X){
                val+=1;
            }
        }
        return val;

    }
    else if (Player==CELL_O){
        for(int i=0;i<16;i++){
            if(pState.at(i)&CELL_O){
                val+=1;
            }
        }
        return val;
    
    }
    else{
        std::cerr << "error de players";
        return 1;
    }
}
int Player::alphabeta(const GameState &pState,int depth,int alpha,int beta,uint8_t Player)
{
    
    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);
    if(depth==0 || pState.isEOG()){
        return evalBoard(pState,Player);
    }
    else if (Player==CELL_O)
    {
        int v=-99999;
        for (int i=0;i<lNextStates.size();i++)
        {
            v=std::max(v,alphabeta(lNextStates[i],depth-1,alpha,beta,CELL_O));
            alpha=std::max(alpha,v);
            if (beta<=alpha){
                break; //Prune
            }

        }
    }
    else
    {
        int v=99999;
        for (int i=0;i<lNextStates.size();i++)
        {
            v=std::min(v,alphabeta(lNextStates[i],depth-1,alpha,beta,CELL_X));
            beta=std::min(alpha,v);
            if (beta<=alpha){
                break; //Prune
            }

        }
    }
}
GameState Player::play(const GameState &pState,const Deadline &pDue)
{
    std::cerr << "Processing " << pState.toMessage() << std::endl;
    auto Player=pState.getNextPlayer();
    std::cerr << "Player: " << Player << "\n";
    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);
    GameState move=lNextStates[rand() % lNextStates.size()];
    int v=0;
    for(int i=0;i<lNextStates.size();i++){
        if(alphabeta(lNextStates[i],1000,-999999,999999,Player)>v){
            std::cerr << "evaluando";
            move=lNextStates[i];
            v=alphabeta(lNextStates[i],1000,-999999,999999,Player);
        }

    }
    //alphabeta(lNextStates[i],50,9999,9999,'X') minimax(lNextStates[i],'X')
    if (lNextStates.size() == 0) return GameState(pState, Move());

    /*
     * Here you should write your clever algorithms to get the best next move, ie the best
     * next state. This skeleton returns a random move instead.
     */

    return move;
}



/*namespace TICTACTOE*/ }
