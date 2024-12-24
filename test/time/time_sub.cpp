#include <iostream>
#include <typeinfo>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

int main() {
    // 获取当前年月日
    // boost::gregorian::date new_date  = boost::gregorian::day_clock::local_day();
    boost::gregorian::date new_date(2024, 1, 1);
    int y = new_date.year();
    int m = new_date.month().as_number();
    std::cout << m << std::endl;
    // 获取当前时间
    boost::posix_time::ptime current_time = boost::posix_time::second_clock::local_time();
    // 获取小时、分钟和秒
    int hours = current_time.time_of_day().hours();
    int minutes = current_time.time_of_day().minutes();
    int seconds = current_time.time_of_day().seconds();
    // 输出时分秒
    std::cout << "Hours: " << hours << std::endl;
    std::cout << "Minutes: " << minutes << std::endl;
    std::cout << "Seconds: " << seconds << std::endl;
    std::string time_ymd = std::to_string(new_date.year())+std::to_string(new_date.month().as_number())+std::to_string(new_date.day());
    // y(year) m(month) d(day)
    std::cout << time_ymd << std::endl;
    std::string time_hds = std::to_string(hours)+":"+std::to_string(minutes)+":"+std::to_string(seconds);
    std::cout << time_hds << std::endl;
    return 0;
}
