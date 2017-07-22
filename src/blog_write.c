#include "blog_common.h"

int blog_write_newentry(char *k, char *v)
{
	int rc;

	MDB_env *env;
	MDB_txn *txn;
	MDB_dbi dbi;
	MDB_val key, data;
	MDB_stat mst;

	key.mv_size = strlen(k) + 1;
	key.mv_data = k;
	data.mv_size = sizeof(v);
	data.mv_data = v;

	E(mdb_env_create(&env));
	E(mdb_env_open(env, DB_PATH, 0, S_IRWXU));
	E(mdb_txn_begin(env, NULL, 0, &txn));
	E(mdb_dbi_open(txn, NULL, 0, &dbi));

	mdb_put(txn, dbi, &key, &data, 0);

	E(mdb_txn_commit(txn));
	E(mdb_env_stat(env, &mst));

	mdb_dbi_close(env, dbi);
	mdb_env_close(env);

	return 0;
}

int main(int argc, char **argv)
{
	printf("writing '%s' to key '%s'\n", argv[2], argv[1]);
	if (argc == 3)
		return blog_write_newentry(argv[1], argv[2]);
	return -1;
}
