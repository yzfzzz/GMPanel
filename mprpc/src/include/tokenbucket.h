#include <chrono>
#include <mutex>

class TokenBucket {
   public:
    TokenBucket(int capacity = 5000, int rate = 8000)
        : capacity_(capacity), rate_(rate), tokens_(capacity) {}

    bool consume(int tokens) {
        std::lock_guard<std::mutex> lock(mutex_);
        refill();
        if (tokens_ >= tokens) {
            tokens_ -= tokens;
            return true;
        } else {
            return false;
        }
    }

   private:
    void refill() {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(
            now - last_refill_);
        int new_tokens = static_cast<int>(duration.count() * rate_);
        if (new_tokens > 0) {
            tokens_ = std::min(capacity_, tokens_ + new_tokens);
            last_refill_ = now;
        }
    }

    int capacity_;
    int rate_;
    int tokens_;
    std::chrono::steady_clock::time_point last_refill_ =
        std::chrono::steady_clock::now();
    std::mutex mutex_;
};
