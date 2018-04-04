//
// Created by ori on 29/03/18.
//

#ifndef ADVANCED_SOFTWARE_TOPICS_BOARD_PIECE_H
#define ADVANCED_SOFTWARE_TOPICS_BOARD_PIECE_H


class board_piece {
    char piece, mimic_piece;

public:
    void setPiece(char piece);

    void setMimicPiece(char mimic_piece);

    char getPiece();

    char getMimicPiece();

    char getCurrentPiece();
};


#endif //ADVANCED_SOFTWARE_TOPICS_BOARD_PIECE_H
