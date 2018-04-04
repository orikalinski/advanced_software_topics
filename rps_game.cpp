//
// Created by ori on 27/03/18.
//

#include "rps_game.h"

rps_game::rps_game(int M, int N, int R, int P, int S, int B, int J, int F) : M{M}, N{N}, R{R}, P{P}, S{S}, B{B}, J{J},
                                                                             F{F} {}

int rps_game::getWinner() {
    int winner = -1;
    if (!player_1_board->isValidBoard() && !player_2_board->isValidBoard()) {
        winner = 0;
        std::cout << "Tie" << std::endl;
    }
    else if (!player_1_board->isValidBoard()) {
        winner = 2;
        std::cout << "Player 2 won" << std::endl;
    }
    else if (!player_2_board->isValidBoard()) {
        winner = 1;
        std::cout << "Player 1 won" << std::endl;
    }
    else
        std::cout << "Both boards are valid" << std::endl;
    return winner;
}

int rps_game::initializeBoard(const string &positions_file_path_1, const string &positions_file_path_2){
    player_1_board = new game_board(positions_file_path_1, M, N, R, P, S, B, J, F);
    player_2_board = new game_board(positions_file_path_2, M, N, R, P, S, B, J, F);
    return getWinner();
}

int rps_game::compareBetweenTwoPieces(board_piece board_piece_1, board_piece board_piece_2) {
    char piece_1 = board_piece_1.getCurrentPiece();
    char piece_2 = board_piece_2.getCurrentPiece();
    if (piece_1 == 'E' || piece_2 == 'E')
        return -1;
    else if (piece_1 == 'B' || piece_2 == 'B' || piece_1 == piece_2)
        return 0;
    else if (piece_2 == 'F')
        return 1;
    else if (piece_1 == 'F')
        return 2;
    else if ((piece_1 == 'R' && piece_2 == 'S') || (piece_1 == 'S' && piece_2 == 'P')
             || (piece_1 == 'P' && piece_2 == 'R'))
        return 1;
    else
        return 2;
}

void rps_game::handleElapsedCell(int X, int Y) {
    int cell_winner;
    board_piece player_1_cell = player_1_board->boardPiece(X, Y);
    board_piece player_2_cell = player_2_board->boardPiece(X, Y);
    cell_winner = compareBetweenTwoPieces(player_1_cell, player_2_cell);
    if (cell_winner == -1)
        return;
    else if (cell_winner == 0) {
        player_1_board->emptyCell(X, Y);
        player_2_board->emptyCell(X, Y);
    }
    else if (cell_winner == 1)
        player_2_board->emptyCell(X, Y);
    else if (cell_winner == 2)
        player_1_board->emptyCell(X, Y);
}

int rps_game::handleElapsedCells() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            handleElapsedCell(j, i);
        }
    }
}

string nextLine(std::ifstream &stream_file) {
    std::string line;

    while (std::getline(stream_file, line)) {
        if (!line.empty())
            break;
    }
    return line;
}

stringstream rps_game::getReasonRepr(int winner) {
    std::stringstream return_string;
    int player_1_reason = player_1_board->getReason();
    int player_2_reason = player_2_board->getReason();
    int looser = winner == 1 ? 2 : 1;
    int line_number = looser == 1 ? player_1_board->getLineNumber() : player_2_board->getLineNumber();
    if (winner == 0) {
        if (player_1_reason == player_2_reason) {
            if (player_1_reason == valid)
                return_string << "A tie - both Moves input files done without a winner";
            else if (player_1_reason == flags)
                return_string << "A tie - all flags are eaten by both players in the position files";
            else if (player_1_reason == bad_position)
                return_string << "Bad Positioning input file for both players - player 1: line " <<
                              player_1_board->getLineNumber() << ", player 2: line " << player_2_board->getLineNumber();
        }
    }
    else if (player_1_reason == flags || player_2_reason == flags)
        return_string << "All flags of the opponent are captured";
    else if (player_1_reason == moving_pieces || player_2_reason == moving_pieces)
        return_string << "All moving PIECEs of the opponent are eaten";
    else if (player_1_reason == bad_position || player_2_reason == bad_position)
        return_string << "Bad Positioning input file for player " << looser << " - line " << line_number;
    else if (player_1_reason == bad_move || player_2_reason == bad_move)
        return_string << "Bad Moves input file for player " << looser << " - line " << line_number;

    return return_string;
}

void rps_game::writeOutputFile(const string &output_file_path, int winner) {
    ofstream output_file(output_file_path);
    output_file << "Winner: " << winner << std::endl;
    output_file << "Reason: " << getReasonRepr(winner).str() << std::endl;
    output_file << std::endl;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            char player_1_piece = player_1_board->boardPiece(j, i).getPiece();
            char player_2_piece = player_2_board->boardPiece(j, i).getPiece();
            if (player_1_piece != 'E')
                output_file << player_1_piece;
            else if (player_2_piece != 'E')
                output_file << std::tolower(player_2_piece, std::locale());
            else
                output_file << ' ';
        }
        output_file << std::endl;
    }

}

int rps_game::playGame(const string &positions_file_path_1, const string &positions_file_path_2,
                       const string &movement_file_path_1, const string &movement_file_path_2,
                       const string &output_file_path) {
    int winner = initializeBoard(positions_file_path_1, positions_file_path_2);
    if (winner != -1)
        return winner;
    player_1_board->initializeLineNumber();
    player_2_board->initializeLineNumber();
    handleElapsedCells();
    player_1_board->updateBoardValidity();
    player_2_board->updateBoardValidity();
    winner = getWinner();
    if (winner != -1) {
        writeOutputFile(output_file_path, winner);
        return winner;
    }

    std::ifstream movement_file_1(movement_file_path_1);
    std::ifstream movement_file_2(movement_file_path_2);
    int to_X, to_Y, joker_X = -1, joker_Y = -1;
    char new_joker_rep = 'E';
    bool should_change_joker = false;
    bool is_valid;
    game_board *current_player_board = player_1_board;
    int current_player = 1;
    bool is_player_1_done = false, is_player_2_done = false;
    while (true) {
        string line = nextLine(current_player == 1 ? movement_file_1 : movement_file_2);
        if (!line.empty()) {
            is_valid = current_player_board->readPieceMovement(line, to_X, to_Y, joker_X, joker_Y, new_joker_rep,
                                                               should_change_joker);
            if (!is_valid) {
                winner = current_player == 1 ? 2 : 1;
                std::cout << "Player " << winner << " won" << std::endl;
                writeOutputFile(output_file_path, winner);
                return winner;
            }
            handleElapsedCell(to_X, to_Y);
            player_1_board->updateBoardValidity();
            player_2_board->updateBoardValidity();
            winner = getWinner();
            if (winner != -1) {
                writeOutputFile(output_file_path, winner);
                return winner;
            }

            if (should_change_joker)
                current_player_board->alterJoker(joker_X, joker_Y, new_joker_rep);

            if (!is_player_1_done && !is_player_2_done) {
                if (current_player == 1) {
                    current_player_board = player_2_board;
                    current_player = 2;
                } else {
                    current_player_board = player_1_board;
                    current_player = 1;
                }
            }
        }
        else {
            if (current_player == 1) {
                is_player_1_done = true;
                current_player_board = player_2_board;
                current_player = 2;
            }
            else {
                is_player_2_done = true;
                current_player_board = player_1_board;
                current_player = 1;
            }
            if (is_player_1_done && is_player_2_done)
                break;
        }
    }
    std::cout << "Tie" << std::endl;
    writeOutputFile(output_file_path, winner);
    return 0;

}
