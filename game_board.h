//
// Created by ori on 29/03/18.
//

#ifndef ADVANCED_SOFTWARE_TOPICS_GAME_BOARD_H
#define ADVANCED_SOFTWARE_TOPICS_GAME_BOARD_H


#include "board_piece.h"
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

using namespace std;

enum Reason {valid, flags, moving_pieces, bad_position, bad_move};
std::map<int, string> reason_to_string = {{valid, "Valid Board"}, {flags, "All flags of the opponent are captured"},
                                          {moving_pieces, "All moving PIECEs of the opponent are eaten"},
                                          {bad_position, "Bad Positioning input file - line: "},
                                          {bad_move, ""}};

typedef std::map<char, int> char_to_int_map;


class game_board {
    bool is_board_valid;
    int N, M;
    char_to_int_map mimic_counters;
    char_to_int_map counters;
    char_to_int_map max_per_type;

    board_piece **board;
    Reason reason = valid;
    int line_number = 0;

public:

    game_board(string file_path, int M, int N, int R, int P, int S, int B, int J, int F);

    void initializeBoard();

    bool validateCoordinates(int X, int Y);

    bool validateEmptyCell(int X, int Y);

    bool validateNumOfSameType(char piece);

    bool validateNumOfFlags();

    bool validatePieceExistence(char piece);

    bool validatePositionRow(int X, int Y, char piece);

    void insertRow(int X, int Y, char piece, char mimic_piece = 'E');

    bool readPiecesPositioning(string player_file_name);

    bool isValidBoard();

    bool validateMimicPieceExistence(char mimic_piece);

    bool checkZeroFlags();

    bool checkZeroMovingPieces();

    board_piece boardPiece(int X, int Y);

    void emptyCell(int X, int Y);

    bool updateBoardValidity();

    bool validateMovementRow(int from_X, int from_Y, int to_X, int to_Y);

    bool validateMovingPiece(int X, int Y);

    void moveRow(int from_X, int from_Y, int to_X, int to_Y);

    bool validateJokerExistence(int X, int Y);

    void alterJoker(int X, int Y, char new_rep);

    void alterBoardPiece(int X, int Y, char piece, char mimic = 'E');

    bool validateJokerCharacter(char joker);

    bool validateJokerAlter(char joker, int X, int Y, char new_rep);

    bool validateJokerPositioning(char joker, char mimic_piece);

    int getTotalCount(char piece);

    bool validateCoordinatesDiff(int from_X, int from_Y, int to_X, int to_Y);

    bool readPieceMovement(string line, int &to_X, int &to_Y, int &joker_X, int &joker_Y, char &new_rep,
                           bool &should_change_joker);

    void initializeLineNumber();

    string getReason();
};


#endif //ADVANCED_SOFTWARE_TOPICS_GAME_BOARD_H
