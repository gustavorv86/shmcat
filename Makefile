SHELL=/bin/bash

#### Source ####

BUILD=build
DIST=dist
OBJECTS=${BUILD}/shmcat.o
TARGET=${DIST}/shmcat

#### Compilation options ####

CC=gcc
CFLAGS=-ggdb -std=gnu11 -Wall -Wextra

#### Rules ####

all: start make_dirs ${TARGET} end

clean:
	@echo '----[ Makefile clean ]----'
	@if [[ -d ${BUILD} ]]; then rm -rf ${BUILD}; fi
	@if [[ -f ${DIST} ]]; then rm -rf ${DIST}; fi

start:
	@echo '----[ Makefile ${TARGET} ]----'

make_dirs:
	@mkdir -p ${BUILD}
	@mkdir -p ${DIST}

$(BUILD)/shmcat.o: shmcat.c
	${CC} -c shmcat.c -o ${BUILD}/shmcat.o

${TARGET}: ${OBJECTS}
	${CC} ${OBJECTS} -o ${TARGET}

end:
	@echo 'Done'
