#pragma once

// 一个抽象类，用于禁止拷贝和赋值
namespace mymuduo {
class noncopyable {
   public:
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;

   protected:
    noncopyable() = default;
    ~noncopyable() = default;
};
};  // namespace mymuduo