int utmp_open(char *filename);
struct utmp *utmp_next(void);
void utmp_close();
