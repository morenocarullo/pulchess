#CXXFLAGS= -O2 -Wall -fno-rtti -I src/logic -I src/ #-DPULCHESS_NOTABLES
CXXFLAGS= -g3 -O2 -fno-rtti -I src/logic -I src/ -DDEBUG #-DPULCHESS_NOTABLES
LOGICPATH=src/logic/
TXTUIPATH=src/ui/txt/

PULCHESSLOGIC=${LOGICPATH}tower.o ${LOGICPATH}queen.o ${LOGICPATH}king.o ${LOGICPATH}soldier.o \
	${LOGICPATH}bishop.o ${LOGICPATH}piece.o ${LOGICPATH}board.o ${LOGICPATH}move.o \
	${LOGICPATH}cpuplayer.o ${LOGICPATH}timecontrol.o ${LOGICPATH}player.o ${LOGICPATH}knight.o \
	${LOGICPATH}humanplayer.o ${LOGICPATH}facade.o \
	${LOGICPATH}hashcache.o ${LOGICPATH}book.o \

FPULCHESSLOGIC=${LOGICPATH}logic.o

PULCHESSTXT=${TXTUIPATH}main.o

all: deploy
	@echo "Making pulchess..."

pulchess: ${PULCHESSTXT} ${PULCHESSLOGIC}
	@g++ ${CXXFLAGS} -o build/pulchess ${PULCHESSTXT} ${PULCHESSLOGIC}
	#strip pulchess
	
deploy: pulchess
	@cp -R data build

clean:
	@rm -f pulchess ${LOGICPATH}*.o ${TXTUIPATH}*.o ; \
	 rm -rf build/*

count:
	@echo "Total LOC (.cpp) : `find ./src | grep \.cpp$ | xargs cat | wc -l`"; \
	 echo "Total LOC (.H)   : `find ./src | grep \.H$ | xargs cat | wc -l`"; 
