#ifndef _ERR_
#define _ERR_

/* wypisuje informacje o blednym zakonczeniu funkcji systemowej 
i konczy dzialanie */
extern void syserr(int err, const char *fmt, ...);

#endif
