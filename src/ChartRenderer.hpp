#pragma once
#include <imgui.h>
#include <vector>
#include <string>
#include "StockFetcher.hpp"

class ChartRenderer {
public:
    static void PlotCandlestick(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, bool tooltip = true, float width_percent = 0.25f, ImVec4 bullCol = ImVec4(0,1,0,1), ImVec4 bearCol = ImVec4(1,0,0,1));
    enum class ChartType { Candlestick, Line };
    ChartRenderer();
    void draw(const std::vector<StockDataPoint>& data, ChartType type, bool showSMA = false, int smaPeriod = 10);
private:
    std::vector<double> computeSMA(const std::vector<StockDataPoint>& data, int period);
};
