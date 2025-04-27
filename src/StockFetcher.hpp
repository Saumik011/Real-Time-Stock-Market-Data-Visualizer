#pragma once
#include <string>
#include <vector>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

struct StockDataPoint {
    std::string timestamp;
    double open, high, low, close, volume;
};

class StockFetcher {
public:
    using Callback = std::function<void(const std::vector<StockDataPoint>&, const std::string& error)>;
    StockFetcher();
    ~StockFetcher();
    void start(const std::string& symbol, int interval_seconds, Callback cb);
    void stop();
private:
    std::thread fetch_thread;
    std::atomic<bool> running;
    std::mutex mtx;
    std::condition_variable cv;
    std::string current_symbol;
    int current_interval;
    Callback user_callback;
    void fetchLoop();
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);
    std::vector<StockDataPoint> parseAlphaVantage(const std::string& json_str, std::string& error);
};
