// GoldmannSheet.h
#pragma once

#include <vector>
#include <memory>
#include "GoldmannSizes.h"

// Entspricht der PolarPoint-Klasse
struct PolarPoint {
    float theta;
    float phi;
};

class GoldmannSheet {
private:
    std::vector<PolarPoint> m_points;
    std::vector<std::shared_ptr<MeteoroideSize>> m_sizes;

public:
    GoldmannSheet() {}

    void add_point(PolarPoint p, std::shared_ptr<MeteoroideSize> size) {
        m_points.push_back(p);
        m_sizes.push_back(size);
    }

    // Getter (optional, aber guter Stil)
    const std::vector<PolarPoint>& get_points() const { return m_points; }
    const std::vector<std::shared_ptr<MeteoroideSize>>& get_sizes() const { return m_sizes; }
};