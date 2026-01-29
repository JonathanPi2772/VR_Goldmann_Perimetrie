// GoldmannSizes.h
#pragma once

#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>
#include "cmath"
#include "vector"



// Ein Enum, um die Größen typsicher in Maps zu verwenden
enum class MeteoroidSizeID { I, II, III, IV, V, None };

MeteoroidSizeID getSizeByNumber(int number);

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

    virtual int get_index() const = 0;
    virtual int get_speed() const = 0;


    double getStimulusIntensityInNits(const std::string& stimulus) {
        if (stimulus.length() < 2) {
            std::runtime_error("Invalid stimulus format. Expected format like '3e'.");
        }

        char numberChar = stimulus[0];
        char letterChar = stimulus[1];

        // Calculate attenuation from the number component (Coarse filter)
        // 4 -> 0dB, 3 -> 5dB, 2 -> 10dB, 1 -> 15dB
        int coarseDb = 0;
        switch (numberChar) {
            case '4': coarseDb = 0; break;
            case '3': coarseDb = 5; break;
            case '2': coarseDb = 10; break;
            case '1': coarseDb = 15; break;
            default: std::runtime_error("Invalid stimulus number. Must be 1-4.");
        }

        // Calculate attenuation from the letter component (Fine filter)
        // e -> 0dB, d -> 1dB, c -> 2dB, b -> 3dB, a -> 4dB
        int fineDb = 0;
        switch (letterChar) {
            case 'e': fineDb = 0; break;
            case 'd': fineDb = 1; break;
            case 'c': fineDb = 2; break;
            case 'b': fineDb = 3; break;
            case 'a': fineDb = 4; break;
            default: std::runtime_error("Invalid stimulus letter. Must be a-e.");
        }

        // Total attenuation in decibels
        int totalDb = coarseDb + fineDb;

        // Reference intensity for 4e (0 dB) is 315 cd/m^2 (nits)
        const double maxIntensityNits = 315.0;

        // Formula: I = I_max * 10^(-dB / 10)
        return maxIntensityNits * std::pow(10.0, -static_cast<double>(totalDb) / 10.0);
    }

    std::vector<float> GetGoldmannColor(std::string luminance_id, float background_nits, float max_headset_nits)
    {
        float targetNits = getStimulusIntensityInNits(luminance_id);
        targetNits += background_nits;

        // Check for hardware capability
        if (targetNits > max_headset_nits)
        {
            return std::vector<float>{1.0f, 1.0f, 1.0f, 1.0f}; // Return max brightness (255, 255, 255)
        }

        // Gamma correction to get linear RGB
        float normalizedIntensity = std::pow(targetNits / max_headset_nits, 1.0f / 2.2f);

        return std::vector<float>{normalizedIntensity, normalizedIntensity, normalizedIntensity, 1.0f};
    }
};

// Abgeleitete Klassen
class Size_V : public MeteoroideSize {
public:
    Size_V() : MeteoroideSize(1.72) {}
    MeteoroidSizeID get_id() const override { return MeteoroidSizeID::V; }
    MeteoroidSizeID get_next_size_id() const override { return MeteoroidSizeID::IV; }
    std::string get_name() const override { return "Size_V"; }
    int get_index() const override { return 5; }
    int get_speed() const override { return 5; }
};

class Size_IV : public MeteoroideSize {
public:
    Size_IV() : MeteoroideSize(0.86) {}
    MeteoroidSizeID get_id() const override { return MeteoroidSizeID::IV; }
    MeteoroidSizeID get_next_size_id() const override { return MeteoroidSizeID::III; }
    std::string get_name() const override { return "Size_IV"; }
    int get_index() const override { return 4; }
    int get_speed() const override { return 5; }
};

class Size_III : public MeteoroideSize {
public:
    Size_III() : MeteoroideSize(0.43) {}
    MeteoroidSizeID get_id() const override { return MeteoroidSizeID::III; }
    MeteoroidSizeID get_next_size_id() const override { return MeteoroidSizeID::II; }
    std::string get_name() const override { return "Size_III"; }
    int get_index() const override { return 3; }
    int get_speed() const override { return 5; }
};

class Size_II : public MeteoroideSize {
public:
    Size_II() : MeteoroideSize(0.22) {}
    MeteoroidSizeID get_id() const override { return MeteoroidSizeID::II; }
    MeteoroidSizeID get_next_size_id() const override { return MeteoroidSizeID::I; }
    std::string get_name() const override { return "Size_II"; }
    int get_index() const override { return 2; }
    int get_speed() const override { return 3; }
};

class Size_I : public MeteoroideSize {
public:
    Size_I() : MeteoroideSize(0.11) {}
    MeteoroidSizeID get_id() const override { return MeteoroidSizeID::I; }
    MeteoroidSizeID get_next_size_id() const override { return MeteoroidSizeID::None; }
    std::string get_name() const override { return "Size_I"; }
    int get_index() const override { return 1; }
    int get_speed() const override { return 2; }
};

class Size_O : public MeteoroideSize {
public:
    Size_O() : MeteoroideSize(0.0) {}
    MeteoroidSizeID get_id() const override { return MeteoroidSizeID::None; }
    MeteoroidSizeID get_next_size_id() const override { return MeteoroidSizeID::None; }
    std::string get_name() const override { return "Size_O"; }
    int get_index() const override { return 0; }
    int get_speed() const override { return 0; }
};


using AnyMeteoroidSize = std::variant<Size_I, Size_II, Size_III, Size_IV, Size_V, Size_O>;
