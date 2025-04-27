#pragma once
#include <string>
#include <vector>
#include "ChartRenderer.hpp"

class UI {
public:
    UI();
    void draw(
        std::vector<std::string>& stockSymbols,
        std::string& selectedSymbol,
        ChartRenderer::ChartType& chartType,
        bool& showSMA,
        int& smaPeriod
    );
};
