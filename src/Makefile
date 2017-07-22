PROGRAM=blog
READER=${PROGRAM}_read
WRITER=${PROGRAM}_write
ALL=${READER} ${WRITER}
CC=gcc -Wall -Werror -g
LIBS=-llmdb

all: ${ALL}

${READER}: ${READER}.o
	${CC} -o ${READER} ${READER}.o ${LIBS}

${READER}.o: ${READER}.c
	${CC} -c ${READER}.c ${LIBS}

${WRITER}: ${WRITER}.o
	${CC} -o ${WRITER} ${WRITER}.o ${LIBS}

${WRITER}.o: ${WRITER}.c
	${CC} -c ${WRITER}.c ${LIBS}


.PHONY: clean


clean:
	rm -f *.o ${ALL}

