#ifndef _TICTACTOE_PLAYER_HPP_
#define _TICTACTOE_PLAYER_HPP_

#include "constants.hpp"
#include "deadline.hpp"
#include "move.hpp"
#include "gamestate.hpp"
#include <vector>

namespace TICTACTOE
{

class Player
{
public:
    ///perform a move
    ///\param pState the current state of the board
    ///\param pDue time before which we must have returned
    ///\return the next state the board is in after our move
    
    int minimax(const GameState &pState, uint8_t Player);
    int alphabeta(const GameState &pState,int depth,int alpha,int beta,uint8_t Player);
    int evalBoard(const GameState &pState,uint8_t Player);
    GameState play(const GameState &pState, const Deadline &pDue);
};

/*namespace TICTACTOE*/ }

#endif
