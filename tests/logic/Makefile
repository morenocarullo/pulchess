INCLUDE_OBJ=../../src/logic/
INCLUDE=../../src/
CXXFLAGS= -g -O -DDEBUG -I ${INCLUDE}

all: soldier
	@echo test: compilazione logic

clean:
	@rm -f *.o

soldier: soldier.o
	g++ -g -o soldier -fno-rtti soldier.o ${INCLUDE_OBJ}player.o ${INCLUDE_OBJ}cpuplayer.o  ${INCLUDE_OBJ}board.o ${INCLUDE_OBJ}piece.o ${INCLUDE_OBJ}soldier.o ${INCLUDE_OBJ}move.o ${INCLUDE_OBJ}tower.o ${INCLUDE_OBJ}king.o ${INCLUDE_OBJ}knight.o ${INCLUDE_OBJ}bishop.o ${INCLUDE_OBJ}queen.o

