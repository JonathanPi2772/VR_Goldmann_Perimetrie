#include "Settings.h"
#include "Vectors.h"

const std::map<MeteoroidSizeID, AnyMeteoroidSize> m_size_map = {
        {MeteoroidSizeID::I, Size_I()},
        {MeteoroidSizeID::II, Size_II()},
        {MeteoroidSizeID::III, Size_III()},
        {MeteoroidSizeID::IV, Size_IV()},
        {MeteoroidSizeID::V, Size_V()},
        {MeteoroidSizeID::None, Size_O()}
};

const std::map<MeteoroidSizeID, std::vector<std::string>> LUMINANCE_TO_USE = {
        {MeteoroidSizeID::I, {"3e", "2e", /*"1e"*/}},
        {MeteoroidSizeID::II, {}},
        {MeteoroidSizeID::III, {/*"4e"*/}},
        {MeteoroidSizeID::IV, {}},
        {MeteoroidSizeID::V, {"4e"}},
        {MeteoroidSizeID::None, {}}
};


const std::vector<PerimetryVector> METEOROID_LONGITUDES_DEG = {
        PerimetryVector{(0*90)+0, std::vector<double>{90, 90, 90, 90, 85}, std::vector<double>{90, 90, 90, 90, 70}, "1a", MeteoroidSizeID::None},
        //PerimetryVector{(0*90)+15, std::vector<double>{90, 90, 90, 90, 90}, std::vector<double>{90, 90, 90, 90, 70}, "1a", MeteoroidSizeID::None},
        PerimetryVector{(0*90)+30, std::vector<double>{90, 90, 90, 90, 80}, std::vector<double>{90, 90, 90, 90, 70}, "1a", MeteoroidSizeID::None},
        //PerimetryVector{(0*90)+45, std::vector<double>{90, 90, 90, 90, 70}, std::vector<double>{90, 90, 90, 90, 65}, "1a", MeteoroidSizeID::None},
        PerimetryVector{(0*90)+60, std::vector<double>{90, 90, 90, 90, 65}, std::vector<double>{90, 90, 90, 90, 65}, "1a", MeteoroidSizeID::None},
        //PerimetryVector{(0*90)+75, std::vector<double>{90, 90, 90, 90, 60}, std::vector<double>{90, 90, 90, 90, 55}, "1a", MeteoroidSizeID::None},
        PerimetryVector{(0*90)+90, std::vector<double>{90, 90, 90, 90, 55}, std::vector<double>{90, 90, 90, 90, 55}, "1a", MeteoroidSizeID::None},
//
        //PerimetryVector{(1*90)+15, std::vector<double>{90, 90, 90, 90, 55}, std::vector<double>{90, 90, 90, 90, 60}, "1a", MeteoroidSizeID::None},
        PerimetryVector{(1*90)+30, std::vector<double>{90, 90, 90, 90, 65}, std::vector<double>{90, 90, 90, 90, 65}, "1a", MeteoroidSizeID::None},
        //PerimetryVector{(1*90)+45, std::vector<double>{90, 90, 90, 90, 65}, std::vector<double>{90, 90, 90, 90, 70}, "1a", MeteoroidSizeID::None},
        PerimetryVector{(1*90)+60, std::vector<double>{90, 90, 90, 90, 70}, std::vector<double>{90, 90, 90, 90, 80}, "1a", MeteoroidSizeID::None},
        //PerimetryVector{(1*90)+75, std::vector<double>{90, 90, 90, 90, 70}, std::vector<double>{90, 90, 90, 90, 90}, "1a", MeteoroidSizeID::None},
        PerimetryVector{(1*90)+90, std::vector<double>{90, 90, 90, 90, 70}, std::vector<double>{90, 90, 90, 90, 85}, "1a", MeteoroidSizeID::None},
        //
        //PerimetryVector{(2*90)+15, std::vector<double>{90, 90, 90, 90, 75}, std::vector<double>{90, 90, 90, 90, 85}, "1a", MeteoroidSizeID::None},
        PerimetryVector{(2*90)+30, std::vector<double>{90, 90, 90, 90, 75}, std::vector<double>{90, 90, 90, 90, 85}, "1a", MeteoroidSizeID::None},
        //PerimetryVector{(2*90)+45, std::vector<double>{90, 90, 90, 90, 75}, std::vector<double>{90, 90, 90, 90, 90}, "1a", MeteoroidSizeID::None},
        PerimetryVector{(2*90)+60, std::vector<double>{90, 90, 90, 90, 75}, std::vector<double>{90, 90, 90, 90, 85}, "1a", MeteoroidSizeID::None},
        //PerimetryVector{(2*90)+75, std::vector<double>{90, 90, 90, 90, 85}, std::vector<double>{90, 90, 90, 90, 75}, "1a", MeteoroidSizeID::None},
        PerimetryVector{(2*90)+90, std::vector<double>{90, 90, 90, 90, 75}, std::vector<double>{90, 90, 90, 90, 75}, "1a", MeteoroidSizeID::None},
        //
        //PerimetryVector{(3*90)+15, std::vector<double>{90, 90, 90, 90, 75}, std::vector<double>{90, 90, 90, 90, 80}, "1a", MeteoroidSizeID::None},
        PerimetryVector{(3*90)+30, std::vector<double>{90, 90, 90, 90, 85}, std::vector<double>{90, 90, 90, 90, 75}, "1a", MeteoroidSizeID::None},
        //PerimetryVector{(3*90)+45, std::vector<double>{90, 90, 90, 90, 90}, std::vector<double>{90, 90, 90, 90, 75}, "1a", MeteoroidSizeID::None},
        PerimetryVector{(3*90)+60, std::vector<double>{90, 90, 90, 90, 85}, std::vector<double>{90, 90, 90, 90, 75}, "1a", MeteoroidSizeID::None},
        //PerimetryVector{(3*90)+75, std::vector<double>{90, 90, 90, 90, 85}, std::vector<double>{90, 90, 90, 90, 75}, "1a", MeteoroidSizeID::None},

         };
const Vector3 METEOROID_COLOR = Vector3(1.0f, 0.0f, 0.0f); //
const glm::vec3 GENERAL_THALES_POINT = glm::vec3(0.0f, 0.0f, -METEOROID_DISTANCE);
const Vector3 GENERAL_THALES_POINT_VEC = Vector3(0.0f, 0.0f, -METEOROID_DISTANCE);

