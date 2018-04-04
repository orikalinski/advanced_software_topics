//
// Created by ori on 27/03/18.
//

#ifndef ADVANCED_SOFTWARE_TOPICS_RPS_GAME_H
#define ADVANCED_SOFTWARE_TOPICS_RPS_GAME_H

#include <sstream>
#include <string>
#include <fstream>
#include "game_board.h"

using namespace std;


class rps_game {

public:
    int M, N, R, P, S, B, J, F;
    game_board *player_1_board, *player_2_board;

    rps_game(int M, int N, int R, int P, int S, int B, int J, int F);

    int initializeBoard(const string &positions_file_path_1, const string &positions_file_path_2);

    int playGame(const string &positions_file_path_1, const string &movement_file_path_1,
                 const string &positions_file_path_2, const string &movement_file_path_2,
                 const string &output_file_path);

    void handleElapsedCell(int X, int Y);

    int handleElapsedCells();

    int compareBetweenTwoPieces(board_piece board_piece_1, board_piece board_piece_2);

    int getWinner();

    void writeOutputFile(const string &output_file_path, int winner);

    string getReasonRepr();

    stringstream getReasonRepr(int winner);
};


#endif //ADVANCED_SOFTWARE_TOPICS_RPS_GAME_H
