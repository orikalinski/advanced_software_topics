//
// Created by ori on 29/03/18.
//

#include "board_piece.h"

void board_piece::setPiece(char piece) {
    this->piece = piece;
}

void board_piece::setMimicPiece(char mimic_piece) {
    this->mimic_piece = mimic_piece;
}

char board_piece::getPiece() {
    return this->piece;
}

char board_piece::getMimicPiece() {
    return this->mimic_piece;
}

char board_piece::getCurrentPiece() {
    return this->getPiece() == 'J' ? this->getMimicPiece() : this->getPiece();
}


