#include "player.hpp"
#include <cstdlib>
#include <vector>
#include <limits>
#define Possible_Wins 40
namespace TICTACTOE3D
{
const int poss[76][4]={ //1st
        {0,1,2,3},
        {4,5,6,7},
        {8,9,10,11},
        {12,13,14,15},
        {0,4,8,12},
        {1,5,9,13},
        {2,6,10,14},
        {3,7,11,15},
        {0,5,10,15},
        {3,6,9,12},

        {16,17,18,19},//2nd
        {20,21,22,23},
        {24,25,26,27},
        {28,29,30,31},
        {16,20,24,28},
        {17,21,25,29},
        {18,22,26,30},
        {19,23,27,31},
        {16,21,26,31},
        {19,22,25,28},

        {32,33,34,35},//3rd
        {36,37,38,39},
        {40,41,42,43},
        {44,45,46,47},
        {32,36,40,44},
        {33,37,41,45},
        {34,38,42,46},
        {35,39,43,47},
        {32,37,42,47},
        {35,38,41,44},

        {48,49,50,51},//4th
        {52,53,54,55},
        {56,57,58,59},
        {60,61,62,63},
        {48,52,56,60},
        {49,53,57,61},
        {50,54,58,62},
        {51,55,59,63},
        {48,53,58,63},
        {51,54,57,60},

        {0,16,32,48},
        {4,20,36,52},
        {8,24,40,46},
        {12,28,44,50},

        {1,17,33,49},
        {5,21,37,53},
        {9,25,41,57},
        {13,29,45,61},

        {2,18,34,50},
        {6,22,38,54},
        {10,26,42,58},
        {14,30,46,62},
        
        {3,19,35,51},
        {7,23,39,55},
        {11,27,43,59},
        {15,31,47,63},

        {0,20,40,60},
        {12,24,36,48},

        {1,21,41,61},
        {13,25,37,49},

        {2,22,42,62},
        {14,26,38,50},
        
        {3,23,42,63},
        {15,27,39,51},

        //4 diagonales
        {0,21,42,63},
        {12,25,38,51},
        {3,22,41,60},
        {15,26,37,48},

        {0,17,34,51},
        {4,21,38,55},
        {8,25,42,59},
        {12,29,46,63},

        {3,18,33,48},
        {7,22,37,52},
        {11,26,41,56},
        {15,30,45,60},

};

const int Heuristic_Array[5][5] = {
        {     0,   -10,  -100, -1000 , -100000000},
        {    10,     0,     0,     0 ,      0},
        {   100,     0,     0,     0 ,      0},
        {  1000,     0,     0,     0 ,      0},
        { 100000000,     0,     0,     0 ,      0}
};
      
int evaluatePosition(GameState pState, uint8_t player) {
    //uint8_t opponent = (player == CELL_O) ? CELL_O : CELL_X, piece;
    return 1;
    int players, others, t = 0, i, j;
    for (i = 0; i < 76; i++)  {
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
    //std::cerr << "Processing " << pState.toMessage() << std::endl;

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
            aux=alphabeta(lNextStates[i],1,alpha,beta,Player);
            //aux=evaluatePosition(lNextStates[i],Player);
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

/*namespace TICTACTOE3D*/ }
