CXXFLAGS= -g3 -O2 -fno-rtti -I src/logic -I src/ -Wall -DDEBUG #s-DPULCHESS_NOTABLES
LOGICPATH=src/logic/
TXTUIPATH=src/ui/txt/
TESTSPATH=tests/logic/

PULCHESSLOGIC=${LOGICPATH}rook.o ${LOGICPATH}queen.o ${LOGICPATH}king.o ${LOGICPATH}pawn.o \
	${LOGICPATH}bishop.o ${LOGICPATH}piece.o ${LOGICPATH}board.o ${LOGICPATH}move.o \
	${LOGICPATH}cpuplayer.o ${LOGICPATH}timecontrol.o ${LOGICPATH}player.o ${LOGICPATH}knight.o \
	${LOGICPATH}humanplayer.o ${LOGICPATH}pulchess.o \
	${LOGICPATH}hashcache.o ${LOGICPATH}book.o \

PULCHESSTXT=${TXTUIPATH}main.o ${TXTUIPATH}xboard.o

PULCHESSTEST=${TESTSPATH}soldier.o ${TESTSPATH}king.o \
			 ${TESTSPATH}rook.o ${TESTSPATH}facade.o ${TESTSPATH}main.o 

all: deploy
	@echo "Making pulchess..."

pulchess: ${PULCHESSTXT} ${PULCHESSLOGIC}
	@mkdir -p build
	@g++ ${CXXFLAGS} -o build/pulchess ${PULCHESSTXT} ${PULCHESSLOGIC}
	#strip pulchess
	
deploy: pulchess
		
run: deploy
	@build/pulchess
	
run-debug: deploy
	@gdb build/pulchess
	
# unit testing	
test: ${PULCHESSTEST} ${PULCHESSLOGIC}
	@g++ ${CXXFLAGS} -o build/autotest ${PULCHESSLOGIC} ${PULCHESSTEST}
	@build/autotest
	
clean:
	@rm -rf ${LOGICPATH}*.o ${TXTUIPATH}*.o ${TESTSPATH}*.o build/pulchess build/autotest build/data

count:
	@echo "Total LOC (.cpp) : `find ./src | grep \.cpp$ | xargs cat | wc -l`"; \
	 echo "Total LOC (.H)   : `find ./src | grep \.H$ | xargs cat | wc -l`"; 
