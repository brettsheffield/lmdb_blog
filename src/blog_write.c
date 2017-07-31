#include "blog_common.h"

#define BUFSIZE 4096

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
	data.mv_size = strlen(v) + 1;
	data.mv_data = v;

	E(mdb_env_create(&env));
	E(mdb_env_open(env, DB_PATH, MDB_NOSUBDIR, S_IRWXU));
	E(mdb_txn_begin(env, NULL, 0, &txn));
	E(mdb_dbi_open(txn, NULL, 0, &dbi));

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
	char *data = NULL;
	int rc = 0;

	if (argc == 2)
		blog_write_read_stdin(&data);
	else
	if (argc == 3)
		data = argv[2];
	else
		return -1;

	rc = blog_write_newentry(argv[1], data);
	if (argc == 2)
		free(data);

	return rc;
}
