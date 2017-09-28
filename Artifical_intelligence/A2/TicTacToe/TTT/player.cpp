#include "player.hpp"
#include <cstdlib>
#include <vector>
#include <limits>

namespace TICTACTOE
{

int minimax(const GameState &pState, uint8_t Player)
{
    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);
    if(pState.isEOG() || lNextStates.size()==0){
        if(Player==CELL_X && pState.isXWin()){
            return 1;
        }
        else if(Player==CELL_O && pState.isOWin()){
            return 1;
        }
        // else if(pState.isDraw()){
        //     return 0;
        // }
        else{
            return -1;
        }
    }
    else 
    {
        std::cerr << "Calculating minimax \n";
        if (Player==CELL_O)
        {
            int bestPossible = -99999;
            for(int i=0;i<lNextStates.size();i++){
                int v=minimax(lNextStates[i],CELL_O);
                bestPossible=std::max(bestPossible,v);
                
            }
            return bestPossible;
        }
        else //Player='X'
        {
            int bestPossible = 99999;
            for(int i=0;i<lNextStates.size();i++){
                int v=minimax(lNextStates[i],CELL_X);
                bestPossible=std::min(bestPossible,v);
                
            }
            return bestPossible;
        }
        return 0;
    }

}
const int Heuristic_Array[5][5] = {
    {     0,   -10,  -100, -1000 , -100000000},
    {    10,     0,     0,     0 ,      0},
    {   100,     0,     0,     0 ,      0},
    {  1000,     0,     0,     0 ,      0},
    { 100000000,     0,     0,     0 ,      0}
};

int evaluatePosition(GameState pState, uint8_t player) {
    //uint8_t opponent = (player == CELL_O) ? CELL_O : CELL_X, piece;
    int poss[10][4]={
        {0,1,2,3},
        {4,5,6,7},
        {8,9,10,11},
        {12,13,14,15},
        {0,4,8,12},
        {1,5,9,13},
        {2,6,10,14},
        {3,7,11,15},
        {0,5,10,15},
        {3,6,9,12}
    }; 
    int players, others, t = 0, i, j;
    for (i = 0; i < 10; i++)  {
        players = others = 0;
        for (j = 0; j < 4; j++)  {
            if (pState.at(poss[i][j])&CELL_O){
                others++;
            }
            else if(pState.at(poss[i][j])&CELL_X){
                players++;
            }
        }
        t += Heuristic_Array[players][others];
    }
    return t;
}
int alphabeta(const GameState &pState,int depth,int alpha,int beta,uint8_t Player)
{
    int v=0;
    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);
    if(depth==0 || lNextStates.size()==0 || pState.isEOG()){
        v=evaluatePosition(pState,Player);

    }
    else if (Player==CELL_O)
    {
        v=-(std::numeric_limits<int>::max());
        for (int i=0;i<lNextStates.size();i++)
        {
            v=std::max(v,alphabeta(lNextStates[i],depth-1,alpha,beta,CELL_X));
            alpha=std::max(alpha,v);
            if (beta<=alpha){
                break; //Prune
            }

        }
    }
    else if (Player==CELL_X)
    {
        v=std::numeric_limits<int>::max();
        for (int i=0;i<lNextStates.size();i++)
        {
            v=std::min(v,alphabeta(lNextStates[i],depth-1,alpha,beta,CELL_O));
            beta=std::min(beta,v);
            if (beta<=alpha){
                break; //Prune
            }

        }
    }
    return v;
}
GameState Player::play(const GameState &pState,const Deadline &pDue)
{
    srand( time( NULL ) );
    uint8_t Player=pState.getNextPlayer();
    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);

    GameState move;
    int v=-std::numeric_limits<int>::max();
    int alpha=-std::numeric_limits<int>::max();
    int beta=std::numeric_limits<int>::max();
    int aux;
    if(!pState.isBOG()){
        for(int i=0;i<lNextStates.size();i++){
            aux=alphabeta(lNextStates[i],4,alpha,beta,Player);
            if(aux>v){
                move=lNextStates[i];
                v=aux;
            }

        }
    }
    else{
        move=lNextStates[rand() % lNextStates.size()];
    }
    //alphabeta(lNextStates[i],50,9999,9999,'X') minimax(lNextStates[i],'X')
    //if (lNextStates.size() == 0) return GameState(pState, Move());

    /*
     * Here you should write your clever algorithms to get the best next move, ie the best
     * next state. This skeleton returns a random move instead.
     */

    return move;
}



/*namespace TICTACTOE*/ }
