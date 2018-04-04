#include <iostream>
#include "rps_game.h"

inline bool file_exists (const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}

int main(int argc, char* argv[]) {

    string player_1_board_file_path = "./player1.rps_board";
    string player_2_board_file_path = "./player2.rps_board";
    string player_1_moves_file_path = "./player1.rps_moves";
    string player_2_moves_file_path = "./player2.rps_moves";

    if (!file_exists(player_1_board_file_path) || !file_exists(player_2_board_file_path)
        || !file_exists(player_1_moves_file_path) || !file_exists(player_2_moves_file_path))
        std::cout << "Please make sure that the files are in the working directory";
    else {
        auto *game_client = new rps_game(10, 10, 2, 5, 1, 2, 2, 1);
        game_client->playGame(player_1_board_file_path, player_2_board_file_path, player_1_moves_file_path,
                              player_2_moves_file_path, "rps.output");
    }

    return 0;
}
