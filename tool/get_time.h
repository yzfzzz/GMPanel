#include <iostream>
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// cmake lib: boost_date_time
namespace monitor {
class get_curTime{
public:
    std::string get_year_mon_day()
    {
        boost::gregorian::date new_date  = boost::gregorian::day_clock::local_day();
        int y = new_date.year();
        int m = new_date.month().as_number();
        int d = new_date.day();
        std::string y_str = std::to_string(y);
        std::string m_str = std::to_string(m);
        std::string d_str = std::to_string(d);
        if(m < 10)
        {
            m_str = "0"+ m_str;
        }
        if(d < 10)
        {
            d_str = "0"+d_str;
        }
        std::string time_ymd = y_str+m_str+d_str;
        return time_ymd;
    }

    std::string get_hour_min_sec()
    {
        // 获取当前时间
        boost::posix_time::ptime current_time = boost::posix_time::second_clock::local_time();
        // 获取小时、分钟和秒
        int hours = current_time.time_of_day().hours();
        int minutes = current_time.time_of_day().minutes();
        int seconds = current_time.time_of_day().seconds();

        std::string h_str = std::to_string(hours);
        std::string m_str = std::to_string(minutes);
        std::string s_str = std::to_string(seconds);

        if(hours < 10)
        {
            h_str = "0"+h_str;
        }
        if(minutes < 10)
        {
            m_str = "0"+m_str;
        }
        if(seconds< 10)
        {
            s_str = "0"+s_str;
        }
        std::string time_hds = h_str+":"+m_str+":"+s_str;
        return time_hds;
    }
};
}
