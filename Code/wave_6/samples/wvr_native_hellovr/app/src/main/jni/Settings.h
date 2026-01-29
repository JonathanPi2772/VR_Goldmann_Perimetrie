#pragma once

// Diese Datei dient als Ersatz für die settings.py Datei.
// Hier können globale Konfigurationswerte für die Szene gespeichert werden.

#include <Vectors.h>
#include <map>
#include <glm/glm.hpp>
#include "GoldmannSizes.h"

// Stary Sky
// -----------
constexpr bool SHOW_STARS = false;
constexpr int NUM_STARS = 200;
constexpr float STAR_FIELD_RADIUS = 250.0f;
constexpr float STARS_DISTANCE = 249.0f;
constexpr float STAR_SIZE_VARIATION = 1.6f;
constexpr float STAR_POS_VARIATION = 1.8f;
// Terrain
// ------------
constexpr bool SHOW_TERRAIN = false;
constexpr float PEAK_AMPLITUDE = 20.0f;
constexpr float SIGMA = 7.0f;
constexpr float SUMMIT_Y = -1.70f;
constexpr float MOUNTAIN_EXTENT = 20.0f;
constexpr int   MOUNTAIN_RESOLUTION = 150;
constexpr std::array<float, 3> MOUNTAIN_COLOR = {0.06f, 0.25f, 0.06f};

// Focus Point
constexpr float FOCUS_POINT_DISTANCE = 20.0f;


// Meteroid
struct PerimetryVector {
    int angle_deg;
    std::vector<double> normative_val_right;
    std::vector<double> normative_val_left;
    std::string luminance;
    MeteoroidSizeID size;
};
constexpr float METEOROID_DISTANCE = 50.0f;
constexpr bool METEOROID_RANDOM = true;
constexpr float METEOROID_SPEED = 5.0f; // deg/sec
constexpr int NUMBER_ITERATIONS_PER_SIZE = 2;
constexpr float REACTION_TIME = 0.5; // seconds

// extern const std::string TARGET_LUMINANCE_DB = "3e";
extern const std::map<MeteoroidSizeID, std::vector<std::string>> LUMINANCE_TO_USE;

extern const std::map<MeteoroidSizeID, bool> METEOROID_SIZES;
extern const std::map<MeteoroidSizeID, AnyMeteoroidSize> m_size_map;
extern const std::vector<PerimetryVector> METEOROID_LONGITUDES_DEG;
extern const Vector3 METEOROID_COLOR; //
extern const glm::vec3 GENERAL_THALES_POINT;
extern const Vector3 GENERAL_THALES_POINT_VEC;

// Headset
// Luminance Settings
// HTC Vive Focus 3 is approx 150 nits (check specs!)
constexpr float MAX_HEADSET_LUMINANCE_NITS = 150.0f;

// Goldmann Standard Background: 31.5 asb ~= 10 cd/m² (nits)
constexpr float BACKGROUND_LUMINANCE_NITS = 10.0f;
// Angle for eye tracker
const float MAX_ACCEPTANCE_ANGLE_DEG = 6.0f;

