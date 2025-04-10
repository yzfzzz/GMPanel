#pragma once

#include <iostream>
#include <string>

// 时间类
namespace mymuduo {
class Timestamp {
   public:
    Timestamp();
    explicit Timestamp(int64_t microSecondsSinceEpoch);
    static Timestamp now();
    std::string toString() const;

   private:
    int64_t microSecondsSinceEpoch_;
};
};  // namespace mymuduo