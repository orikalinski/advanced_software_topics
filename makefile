COMP = g++

OBJS = rps_game.cpp game_board.cpp board_piece.cpp rps_game.h game_board.h board_piece.h main.cpp

EXEC = ex1
CPP_COMP_FLAG = -std=c++11 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(CPP_COMP_FLAG) $(OBJS) -o $@

main.o: main.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
	rm -f $(OBJS) $(EXEC)
