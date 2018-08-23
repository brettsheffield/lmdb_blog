/*
 * blog_dump.c
 *
 * this file is part of lmdb_blog
 *
 * Copyright (c) 2018 Brett Sheffield <brett@gladserv.com>
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

int blog_dump_all()
{
	int rc;
	MDB_env *env;
	MDB_txn *txn;
	MDB_dbi dbi;
	MDB_val key, data;
	MDB_cursor *cursor;
	MDB_cursor_op op;
	FILE *f = NULL;
	char *filename;

	E(mdb_env_create(&env));
	E(mdb_env_open(env, DB_PATH, MDB_NOSUBDIR, S_IRWXU));
	E(mdb_txn_begin(env, NULL, MDB_RDONLY, &txn));
	E(mdb_dbi_open(txn, NULL, 0, &dbi));

	/* cycle through all entries and write to files */
	E(mdb_cursor_open(txn, dbi, &cursor));
	for (op = MDB_FIRST; (rc = mdb_cursor_get(cursor, &key, &data, op)) == 0; op = MDB_NEXT) {

		filename = malloc(key.mv_size + 1);
		memcpy(filename, key.mv_data, key.mv_size);
		filename[key.mv_size] = '\0';
		puts(filename);

		f = fopen(filename, "w");
		fwrite(data.mv_data, data.mv_size, 1, f);
		fclose(f);
		free(filename);
	}
	mdb_cursor_close(cursor);
	mdb_txn_abort(txn);
	mdb_dbi_close(env, dbi);
	mdb_env_close(env);

	return 0;
}

int main(int argc, char **argv)
{
	blog_dump_all();
	return 0;
}
