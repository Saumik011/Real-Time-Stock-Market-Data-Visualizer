#include "ChartRenderer.hpp"
#include <implot.h>
#include <algorithm>

void ChartRenderer::PlotCandlestick(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, bool tooltip, float width_percent, ImVec4 bullCol, ImVec4 bearCol) {
    ImDrawList* draw_list = ImPlot::GetPlotDrawList();
    double half_width = count > 1 ? (xs[1] - xs[0]) * width_percent : width_percent;
    if (ImPlot::IsPlotHovered() && tooltip) {
        ImPlotPoint mouse = ImPlot::GetPlotMousePos();
        for (int i = 0; i < count; ++i) {
            // Show tooltip if mouse is close to the candle center (within a threshold)
            if (fabs(mouse.x - xs[i]) < 0.5) {
                ImGui::BeginTooltip();
                ImGui::Text("O: %.2f\nC: %.2f\nL: %.2f\nH: %.2f", opens[i], closes[i], lows[i], highs[i]);
                ImGui::EndTooltip();
                break;
            }
        }
    }
    // Draw candlesticks
    for (int i = 0; i < count; ++i) {
        ImVec2 open_pos = ImPlot::PlotToPixels(xs[i] - half_width, opens[i]);
        ImVec2 close_pos = ImPlot::PlotToPixels(xs[i] + half_width, closes[i]);
        ImVec2 low_pos = ImPlot::PlotToPixels(xs[i], lows[i]);
        ImVec2 high_pos = ImPlot::PlotToPixels(xs[i], highs[i]);
        ImU32 color = ImGui::GetColorU32(opens[i] > closes[i] ? ImVec4(1.0f, 0.0f, 0.0f, 1.0f) : ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        draw_list->AddLine(low_pos, high_pos, color);
        draw_list->AddRectFilled(open_pos, close_pos, color);
    }
}

ChartRenderer::ChartRenderer() {}

std::vector<double> ChartRenderer::computeSMA(const std::vector<StockDataPoint>& data, int period) {
    std::vector<double> sma;
    if (data.size() < period) return sma;
    double sum = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        sum += data[i].close;
        if (i >= period) sum -= data[i - period].close;
        if (i >= period - 1) sma.push_back(sum / period);
    }
    return sma;
}

void ChartRenderer::draw(const std::vector<StockDataPoint>& data, ChartType type, bool showSMA, int smaPeriod) {
    if (data.empty()) return;
    std::vector<double> xs, opens, highs, lows, closes;
    xs.reserve(data.size()); opens.reserve(data.size()); highs.reserve(data.size()); lows.reserve(data.size()); closes.reserve(data.size());
    for (int i = (int)data.size() - 1; i >= 0; --i) { // Reverse order for time
        xs.push_back((double)(data.size() - 1 - i));
        opens.push_back(data[i].open);
        highs.push_back(data[i].high);
        lows.push_back(data[i].low);
        closes.push_back(data[i].close);
    }
    if (ImPlot::BeginPlot("Stock Chart", "Time", "Price", ImVec2(-1,400))) {
        if (type == ChartType::Candlestick) {
            PlotCandlestick("Candlestick", xs.data(), opens.data(), closes.data(), lows.data(), highs.data(), (int)xs.size());
        } else {
            ImPlot::PlotLine("Close Price", xs.data(), closes.data(), (int)xs.size());
        }
        if (showSMA) {
            auto sma = computeSMA(data, smaPeriod);
            if (!sma.empty()) {
                std::vector<double> sma_xs(xs.begin() + (xs.size() - sma.size()), xs.end());
                ImPlot::PlotLine("SMA", sma_xs.data(), sma.data(), (int)sma.size());
            }
        }
        ImPlot::EndPlot();
    }
}
