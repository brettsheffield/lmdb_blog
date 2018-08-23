/*
 * blog_import.c
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
#include <sys/types.h>
#include <dirent.h>

int blog_import_all()
{
	int rc = 0;
	DIR *d = NULL;
	FILE *f = NULL;
	struct dirent *dp;
	char *buf = NULL;
	long bufsize = 0;
	long l = 0;

	/* cycle through all files in current directory and import */
	d = opendir(".");
	while ((dp = readdir(d)) != NULL && rc == 0)
	{
		if (dp->d_type == DT_REG) {
			puts(dp->d_name);
			f = fopen(dp->d_name, "r");

			/* make sure our buffer is big enough */
			fseek(f, 0L, SEEK_END);
			l = ftell(f);
			fseek(f, 0L, SEEK_SET);
			if (l > bufsize) {
				bufsize = l;
				buf = realloc(buf, bufsize + 1);
			}

			fread(buf, bufsize, 1, f);
			fclose(f);
			buf[l + 1] = '\0';
			rc = blog_write_newentry(NULL, dp->d_name, buf);
		}
	}
	closedir(d);

	return 0;
}

int main(int argc, char **argv)
{
	blog_import_all();
	return 0;
}
