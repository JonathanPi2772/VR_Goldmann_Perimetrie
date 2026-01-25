#include "Settings.h"
#include "Vectors.h"

const std::map<MeteoroidSizeID, bool> METEOROID_SIZES = {
{MeteoroidSizeID::I, false},
{MeteoroidSizeID::II, false},
{MeteoroidSizeID::III, true},
{MeteoroidSizeID::IV, false},
{MeteoroidSizeID::V, true}
};

const std::vector<double> METEOROID_LONGITUDES_DEG = {
        45, 90, 135 //, 180, 225, 270, 315, 0
};
const Vector3 METEOROID_COLOR = Vector3(1.0f, 0.0f, 0.0f); //
const glm::vec3 GENERAL_THALES_POINT = glm::vec3(0.0f, 0.0f, -METEOROID_DISTANCE);
const Vector3 GENERAL_THALES_POINT_VEC = Vector3(0.0f, 0.0f, -METEOROID_DISTANCE);