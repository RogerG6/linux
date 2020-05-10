/* Date : 2020/4/6
   Func : 为提升who程序的性能，增加缓冲功能
 */
#include <stdio.h>
#include <unistd.h>
#include <utmp.h>
#include <sys/types.h>
#include <fcntl.h>

#define NRECS 16
#define UTSIZE (sizeof(struct utmp))
#define NULLUT ((struct utmp *)NULL)

static char utmpbuf[NRECS * UTSIZE];
static int fd_utmp = -1;
static int cur_rec;		// 
static int num_recs;

int utmp_open(char *filename)
{
	fd_utmp = open(filename, O_RDONLY);
	cur_rec = num_recs = 0;
	return fd_utmp;
}

struct utmp *utmp_next(void)
{
	struct utmp *recp;
	int utmp_reload(void);

	if (fd_utmp == -1)
		return NULLUT;

	if (cur_rec == num_recs && utmp_reload() == 0) 
		return NULLUT;

	recp = (struct utmp *)&utmpbuf[cur_rec * UTSIZE];
	cur_rec++;
	return recp;
}

int utmp_reload()
{
	int amt_read;

	amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE);
	num_recs = amt_read / UTSIZE;
	cur_rec = 0;
	return num_recs;
}

void utmp_close()
{
	if (fd_utmp != -1)
		close(fd_utmp);
}