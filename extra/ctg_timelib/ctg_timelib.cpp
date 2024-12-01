#include "ctg_timelib.hpp"

namespace CTG_TimeLib {
  
time_t tOffset = 0;
struct tm m;

#if (defined ( CTG_USE_tmElements_t ) || defined (CTG_USE_ALL_TimeLib_features))

void setTime(int16_t hr,int16_t min,int16_t sec,int16_t dy, int16_t mnth, int16_t yr){
 // year can be given as full four digit year or two digts (2010 or 10 for 2010);  
 //it is converted to years since 1970
  tmElements_t tem;
  if( yr >= 1900) yr -= 1970;
  else yr += 30;  
  tem.Year = yr;
  tem.Month = mnth;
  tem.Day = dy;
  tem.Wday = 0;
  tem.Hour = hr;
  tem.Minute = min;
  tem.Second = sec;
  setTime(makeTime(tem));
}

#define LEAP_YEAR(Y) ( ((1970+(Y))>0) && !((1970+(Y))%4) && ( ((1970+(Y))%100) || !((1970+(Y))%400) ) )
static const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31}; 

void breakTime(time_t timeInput, tmElements_t &tm){
// break the given time_t into time components
// this is a more compact version of the C library localtime function
// note that year is offset from 1970 !!!

  uint8_t year;
  uint8_t month, monthLength;
  uint32_t time;
  unsigned long days;
  // API starts months from 1, this array starts from 0
  time = (uint32_t)timeInput;
  tm.Second = time % 60;
  time /= 60; // now it is minutes
  tm.Minute = time % 60;
  time /= 60; // now it is hours
  tm.Hour = time % 24;
  time /= 24; // now it is days
  tm.Wday = ((time + 4) % 7) + 1;  // Sunday is day 1 
  
  year = 0;  
  days = 0;
  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  tm.Year = year; // year is offset from 1970 
  
  days -= LEAP_YEAR(year) ? 366 : 365;
  time  -= days; // now it is days in this year, starting at 0
  
  days=0;
  month=0;
  monthLength=0;
  for (month=0; month<12; month++) {
    monthLength = monthDays[month];
    if(LEAP_YEAR(year) && month==1) monthLength++;// february
    if (time >= monthLength) 
      time -= monthLength;
    else  break;
  }
  tm.Month = month + 1;  // jan is month 1  
  tm.Day = time + 1;     // day of month
}

time_t makeTime(const tmElements_t &tm){   
// assemble time elements into time_t 
// note year argument is offset from 1970 (see macros in time.h to convert to other formats)
// previous version used full four digit year (or digits since 2000),i.e. 2009 was 2009 or 9
  
  int i;
  uint32_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds= tm.Year*(86400 * 365);
  for (i = 0; i < tm.Year; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  86400;   // add extra days for leap years
    }
  }
  
  // add days for this year, months start from 1
  for (i = 1; i < tm.Month; i++) {
    if ( (i == 2) && LEAP_YEAR(tm.Year)) { 
      seconds += 86400 * 29;
    } else {
      seconds += 86400 * monthDays[i-1];  //monthDay array starts from 0
    }
  }
  seconds+= (tm.Day-1) * 86400;
  seconds+= tm.Hour * 3600;
  seconds+= tm.Minute * 60;
  seconds+= tm.Second;
  return (time_t)seconds; 
}
#endif // CTG_USE_tmElements_t

} //namespace