// GoldmannSizes.h
#pragma once

#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>

// Ein Enum, um die Größen typsicher in Maps zu verwenden
enum class MeteoroidSizeID { I, II, III, IV, V, None };

// Basisklasse, die goldmann_sizes.py::MeteoroideSize entspricht
class MeteoroideSize {
protected:
    double m_size_rad;
    double m_distance = -1.0;
    double m_size_meter_sq = -1.0;

public:
    MeteoroideSize(double size_deg) {
        m_size_rad = size_deg * M_PI * (1.0 / 180.0);
    }
    virtual ~MeteoroideSize() {}

    void set_distance(double distance) {
        m_distance = distance;
        // A = π * r²
        double radius_m = std::tan(m_size_rad * 0.5) * m_distance;
        m_size_meter_sq = (radius_m * radius_m) * M_PI;
    }

    double get_size_meter_sq() {
        if (m_size_meter_sq < 0) {
            return 0;
            // throw std::runtime_error("Size could not be calculated yet, missing Distance.");
        }
        return m_size_meter_sq;
    }

    // Virtuelle Funktionen für die Logik
    virtual MeteoroidSizeID get_id() const = 0;
    virtual MeteoroidSizeID get_next_size_id() const = 0;
    virtual std::string get_name() const = 0;
};

// Abgeleitete Klassen
class Size_V : public MeteoroideSize {
public:
    Size_V() : MeteoroideSize(1.72) {}
    MeteoroidSizeID get_id() const override { return MeteoroidSizeID::V; }
    MeteoroidSizeID get_next_size_id() const override { return MeteoroidSizeID::IV; }
    std::string get_name() const override { return "Size_V"; }
};

class Size_IV : public MeteoroideSize {
public:
    Size_IV() : MeteoroideSize(0.86) {}
    MeteoroidSizeID get_id() const override { return MeteoroidSizeID::IV; }
    MeteoroidSizeID get_next_size_id() const override { return MeteoroidSizeID::III; }
    std::string get_name() const override { return "Size_IV"; }
};

class Size_III : public MeteoroideSize {
public:
    Size_III() : MeteoroideSize(0.43) {}
    MeteoroidSizeID get_id() const override { return MeteoroidSizeID::III; }
    MeteoroidSizeID get_next_size_id() const override { return MeteoroidSizeID::II; }
    std::string get_name() const override { return "Size_III"; }
};

class Size_II : public MeteoroideSize {
public:
    Size_II() : MeteoroideSize(0.22) {}
    MeteoroidSizeID get_id() const override { return MeteoroidSizeID::II; }
    MeteoroidSizeID get_next_size_id() const override { return MeteoroidSizeID::I; }
    std::string get_name() const override { return "Size_II"; }
};

class Size_I : public MeteoroideSize {
public:
    Size_I() : MeteoroideSize(0.11) {}
    MeteoroidSizeID get_id() const override { return MeteoroidSizeID::I; }
    MeteoroidSizeID get_next_size_id() const override { return MeteoroidSizeID::None; }
    std::string get_name() const override { return "Size_I"; }
};