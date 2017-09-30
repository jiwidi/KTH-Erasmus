#include "player.hpp"
#include <cstdlib>
#include <limits>
#include <time.h>

namespace checkers
{

//saves the player who is playing now
uint8_t actual_color;
const int max_depth = 9;

int evaluatePosition(GameState pState, uint8_t player) {
    int Red = 0;
    int White = 0;
    int total = 0;
    if (pState.isEOG()) {
        //if it is the end of game
            if (player == CELL_RED && pState.isRedWin()) {
                total = 10000;
            } else if (player == CELL_RED && pState.isWhiteWin()) {
                total = -10000;
            } else if (player == CELL_WHITE && pState.isRedWin()) {
                total = -10000;
            } else if (player == CELL_WHITE && pState.isWhiteWin()) {
                total = 10000;
            } 
            //it is a draw            
            else {
                total = 0;
            }
        return total;
    } 

    else {
        // We count the number of normal pieces of each color and number of king pieces of each color giving a weight depending on that    
        for (int i=0; i<32; i++){
            if (pState.at(i) & CELL_WHITE){
                White++;
                if (pState.at(i) & CELL_KING){
                    White+=3;               
                }
                // We also give points to pieces that are close to become a king
                else {
                    if (pState.cellToRow(i)==2){
                        White++; 
                    }
                    else if (pState.cellToRow(i)==0 || pState.cellToRow(i)==1){
                        White+=2;                    
                    }            
                }
            }
            else if (pState.at(i) & CELL_RED){
                Red++;
                if(pState.at(i) & CELL_KING){
                    Red+=3;                
                }
                else {
                    if (pState.cellToRow(i)==5){
                        Red++; 
                    }
                    else if (pState.cellToRow(i)==7 || pState.cellToRow(i)==6){
                        Red+=2;                    
                    }            
                }
            }

        }

        if (player == CELL_RED) {
            total += Red;
            total -= White;
        }
        else {
            total -= Red;
            total += White;
        }
        
        return total;
    }
}

int alphabeta(const GameState &pState,int depth,int alpha,int beta,bool maxplayer)
{
    int v=0;
    std::vector<GameState> lNextStates;
    
    if(depth<=0 || pState.isEOG()){
        v=evaluatePosition(pState,actual_color);

    }
    else if (maxplayer)
    {
        pState.findPossibleMoves(lNextStates);
        v=-(std::numeric_limits<int>::max());
        for (int i=0;i<lNextStates.size();i++)
        {
            int score=alphabeta(lNextStates[i],depth-1,alpha,beta,false);
            v=std::max(v,score);
            alpha=std::max(alpha,v);
            if (beta<=alpha){
                break; //Prune
            }

        }
    }
    else if (!maxplayer)
    {
        pState.findPossibleMoves(lNextStates);
        v=std::numeric_limits<int>::max();
        for (int i=0;i<lNextStates.size();i++)
        {
            int score=alphabeta(lNextStates[i],depth-1,alpha,beta,true);
            v=std::min(v,score);
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

    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);

    // Must play "pass" move if there are no other moves possible.
    if (lNextStates.size() == 0) return GameState(pState, Move());
        
    actual_color = pState.getNextPlayer();

    srand(time(NULL));

    GameState move;
    int v=-std::numeric_limits<int>::max();
    int alpha=-std::numeric_limits<int>::max();
    int beta=std::numeric_limits<int>::max();
    int aux;
    if(!pState.isBOG()){
        for(int j=0;j<=max_depth && pDue.getSeconds()-pDue.now().getSeconds() > 0.58; j++){
            std::cerr << "getseconds" << pDue.getSeconds() << std::endl;
            std::cerr << "now" << pDue.now().getSeconds() << std::endl;
            for(int i=0;i<lNextStates.size();i++){
                aux= alphabeta(lNextStates[i],j,alpha,beta,false);
                if(aux>v){
                    move=lNextStates[i];
                    v=aux;
                }
                else if(aux==v){
                    if(rand()%2==0){
                        move=lNextStates[i];
                        v=aux;
                    }            
                }

            }
        }
    }
    else{
        move=lNextStates[rand() % lNextStates.size()];
    }

    return move;
}

/*namespace checkers*/ }
