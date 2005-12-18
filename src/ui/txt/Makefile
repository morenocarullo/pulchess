INCLUDE_OBJ=../../../src/logic/
INCLUDE=../../../src/
CXXFLAGS= -g3 -O -I ${INCLUDE}

all: main
	@echo test: compilazione txtui

clean:
	@rm -f *.o

main: main.o
	g++ -g -o pulchess -fno-rtti main.o ${INCLUDE_OBJ}facade.o ${INCLUDE_OBJ}player.o ${INCLUDE_OBJ}humanplayer.o ${INCLUDE_OBJ}cpuplayer.o  ${INCLUDE_OBJ}board.o ${INCLUDE_OBJ}piece.o ${INCLUDE_OBJ}soldier.o ${INCLUDE_OBJ}move.o ${INCLUDE_OBJ}tower.o ${INCLUDE_OBJ}king.o ${INCLUDE_OBJ}knight.o ${INCLUDE_OBJ}bishop.o ${INCLUDE_OBJ}queen.o