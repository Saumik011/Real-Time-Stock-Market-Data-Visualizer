#include "StockFetcher.hpp"
#include <sstream>
#include <chrono>

StockFetcher::StockFetcher() : running(false) {}
StockFetcher::~StockFetcher() { stop(); }

void StockFetcher::start(const std::string& symbol, int interval_seconds, Callback cb) {
    stop();
    running = true;
    current_symbol = symbol;
    current_interval = interval_seconds;
    user_callback = cb;
    fetch_thread = std::thread(&StockFetcher::fetchLoop, this);
}

void StockFetcher::stop() {
    running = false;
    if (fetch_thread.joinable()) fetch_thread.join();
}

void StockFetcher::fetchLoop() {
    while (running) {
        std::string error;
        std::vector<StockDataPoint> data;
        // Use Alpha Vantage free API (demo key)
        std::string api_key = "demo";
        std::string url = "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=" + current_symbol + "&interval=1min&apikey=" + api_key + "&outputsize=compact";

        CURL* curl = curl_easy_init();
        std::string readBuffer;
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            CURLcode res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                error = curl_easy_strerror(res);
            }
            curl_easy_cleanup(curl);
        } else {
            error = "Failed to init curl";
        }
        if (error.empty()) {
            data = parseAlphaVantage(readBuffer, error);
        }
        if (user_callback) {
            user_callback(data, error);
        }
        for (int i = 0; i < current_interval && running; ++i)
            std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

size_t StockFetcher::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t total = size * nmemb;
    s->append((char*)contents, total);
    return total;
}

std::vector<StockDataPoint> StockFetcher::parseAlphaVantage(const std::string& json_str, std::string& error) {
    std::vector<StockDataPoint> result;
    try {
        auto j = nlohmann::json::parse(json_str);
        if (j.contains("Time Series (1min)")) {
            auto& ts = j["Time Series (1min)"];
            for (auto it = ts.begin(); it != ts.end(); ++it) {
                StockDataPoint dp;
                dp.timestamp = it.key();
                dp.open = std::stod(it.value()["1. open"].get<std::string>());
                dp.high = std::stod(it.value()["2. high"].get<std::string>());
                dp.low = std::stod(it.value()["3. low"].get<std::string>());
                dp.close = std::stod(it.value()["4. close"].get<std::string>());
                dp.volume = std::stod(it.value()["5. volume"].get<std::string>());
                result.push_back(dp);
            }
        } else {
            error = "API response missing Time Series (1min)";
        }
    } catch (const std::exception& e) {
        error = e.what();
    }
    return result;
}
