#
# PROJECT: PulCHESS, a Computer Chess program
# AUTHOR:  Moreno Carullo - moreno.carullo [at] pulc [dot] it
# LICENSE: GPL, see license.txt in project root
# FILE:	   Project Makefile
# 
#*********************************************************************
# This program is free software; you can redistribute it and/or modify         
# it under the terms of the GNU General Public License as published by      
# the Free Software Foundation; either version 2 of the License, or         
# (at your option) any later version.                                       
#                                                                           
# This program is distributed in the hope that it will be useful,           
# but WITHOUT ANY WARRANTY; without even the implied warranty of            
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             
# GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          
# for more details.                                                         
#*********************************************************************
#
# Created on 20-giu-2006
# $Id$
#
CXXFLAGS= -g3 -O2 -fno-rtti -I src/logic -I src/ -Wall -DDEBUG #-DPULCHESS_NOTABLES
LOGICPATH=src/logic/
TXTUIPATH=src/ui/txt/
TESTSPATH=tests/logic/

PULCHESSLOGIC=${LOGICPATH}rook.o ${LOGICPATH}queen.o ${LOGICPATH}king.o ${LOGICPATH}pawn.o \
	${LOGICPATH}bishop.o ${LOGICPATH}piece.o ${LOGICPATH}board.o ${LOGICPATH}move.o \
	${LOGICPATH}cpuplayer.o ${LOGICPATH}timecontrol.o ${LOGICPATH}player.o ${LOGICPATH}knight.o \
	${LOGICPATH}humanplayer.o ${LOGICPATH}pulchess.o \
	${LOGICPATH}hashcache.o ${LOGICPATH}book.o \

PULCHESSTXT=${TXTUIPATH}main.o ${TXTUIPATH}xboard.o

PULCHESSTEST=${TESTSPATH}board.o ${TESTSPATH}soldier.o ${TESTSPATH}king.o ${TESTSPATH}move.o \
			 ${TESTSPATH}rook.o ${TESTSPATH}facade.o ${TESTSPATH}main.o ${TESTSPATH}humanplayer.o

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
