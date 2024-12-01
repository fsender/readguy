/**
 * @file ctg_timelib.h
 * @author FriendshipEnder (Q:3253342798)
 * @brief 时间兼容库文件. 旨在替代原有的, 过时的 TimeLib.h 文件(用于AVR设备)
 *        本库依赖 GLibC 的 time.h 文件.
 *        本库兼容 TimeLib.h 文件
 *        为了确保名称无冲突, 使用名称空间命名避免函数名冲突
 * @version 1.0.0
 * @date 2024-11-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _CTG_TimeLib_H_FILE
#define _CTG_TimeLib_H_FILE

#define CTG_DEFAULT_LOCAL_TIMEZONE ("CST-8") //默认时区
#define CTG_DEFAULT_LOCAL_OFFSET   (8*3600)  //默认时区偏移值
#define CTG_USE_tmElements_t         //使用 tmElements_t (强兼容, 但是没什么用)

#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <lwip/apps/sntp.h> //settimeofday

namespace CTG_TimeLib {

extern time_t tOffset;
extern struct tm m;

/// @brief return the current local time as seconds since Jan 1 1970 
inline time_t now() {return time(NULL)+tOffset;}
/// @brief the hour for the given time
inline int hour(time_t t){gmtime_r(&t, &m);return m.tm_hour;}
/// @brief the hour now 
inline int hour(){return hour(now());}  
/// @brief the minute for the given time
inline int minute(time_t t){gmtime_r(&t, &m);return m.tm_min;}
/// @brief the minute now 
inline int minute(){return minute(now());}
/// @brief the second for the given time
inline int second(time_t t){gmtime_r(&t, &m);return m.tm_sec;}
/// @brief the second now 
inline int second(){return second(now());}
/// @brief the day for the given time
inline int day(time_t t){gmtime_r(&t, &m);return m.tm_mday;}
/// @brief the day now 
inline int day(){return day(now());}   
/// @brief the weekday for the given time  (Sunday is day 1) 
inline int weekday(time_t t){gmtime_r(&t, &m);return m.tm_wday+1;}
/// @brief the weekday now (Sunday is day 1) 
inline int weekday(){return weekday(now());}         
/// @brief the month for the given time (Jan is month 1)
inline int month(time_t t){gmtime_r(&t, &m);return m.tm_mon+1;}
/// @brief the month now  (Jan is month 1)
inline int month(){return month(now());} 
/// @brief the year for the given time
inline int year(time_t t){gmtime_r(&t, &m);return m.tm_year+1900;}
/// @brief the full four digit year: (2009, 2010 etc) 
inline int year(){return year(now());}  
/// @brief set the time-zone
/// @param z (string) timezone name (like 'CST-8', 'EST+5', etc)
/// @param offset (time_t) how many seconds offset (+8 == 8*3600)
inline void   setTimezone(const char *z, time_t offset){
  tOffset = offset;
  setenv("TZ", z, 1); //设置时区变量 (当前设置为北京时间)
  tzset();
}

/// @brief set the time (input UTC time,not local time)
/// @param t (time_t): correct UTC time
/// @param fit_zone_cn (bool): Set timezone to UTC+8 (Beijing)
inline void   setUTCTime(time_t t, bool fit_local = true){
  if(tOffset != CTG_DEFAULT_LOCAL_OFFSET && fit_local)
    setTimezone(CTG_DEFAULT_LOCAL_TIMEZONE, CTG_DEFAULT_LOCAL_OFFSET);
  timeval tm_now={t, 0};
  settimeofday(&tm_now,nullptr);// need #include <lwip/apps/sntp.h>
}

/// @brief set the time (input local time)
/// @param t (time_t): correct local time (UTC+8 = Beijing)
inline void   setTime(time_t t){
  if(tOffset != CTG_DEFAULT_LOCAL_OFFSET)
    setTimezone(CTG_DEFAULT_LOCAL_TIMEZONE, CTG_DEFAULT_LOCAL_OFFSET);
  timeval tm_now={t-CTG_DEFAULT_LOCAL_OFFSET, 0};
  settimeofday(&tm_now,nullptr);// need #include <lwip/apps/sntp.h>
}

#ifdef CTG_USE_tmElements_t
typedef enum {timeNotSet, timeNeedsSync, timeSet
}  timeStatus_t ;

typedef enum {
    dowInvalid, dowSunday, dowMonday, dowTuesday, dowWednesday, dowThursday, dowFriday, dowSaturday
} timeDayOfWeek_t;

typedef enum {
    tmSecond, tmMinute, tmHour, tmWday, tmDay,tmMonth, tmYear, tmNbrFields
} tmByteFields;
typedef struct  { 
  uint8_t Second; 
  uint8_t Minute; 
  uint8_t Hour; 
  uint8_t Wday;   // day of week, sunday is day 1
  uint8_t Day;
  uint8_t Month; 
  int16_t Year;   // offset from 1970; 
} tmElements_t;

#define  tmYearToCalendar(Y) ((Y) + 1970)  // full four digit year 
#define  CalendarYrToTm(Y)   ((Y) - 1970)
#define  tmYearToY2k(Y)      ((Y) - 30)    // offset is from 2000
#define  y2kYearToTm(Y)      ((Y) + 30)   

/* Useful Constants */
#define SECS_PER_MIN  ((time_t)(60UL))
#define SECS_PER_HOUR ((time_t)(3600UL))
#define SECS_PER_DAY  ((time_t)(SECS_PER_HOUR * 24UL))
#define DAYS_PER_WEEK ((time_t)(7UL))
#define SECS_PER_WEEK ((time_t)(SECS_PER_DAY * DAYS_PER_WEEK))
#define SECS_PER_YEAR ((time_t)(SECS_PER_DAY * 365UL)) // TODO: ought to handle leap years
#define SECS_YR_2000  ((time_t)(946684800UL)) // the time at the start of y2k
 
