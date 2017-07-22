#include "blog_common.h"

int blog_write_newentry(char *k, char *v)
{
	int rc;

	MDB_env *env;
	MDB_txn *txn;
	MDB_dbi dbi;
	MDB_val key, data;

	key.mv_size = sizeof(k);
	key.mv_data = k;
	data.mv_size = sizeof(v);
	data.mv_data = v;

	E(mdb_env_create(&env));
	E(mdb_env_open(env, DB_PATH, MDB_FIXEDMAP, S_IRWXU));
	E(mdb_txn_begin(env, NULL, 0, &txn));
	E(mdb_dbi_open(txn, NULL, 0, &dbi));

	mdb_put(txn, dbi, &key, &data, MDB_NOOVERWRITE);

	E(mdb_txn_commit(txn));
	mdb_dbi_close(env, dbi);
	mdb_env_close(env);
	return 0;
}

int main(int argc, char **argv)
{
	puts(argv[2]);
	if (argc == 3)
		return blog_write_newentry(argv[1], argv[2]);
	return -1;
}
