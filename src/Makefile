# 
# this file is part of lmdb_blog
# 
# Copyright (c) 2017-2018 Brett Sheffield <brett@gladserv.com>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program (see the file COPYING in the distribution).
# If not, see <http://www.gnu.org/licenses/>.

PROGRAM=blog
READER=${PROGRAM}_read
WRITER=${PROGRAM}_write
DUMPER=${PROGRAM}_dump
IMPORTER=${PROGRAM}_import
ALL=${READER} ${WRITER} ${DUMPER} ${IMPORTER}
CC=gcc -Wall -Werror -g
LIBS=-llmdb
COMMON=${PROGRAM}_common

all: ${ALL}
	
blog_common.o: blog_common.h blog_common.c
	${CC} -c blog_common.c ${LIBS}

${READER}: ${READER}.o ${COMMON}.o
	${CC} -o ${READER} ${READER}.o ${LIBS}

${READER}.o: ${READER}.c ${COMMON}.h
	${CC} -c ${READER}.c ${LIBS}

${WRITER}: ${WRITER}.o ${COMMON}.o
	${CC} -o ${WRITER} ${WRITER}.o ${COMMON}.o ${LIBS}

${WRITER}.o: ${WRITER}.c ${COMMON}.h
	${CC} -c ${WRITER}.c ${LIBS}

${DUMPER}: ${DUMPER}.o ${COMMON}.o
	${CC} -o ${DUMPER} ${DUMPER}.o ${LIBS}

${DUMPER}.o: ${DUMPER}.c ${COMMON}.h
	${CC} -c ${DUMPER}.c ${LIBS}

${IMPORTER}: ${IMPORTER}.o ${COMMON}.o
	${CC} -o ${IMPORTER} ${IMPORTER}.o ${COMMON}.o ${LIBS}

${IMPORTER}.o: ${IMPORTER}.c ${COMMON}.h
	${CC} -c ${IMPORTER}.c ${LIBS}

install: all
	cp ${ALL} /usr/local/bin/

.PHONY: clean

clean:
	rm -f *.o ${ALL}
