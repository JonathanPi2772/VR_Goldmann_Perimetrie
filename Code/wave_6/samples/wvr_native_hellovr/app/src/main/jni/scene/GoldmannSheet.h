// GoldmannSheet.h
#pragma once

#include <vector>
#include <memory>
#include "GoldmannSizes.h"
#include "Settings.h"
using namespace std;

// Entspricht der PolarPoint-Klasse
struct PolarPoint {
    float theta;
    float phi;
};



struct SheetEntry {
    std::vector<PolarPoint> points;
    float normalized_angle;
    std::string luminance_id;
};

class GoldmannSheet {
private:
    std::vector<PolarPoint> m_points;
    std::vector<AnyMeteoroidSize> m_sizes;

    // Access map via longitude and size




public:
    GoldmannSheet() {}

    // Access map via longitude and size
    map<int, map<MeteoroidSizeID, map<string, SheetEntry>>> m_sheet_right;
    map<int, map<MeteoroidSizeID, map<string, SheetEntry>>> m_sheet_left;
    map<int, map<MeteoroidSizeID, map<string, SheetEntry>>>:: iterator outer_it;
    map<MeteoroidSizeID, map<string, SheetEntry>>:: iterator middle_it;
    map<string, SheetEntry>:: iterator inner_it;


    void setup_sheet(
            std::vector<PerimetryVector> meteoroid_l,
            std::map<MeteoroidSizeID, std::vector<std::string>> sizes,
            int eye
                    ) {
        for (int size_idx = 5; size_idx >= 1; size_idx--) {
            MeteoroidSizeID size_id = getSizeByNumber(size_idx);
            AnyMeteoroidSize size = m_size_map.at(size_id);
            std::vector<string> lumins = sizes.at(size_id);
            for (auto lum : lumins) {
                for (auto &vec: meteoroid_l) {
                    SheetEntry new_entry = SheetEntry();
                    new_entry.points = {};
                    if (eye == 1) {
                        new_entry.normalized_angle = vec.normative_val_right[size_idx-1];
                        m_sheet_right[vec.angle_deg][size_id][lum] = new_entry;
                    } else if (eye == 2) {
                        new_entry.normalized_angle = vec.normative_val_left[size_idx-1];
                        m_sheet_left[vec.angle_deg][size_id][lum] = new_entry;
                    }
                }
            }
        }
    }

    void add_point(PolarPoint p, AnyMeteoroidSize size, int longitude, int eye, std::string luminance) {
        m_points.push_back(p);
        m_sizes.push_back(size);


        MeteoroidSizeID size_id = std::visit([](auto&& s) {
            return s.get_id();
        }, size);
        if (eye == 1) {
            m_sheet_right[longitude][size_id][luminance].points.push_back(p);
        } else if (eye == 2) {
            m_sheet_left[longitude][size_id][luminance].points.push_back(p);
        }
    }

    // Getter (optional, aber guter Stil)
    const std::vector<PolarPoint>& get_points() const { return m_points; }
    const std::vector<AnyMeteoroidSize>& get_sizes() const { return m_sizes; }
};