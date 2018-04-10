#include "tools.h"
#include <ctime>

using namespace std;
using namespace NLMISC;

//NLMISC::CMutex LocalTime.m_mutex("LocalTime.m_mutex");
//char LocalTime.m_cstime[LocalTime.TIME_STR_MAX];
//NLMISC::TTime LocalTime.m_CurrTime;


CLocalTime::CLocalTime():m_mutex("LocalTime.m_mutex"),m_TimeZone(0)
{
    m_CurrTime = CTime::getLocalTime();
    CalcTimeZone();
}


tm CLocalTime::gettime( const time_t time )
{
    struct tm timeinfo;

#ifdef NL_OS_UNIX
    localtime_r(&time, &timeinfo);
#else
    localtime_s(&timeinfo, &time);
#endif // NL_OS_UNIX

	//m_mutex.enter();
	//tm *tms = std::localtime(&time);
	//timeinfo = *tms;
	//m_mutex.leave();

	return timeinfo;
}

tm CLocalTime::gettime()
{
    //time_t t;
    //time(&t);
	return gettime(std::time(NULL));
}

uint32 CLocalTime::str2time(std::string strTime)
{
    tm CurTime = LocalTime.gettime(CTime::getSecondsSince1970());

    // 解析年月日;
    std::string::size_type unOffsetA = strTime.find_first_of( '-' );

    // 解析年;
    if ( unOffsetA != std::string::npos )
    {
        std::string year = strTime.substr( 0 , unOffsetA );
        if ( year.empty() )
            return 0;

        CurTime.tm_year = atoi( year.c_str() )-1900;
        strTime = strTime.substr( unOffsetA + 1 );

        // 解析月;
        unOffsetA = strTime.find_first_of( '-' );
        if ( unOffsetA != std::string::npos )
        {
            std::string mon = strTime.substr( 0 , unOffsetA );
            if ( mon.empty() )
                return -1;

            CurTime.tm_mon = atoi( mon.c_str() )-1;
            strTime = strTime.substr( unOffsetA + 1 );

            // 解析日;
            unOffsetA = strTime.find_first_of(' ');
            if ( unOffsetA != std::string::npos )
            {
                std::string day = strTime.substr( 0 , unOffsetA );
                if ( day.empty() )
                    return -1;

                CurTime.tm_mday = atoi( day.c_str() );
                strTime = strTime.substr( unOffsetA + 1 );
            }
        }
    }


    // 解析时;
    unOffsetA = strTime.find_first_of( ':' );
    if ( unOffsetA != std::string::npos )
    {
        std::string hour = strTime.substr( 0 , unOffsetA );
        if ( hour.empty() )
            return -1;

        CurTime.tm_hour = atoi( hour.c_str() );
        strTime = strTime.substr( unOffsetA + 1 );

        // 解析分;
        unOffsetA = strTime.find_first_of( ':' );
        if ( unOffsetA != std::string::npos )
        {
            std::string min = strTime.substr( 0 , unOffsetA );
            if ( min.empty() )
                return -1;

            CurTime.tm_min = atoi( min.c_str() );
            strTime = strTime.substr( unOffsetA + 1 );

            // 解析秒;
            if ( !strTime.empty() )
            {
                CurTime.tm_sec = atoi( strTime.c_str() );
            }
        }
    }

    return (uint32)std::mktime(&CurTime);
}

uint32 CLocalTime::mktime( const uint year, const uint month, const uint day, const uint hour, const uint minute, const uint second )
{
	tm tms;
	tms.tm_year = year;
	tms.tm_mon  = month;
	tms.tm_mday = day;
	tms.tm_hour = hour;
	tms.tm_min  = minute;
	tms.tm_sec  = second;
    tms.tm_isdst= 0;

	return (uint32)std::mktime(&tms);		//nl_mktime
}

uint32 CLocalTime::gettime( TTimeOperaion time_opt, sint32 time_val, uint32 base_time )
{
	uint32 change_second = ( base_time > 0 ) ? base_time : CTime::getSecondsSince1970();

    tm t = gettime( change_second );
  
    switch (time_opt)
    {
    case NEXT_DAY:
        {
            //  tm_mday [1-31]
            if ( t.tm_mday > time_val )
            {
                // 当前日期大于需要的日期， 设置为下个月的日期
                ++t.tm_mon;
                t.tm_mday = time_val;
            }
            else
            {
                t.tm_mday = time_val;
            }
            change_second = std::mktime(&t);
            break;
        }
    case NEXT_WEEK:
        {
            //  tm_mday [0-6]
            if ( t.tm_wday > time_val )
            {
                // 当前日期大于需要的日期， 设置为下星期的日期
                sint day = t.tm_wday - time_val;
                change_second -= DAY_SECONDS * day;
                change_second += WEEK_SECONDS;
            }
            else
            {
                sint day = time_val - t.tm_wday;
                change_second += DAY_SECONDS * day;
            }  
            break;
        }
    case NEXT_HOUR:
        {
            //  tm_hour [0,23]
            if ( t.tm_hour > time_val )
            {
                // 当前时间大于需要的时间， 设置为第二天的时间
                sint hour = t.tm_hour - time_val;
                change_second -= HOUR_SECONDS * hour;
                change_second += DAY_SECONDS;
            }
            else
            {
                sint hour = time_val - t.tm_hour;
                change_second += HOUR_SECONDS * hour;
            }
            break;
        }
    case ADD_HOUR:
        {
            sint32 day = time_val / DAY_HOURS;
            if (day>0)
            {
                //需要的Next_Hour大于一天  tm_mday加上大于的天数
                t.tm_mday += day; 
            }
            else
            {
                t.tm_hour += time_val;    
            }
            change_second = std::mktime(&t);
            break;
        }
    default:
        nlassertd(0);
        break;
    }

    return change_second;
}

uint32  CLocalTime::gettime( vector<TimeOPT> vct_opt )
{
    uint32 change_second = CTime::getSecondsSince1970();

    //for ( uint i=0; i<vct_opt.size(); ++i )
    //{
    //    //change_second = gettime( vct_opt[i].opt_type, vct_opt[i].value, change_second );
    //}

    return change_second;
}

const char* CLocalTime::printtime( tm& tms )
{
    std::strftime (m_cstime, TIME_STR_MAX, "%Y/%m/%d %H:%M:%S", &tms);
    return m_cstime;
}

const char* CLocalTime::printtime( const uint32 time )
{
    tm t = gettime(time);
    return printtime(t);
}

void CLocalTime::CalcTimeZone()
{
    time_t time_utc;  

    // Get the UTC time  
    time(&time_utc);  

    tm tm_local = gettime(time_utc);

    time_t time_local;  

    // Change tm to time_t   
    time_local = std::mktime(&tm_local);  

    // Change it to GMT tm  
    tm* tm_gmt = gmtime(&time_utc);

    m_TimeZone = tm_local.tm_hour - tm_gmt->tm_hour;  
}