/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) ((_time_) % SECS_PER_MIN)  
#define numberOfMinutes(_time_) (((_time_) / SECS_PER_MIN) % SECS_PER_MIN) 
#define numberOfHours(_time_) (((_time_) % SECS_PER_DAY) / SECS_PER_HOUR)
#define dayOfWeek(_time_) ((((_time_) / SECS_PER_DAY + 4)  % DAYS_PER_WEEK)+1) // 1 = Sunday
#define elapsedDays(_time_) ((_time_) / SECS_PER_DAY)  // this is number of days since Jan 1 1970
#define elapsedSecsToday(_time_) ((_time_) % SECS_PER_DAY)   // the number of seconds since last midnight 
// The following macros are used in calculating alarms and assume the clock is set to a date later than Jan 1 1971
// Always set the correct time before settting alarms
#define previousMidnight(_time_) (((_time_) / SECS_PER_DAY) * SECS_PER_DAY)  // time at the start of the given day
#define nextMidnight(_time_) (previousMidnight(_time_)  + SECS_PER_DAY)   // time at the end of the given day 
#define elapsedSecsThisWeek(_time_) (elapsedSecsToday(_time_) +  ((dayOfWeek(_time_)-1) * SECS_PER_DAY))   // note that week starts on day 1
#define previousSunday(_time_) ((_time_) - elapsedSecsThisWeek(_time_))      // time at the start of the week for the given time
#define nextSunday(_time_) (previousSunday(_time_)+SECS_PER_WEEK)          // time at the end of the week for the given time


/* Useful Macros for converting elapsed time to a time_t */
#define minutesToTime_t ((M)) ( (M) * SECS_PER_MIN)  
#define hoursToTime_t   ((H)) ( (H) * SECS_PER_HOUR)  
#define daysToTime_t    ((D)) ( (D) * SECS_PER_DAY) // fixed on Jul 22 2011
#define weeksToTime_t   ((W)) ( (W) * SECS_PER_WEEK)   

/// @brief set the time (input local time)
/// @param hr,min,sec,day,month,yr: correct local time (UTC+8 = Beijing)
void setTime(int16_t hr,int16_t min,int16_t sec,int16_t day,int16_t month,int16_t yr);

/// @brief break time_t into elements
void breakTime(time_t time, tmElements_t &tm);

/// @brief convert time elements into time_t
time_t makeTime(const tmElements_t &tm);  

#endif // CTG_USE_tmElements_t
} //namespace
#endif // _CTG_TimeLib_H_FILE