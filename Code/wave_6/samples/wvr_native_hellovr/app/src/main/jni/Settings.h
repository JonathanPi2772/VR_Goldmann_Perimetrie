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
constexpr int TARGET_LUMINANCE_DB = -1;
constexpr float METEOROID_DISTANCE = 50.0f;
constexpr bool METEOROID_RANDOM = true;
constexpr float METEOROID_SPEED = 10.0f; // deg/sec


extern const std::map<MeteoroidSizeID, bool> METEOROID_SIZES;
extern const std::vector<double> METEOROID_LONGITUDES_DEG;
extern const Vector3 METEOROID_COLOR; //
extern const glm::vec3 GENERAL_THALES_POINT;
extern const Vector3 GENERAL_THALES_POINT_VEC;

