//
// Created by ori on 29/03/18.
//

#include "game_board.h"
#include <utility>
#include <iterator>


int valueFromMap(char_to_int_map m, char key) {
    auto count = m.find(key);
    if (count != m.end()) {
        return count->second;
    }
    return -1;
}

unsigned int countWordsInString(std::string const& str)
{
    std::stringstream stream(str);
    return static_cast<unsigned int>(std::distance(std::istream_iterator<std::string>(stream),
            std::istream_iterator<std::string>()));
}

game_board::game_board(string file_path, int M, int N, int R, int P, int S, int B, int J, int F) {
    this->M = M;
    this->N = N;
    this->counters = {{'R', 0}, {'P', 0}, {'S', 0}, {'B', 0}, {'J', 0}, {'F', 0}};
    this->mimic_counters = {{'R', 0}, {'P', 0}, {'S', 0}, {'B', 0}};
    this->max_per_type = {{'M', M}, {'N', N}, {'R', R}, {'P', P}, {'S', S}, {'B', B}, {'J', J}, {'F', F}};
    initializeBoard();
    this->is_board_valid = readPiecesPositioning(std::move(file_path));
}

void game_board::initializeLineNumber() {
    line_number = 0;
}

board_piece game_board::boardPiece(int X, int Y) {
    return board[Y][X];
}

void game_board::alterBoardPiece(int X, int Y, char piece, char mimic) {
    int old_mimic_piece_count = valueFromMap(mimic_counters, boardPiece(X, Y).getMimicPiece());
    int new_mimic_piece_count = valueFromMap(mimic_counters, mimic);

    if (old_mimic_piece_count != -1)
        mimic_counters[boardPiece(X, Y).getMimicPiece()] -= 1;

    if (new_mimic_piece_count != -1)
        mimic_counters[mimic] += 1;

    int old_piece_count = valueFromMap(counters, boardPiece(X, Y).getPiece());
    int new_piece_count = valueFromMap(counters, piece);

    if (old_piece_count != -1)
        counters[boardPiece(X, Y).getPiece()] -= 1;

    if (new_piece_count != -1)
        counters[piece] += 1;

    board[Y][X].setPiece(piece);
    board[Y][X].setMimicPiece(mimic);
}

void game_board::emptyCell(int X, int Y) {
    alterBoardPiece(X, Y, 'E', 'E');
}

void game_board::initializeBoard() {
    board = new board_piece*[N];
    for (int i = 0; i < N; i++) {
        board[i] = new board_piece[M];
        for (int j = 0; j < M; j++) {
            board[i][j].setPiece('E'); // initializing with E as empty char
        }
    }
}

bool game_board::validateCoordinates(int X, int Y) {
    if (X < 1 || X > M || Y < 1 || Y > N) {
        std::cout << "coordinates are out of range (" << X << "," << Y << ")" << std::endl;
        return false;
    }
    return true;
}

bool game_board::validateEmptyCell(int X, int Y) {
    if (boardPiece(X, Y).getPiece() != 'E') {
        std::cout << "coordinates: (" << X << "," << Y << ") are already taken" << std::endl;
        return false;
    }
    return true;
}

bool game_board::validateNumOfSameType(char piece) {
    int current_number_from_type = valueFromMap(counters, piece);
    int max_number_from_type = valueFromMap(max_per_type, piece);
    if (current_number_from_type >= max_number_from_type) {
        std::cout << "Too many of type " << piece << ", " << current_number_from_type + 1 << " found" <<
                  "(should be: <= " << max_number_from_type << ")" << std::endl;
        return false;
    }
    return true;
}

bool game_board::validateNumOfFlags() {
    int number_of_flags = valueFromMap(counters, 'F');
    int flags_limit = valueFromMap(max_per_type, 'F');
    if (number_of_flags != flags_limit) {
        std::cout << "Inaccurate number of flags. " << number_of_flags << " found" << "(should be: "
             << flags_limit << ")" << std::endl;
        return false;
    }
    return true;
}

bool game_board::validatePieceExistence(char piece) {
    if (counters.find(piece) == counters.end()) {
        std::cout << "Invalid piece " << piece << std::endl;
        return false;
    }
    return true;
}

bool game_board::validateMimicPieceExistence(char mimic_piece) {
    if (mimic_counters.find(mimic_piece) == mimic_counters.end()) {
        std::cout << "Invalid mimic_piece " << mimic_piece << std::endl;
        return false;
    }
    return true;
}

bool game_board::validateMovingPiece(int X, int Y) {
    board_piece b_piece = boardPiece(X, Y);
    char piece = b_piece.getCurrentPiece();
    if (piece == 'R' || piece == 'S' || piece == 'P')
        return true;
    std::cout << "Invalid movement piece " << piece << std::endl;
    return false;
}

bool game_board::validateJokerCharacter(char joker) {
    if (joker != 'J') {
        std::cout << "Expected J got " << joker << " instead" << std::endl;
        return false;
    }
    return true;
}

bool game_board::validateJokerExistence(int X, int Y) {
    char piece = boardPiece(X, Y).getPiece();
    if (piece != 'J') {
        std::cout << "Wrong joker piece " << piece << std::endl;
        return false;
    }
    return true;
}

bool game_board::validatePositionRow(int X, int Y, char piece) {
    return validatePieceExistence(piece) && validateCoordinates(X, Y) && validateEmptyCell(X, Y)
           && validateNumOfSameType(piece);
}

