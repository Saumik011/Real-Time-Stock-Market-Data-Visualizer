#include "UI.hpp"
#include <imgui.h>

UI::UI() {}

void UI::draw(
    std::vector<std::string>& stockSymbols,
    std::string& selectedSymbol,
    ChartRenderer::ChartType& chartType,
    bool& showSMA,
    int& smaPeriod
) {
    ImGui::Begin("Controls");
    ImGui::Text("Stock Selection");
    static int selected = 0;
    for (size_t i = 0; i < stockSymbols.size(); ++i) {
        if (ImGui::Selectable(stockSymbols[i].c_str(), selected == (int)i)) {
            selected = (int)i;
            selectedSymbol = stockSymbols[i];
        }
    }
    ImGui::Separator();
    ImGui::Text("Chart Type");
    int chartTypeInt = (int)chartType;
    ImGui::RadioButton("Candlestick", &chartTypeInt, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Line", &chartTypeInt, 1);
    chartType = (ChartRenderer::ChartType)chartTypeInt;
    ImGui::Separator();
    ImGui::Checkbox("Show SMA", &showSMA);
    ImGui::SliderInt("SMA Period", &smaPeriod, 2, 30);
    ImGui::End();
}
