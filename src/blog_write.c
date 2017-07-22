#include "blog_common.h"

int blog_write_newentry(char *key, char *value)
{
	int rc;

	MDB_env *env;
	MDB_txn *txn;

	E(mdb_env_create(&env));
	E(mdb_env_open(env, DB_PATH, MDB_FIXEDMAP, S_IRWXU));

	E(mdb_txn_begin(env, NULL, 0, &txn));

	E(mdb_txn_commit(txn));

	mdb_env_close(env);
	return 0;
}

int main(int argc, char **argv)
{
	if (argc == 3)
		return blog_write_newentry(argv[1], argv[2]);
	return -1;
}
