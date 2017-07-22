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
	E(mdb_env_open(env, DB_PATH, 0, S_IRWXU));
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
