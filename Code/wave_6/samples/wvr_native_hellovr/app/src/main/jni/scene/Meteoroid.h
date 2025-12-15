// Meteoroid.h
#pragma once

#include <Object.h> // Erbt von Object, genau wie Sphere
#include <Shader.h>
#include <VertexArrayObject.h>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <map>
#include <memory>

#include "Vectors.h"
#include "Matrices.h"

#include "GoldmannSheet.h"
#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include "HelperFunctions.h" // Für calc_rotation_matrix


class Meteoroid : public Object {
public:
    Meteoroid();
    virtual ~Meteoroid();

    // Animationssteuerung
    void start_animation();
    void pause_animation();
    void resume_animation();
    void point_detected();

    // Position des "Fixsterns" / Thales-Punkts ändern
    void star_position_changed(const Vector3& star_position);

    // Die virtuelle Draw-Methode, die von der Engine aufgerufen wird
    virtual void draw(const Matrix4& projection, const Matrix4& eye, const Matrix4& view, const Vector4& lightDir);

private:
    // --- OpenGL-Member (wie in Sphere.cpp) ---
    int mModelviewProjectionLocation;
    int mPositionHandle;
    int mNormalHandle;
    int mColorHandle;
    int mModelMatrixHandle;
    int mViewMatrixHandle;
    int mProjectionMatrixHandle;

    int vCount = 0;
    VertexArrayObject* mVAO;

    // --- Logik-Member (aus meteoroid.py) ---
    GoldmannSheet m_goldmann_sheet;
    std::string m_perimetry_status;

    float m_radius;
    std::vector<double> m_longitudes_original;
    std::vector<double> m_longitudes;
    float m_meteoroid_speed;
    double m_sec_per_longitude;

    int m_current_longitude_index;
    std::shared_ptr<MeteoroideSize> m_current_size;
    std::map<MeteoroidSizeID, std::shared_ptr<MeteoroideSize>> m_size_map;

    std::chrono::time_point<std::chrono::high_resolution_clock> m_current_longitude_start_time;
    double m_passed_seconds; // Gesamtzeit seit Start

    glm::mat4 m_R; // Rotationsmatrix

    // Status für Pause-Modus
    glm::vec3 m_paused_star_position; //
    std::shared_ptr<MeteoroideSize> m_paused_star_size;
    PolarPoint m_paused_star_p;
    double m_paused_passed_seconds;

    // Zufallsgenerator
    std::mt19937 m_rng;

    // --- Private Methoden ---

    // Initialisierung (parallel zu Sphere::initSphere)
    void initMeteoroidSphere();
    void initVertexData(std::vector<float>& alVertix);

    // Logik-Helfer (übersetzt aus meteoroid.py)
    struct CurrentPointInfo {
        bool is_visible;
        glm::vec3 position;
        std::shared_ptr<MeteoroideSize> size;
        PolarPoint p;
    };
    CurrentPointInfo get_current_point_info();

    std::pair<glm::vec3, PolarPoint> _get_coordinates(double longitude, double theta_regler);
    void _reset_animation();
    static double _theta_from_regler(double theta_regler) ;
    double _min_max_normalization(double value) const;
    std::shared_ptr<MeteoroideSize> get_next_valid_size(MeteoroidSizeID start_id);
};