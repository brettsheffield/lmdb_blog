/*
 * blog_write.c
 *
 * this file is part of lmdb_blog
 *
 * Copyright (c) 2017-2018 Brett Sheffield <brett@gladserv.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file COPYING in the distribution).
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "blog_common.h"

#define BUFSIZE 4096

int blog_write_newentry(char *database, char *k, char *v)
{
	int rc;

	MDB_env *env;
	MDB_txn *txn;
	MDB_dbi dbi;
	MDB_val key, data;
	MDB_stat mst;

	key.mv_size = strlen(k) + 1;
	key.mv_data = k;
	data.mv_size = strlen(v) + 1;
	data.mv_data = v;

	E(mdb_env_create(&env));
	if (database)
		E(mdb_env_set_maxdbs(env, DBCOUNT));
	E(mdb_env_open(env, DB_PATH, MDB_NOSUBDIR, S_IRWXU));
	E(mdb_txn_begin(env, NULL, 0, &txn));
	E(mdb_dbi_open(txn, database, MDB_DUPSORT|MDB_CREATE, &dbi));

	mdb_put(txn, dbi, &key, &data, 0);

	E(mdb_txn_commit(txn));
	E(mdb_env_stat(env, &mst));

	mdb_dbi_close(env, dbi);
	mdb_env_close(env);

	return 0;
}

int blog_write_read_stdin(char **data)
{
	char *buf = NULL;
	ssize_t bytes = 0;
	ssize_t len = 0;
	int bufsize = BUFSIZE;

	buf = malloc(bufsize);
	while ((bytes = read(STDIN_FILENO, buf + len, 1)) == 1) {
		len += bytes;
		if (len == bufsize) {
			bufsize += BUFSIZE;
			buf = realloc(buf, bufsize);
		}
	}
	buf[len - 1] = '\0';
	*data = buf;

	return 0;
}

int main(int argc, char **argv)
{
	char *database = NULL;
	char *data = NULL;
	int rc = 0;
	int opts = 0;

	if ((strcmp(argv[1], "--database") == 0) || (strcmp(argv[1], "-d") == 0)) {
		database = argv[2];
		opts += 2;
	}

	if (argc - opts == 2)
		blog_write_read_stdin(&data);
	else
	if (argc - opts == 3)
		data = argv[2 + opts];
	else
		return -1;

	rc = blog_write_newentry(database, argv[1 + opts], data);
	if (argc - opts == 2)
		free(data);

	return rc;
}