bool game_board::validateCoordinatesDiff(int from_X, int from_Y, int to_X, int to_Y) {
    int x_diff = abs(from_X - to_X);
    int y_diff = abs(from_Y - to_Y);
    if ((x_diff == 1 && y_diff == 0) || (x_diff == 0 and y_diff == 1)) {
        return true;
    }
    std::cout << "Wrong coordinates difference(x axis diff: " << x_diff << ", y axis diff: " << y_diff << ")" <<
              std::endl;
    return false;
}

bool game_board::validateMovementRow(int from_X, int from_Y, int to_X, int to_Y) {
    return validateCoordinates(from_X, from_Y) && validateCoordinates(to_X, to_Y)
           && validateEmptyCell(to_X, to_Y) && validateMovingPiece(from_X, from_Y)
           && validateCoordinatesDiff(from_X, from_Y, to_X, to_Y);
}

bool game_board::validateJokerAlter(char joker, int X, int Y, char new_rep) {
    return validateJokerCharacter(joker) && validateMimicPieceExistence(new_rep) && validateCoordinates(X, Y)
           && validateJokerExistence(X, Y);
}

bool game_board::validateJokerPositioning(char joker, char mimic_piece) {
    return validateJokerCharacter(joker) && validateMimicPieceExistence(mimic_piece);
}

void game_board::insertRow(int X, int Y, char piece, char mimic_piece) {
    alterBoardPiece(X, Y, piece, mimic_piece);
}

void game_board::moveRow(int from_X, int from_Y, int to_X, int to_Y) {
    board_piece from_cell = board[from_Y][from_X];
    alterBoardPiece(to_X, to_Y, from_cell.getPiece(), from_cell.getMimicPiece());
    emptyCell(from_X, from_Y);
}

void game_board::alterJoker(int X, int Y, char new_rep) {
    alterBoardPiece(X, Y, 'J', new_rep);
}

bool game_board::readPiecesPositioning(string player_file_name) {
    std::ifstream positioning_file(player_file_name);
    std::string line;

    while (std::getline(positioning_file, line)) {
        line_number += 1;
        std::istringstream iss(line);
        int num_of_tokens = countWordsInString(line);
        char piece = 'E';
        int X = -1, Y = -1;
        char mimic_piece = 'E';
        bool is_valid;
        if ((iss >> piece >> X >> Y >> mimic_piece) && num_of_tokens == 4) {
            is_valid = this->validatePositionRow(X, Y, piece);
            bool is_valid_joker = validateJokerPositioning(piece, mimic_piece);
            if (is_valid && is_valid_joker)
                this->insertRow(X, Y, piece, mimic_piece);
            else {
                reason = bad_position;
                return false;
            }
        }
        else {
            iss.clear();
            iss.str(line);
            if ((iss >> piece >> X >> Y) && num_of_tokens == 3) {
                is_valid = this->validatePositionRow(X, Y, piece);
                if (is_valid)
                    this->insertRow(X, Y, piece);
                else {
                    reason = bad_position;
                    return false;
                }
            } else {
                reason = bad_position;
                std::cout << "Invalid row format (" << line << ")" << std::endl;
                return false;
            }
        }
    }

    return validateNumOfFlags();
}

bool game_board::readPieceMovement(string line, int &to_X, int &to_Y, int &joker_X, int &joker_Y, char &new_rep,
                                   bool &should_change_joker) {
    line_number += 1;
    std::istringstream iss(line);
    int num_of_tokens = countWordsInString(line);
    int from_X = -1, from_Y = -1;
    char joker = 'E';
    bool is_valid;
    if ((iss >> from_X >> from_Y >> to_X >> to_Y >> joker >> joker_X >> joker_Y >> new_rep) && num_of_tokens == 8) {
        is_valid = this->validateMovementRow(from_X, from_Y, to_X, to_Y);
        should_change_joker = true;
        bool is_joker_valid = this->validateJokerAlter(joker, joker_X, joker_Y, new_rep);
        if (is_valid && is_joker_valid) {
            this->moveRow(from_X, from_Y, to_X, to_Y);
        }
        else {
            reason = bad_move;
            return false;
        }
    }
    else {
        iss.clear();
        iss.str(line);
        if ((iss >> from_X >> from_Y >> to_X >> to_Y) && num_of_tokens == 4) {
            is_valid = this->validateMovementRow(from_X, from_Y, to_X, to_Y);
            if (is_valid)
                this->moveRow(from_X, from_Y, to_X, to_Y);
            else {
                reason = bad_move;
                return false;
            }
        } else {
            reason = bad_move;
            std::cout << "Invalid row format (" << line << ")" << std::endl;
            return false;
        }
    }
    return true;
}

bool game_board::isValidBoard() {
    return is_board_valid;
}

bool game_board::checkZeroFlags() {
    if (!valueFromMap(counters, 'F')) {
        std::cout << "Zero flags left" << std::endl;
        reason = flags;
        return true;
    }
    return false;
}

int game_board::getTotalCount(char piece) {
    return valueFromMap(counters, piece) + valueFromMap(mimic_counters, piece);
}

bool game_board::checkZeroMovingPieces() {
    if (!getTotalCount('R') && !getTotalCount('P') && !getTotalCount('S')) {
        std::cout << "Zero moving pieces left" << std::endl;
        reason = moving_pieces;
        return true;
    }
    return false;
}

bool game_board::updateBoardValidity() {
    this->is_board_valid = !checkZeroFlags() && !checkZeroMovingPieces();
}

string game_board::getReason() {
    return reason;
}
