#ifndef _SYS_TIME_H_
#define _SYS_TIME_H_

#include <common.h>

typedef uint32_t systime_t; /* kept in miliseconds */

typedef struct tm {
  int tm_sec;     /* seconds after the minute [0-60] */
  int tm_min;     /* minutes after the hour [0-59] */
  int tm_hour;    /* hours since midnight [0-23] */
  int tm_mday;    /* day of the month [1-31] */
  int tm_mon;     /* months since January [0-11] */
  int tm_year;    /* years since 1900 */
  int tm_wday;    /* days since Sunday [0-6] */
  int tm_yday;    /* days since January 1 [0-365] */
  int tm_isdst;   /* Daylight Savings Time flag */
  long tm_gmtoff; /* offset from UTC in seconds */
  char *tm_zone;  /* timezone abbreviation */
} tm_t;

typedef struct timeval {
  time_t tv_sec;       /* seconds */
  suseconds_t tv_usec; /* microseconds */
} timeval_t;

typedef struct timespec {
  time_t tv_sec; /* seconds */
  long tv_nsec;  /* and nanoseconds */
} timespec_t;

#define TIMEVAL(fp)                                                            \
  (timeval_t) {                                                                \
    .tv_sec = (long)((fp)*1000000L) / 1000000L,                                \
    .tv_usec = (long)((fp)*1000000L) % 1000000L                                \
  }

static inline timeval_t st2tv(systime_t st) {
  return (timeval_t){.tv_sec = st / 1000, .tv_usec = st % 1000};
}

static inline systime_t tv2st(timeval_t tv) {
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

/* Operations on timevals. */
static inline void timeval_clear(timeval_t *tvp) {
  *tvp = (timeval_t){.tv_sec = 0, .tv_usec = 0};
}

static inline bool timeval_isset(timeval_t *tvp) {
  return tvp->tv_sec || tvp->tv_usec;
}

#define timeval_cmp(tvp, uvp, cmp)                                             \
  (((tvp)->tv_sec == (uvp)->tv_sec) ? (((tvp)->tv_usec)cmp((uvp)->tv_usec))    \
                                    : (((tvp)->tv_sec)cmp((uvp)->tv_sec)))

static inline timeval_t timeval_add(timeval_t *tvp, timeval_t *uvp) {
  timeval_t res = {.tv_sec = tvp->tv_sec + uvp->tv_sec,
                   .tv_usec = tvp->tv_usec + uvp->tv_usec};
  if (res.tv_usec >= 1000000) {
    res.tv_sec++;
    res.tv_usec -= 1000000;
  }
  return res;
}

static inline timeval_t timeval_sub(timeval_t *tvp, timeval_t *uvp) {
  timeval_t res = {.tv_sec = tvp->tv_sec - uvp->tv_sec,
                   .tv_usec = tvp->tv_usec - uvp->tv_usec};
  if (res.tv_usec < 0) {
    res.tv_sec--;
    res.tv_usec += 1000000;
  }
  return res;
}

timeval_t get_uptime(void);

#endif /* !_SYS_TIME_H_ */
