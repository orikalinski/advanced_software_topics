#include <iostream>
#include "rps_game.h"

int main(int argc, char* argv[]) {

    if (argc <= 6) {
        auto *game_client = new rps_game(10, 10, 2, 5, 1, 2, 2, 1);
        int winner = game_client->playGame(argv[1], argv[2], argv[3], argv[4]);
    }

    return 0;
}
