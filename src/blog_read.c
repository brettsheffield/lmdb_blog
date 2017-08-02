/*
 * blog_read.c
 *
 * this file is part of lmdb_blog
 *
 * Copyright (c) 2017 Brett Sheffield <brett@gladserv.com>
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

int blog_read_getentry(char *k, char **v)
{
	int rc;

	MDB_env *env;
	MDB_txn *txn;
	MDB_dbi dbi;
	MDB_val key, data;

	key.mv_size = strlen(k) + 1;
	key.mv_data = k;

	E(mdb_env_create(&env));
	E(mdb_env_open(env, DB_PATH, MDB_NOSUBDIR, S_IRWXU));
	E(mdb_txn_begin(env, NULL, MDB_RDONLY, &txn));
	E(mdb_dbi_open(txn, NULL, 0, &dbi));

	E(mdb_get(txn, dbi, &key, &data));
	mdb_txn_abort(txn);

	*v = strdup(data.mv_data);

	mdb_dbi_close(env, dbi);
	mdb_env_close(env);
	return 0;
}

int main(int argc, char **argv)
{
	char *data = NULL;
	if (argc == 2) {
		blog_read_getentry(argv[1], &data);
		puts(data);
		free(data);
		return 0;
	}

	return -1;
}
