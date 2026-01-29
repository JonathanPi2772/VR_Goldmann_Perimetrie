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
#include "Settings.h"


class Meteoroid : public Object {
public:
    Meteoroid();
    virtual ~Meteoroid();

    // Variablen
    std::string m_perimetry_status;

    // Animationssteuerung
    void start_animation();
    void pause_animation(bool point_detected);
    void resume_animation();
    void point_detected();
    void reset_animation();


    // Position des "Fixsterns" / Thales-Punkts ändern
    void star_position_changed(const Vector3& star_position);
    GoldmannSheet m_goldmann_sheet;
    int mActiveEye;

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

    float m_radius;
    std::vector<PerimetryVector> m_longitudes_original;
    std::vector<PerimetryVector> m_longitudes;
    float m_meteoroid_speed;
    double m_sec_per_longitude;

    int m_current_longitude_index;
    AnyMeteoroidSize m_current_size;

    std::chrono::time_point<std::chrono::high_resolution_clock> m_current_longitude_start_time;
    double m_passed_seconds; // Gesamtzeit seit Start
    double m_current_radius_deg = 0.0; // State: Current position (starts outer)
    std::chrono::time_point<std::chrono::high_resolution_clock> m_last_update_time;

    glm::mat4 m_R; // Rotationsmatrix

    // Status für Pause-Modus
    glm::vec3 m_paused_star_position; //
    AnyMeteoroidSize m_paused_star_size;
    PolarPoint m_paused_star_p;
    double m_paused_passed_seconds;

    // Zufallsgenerator
    std::mt19937 m_rng;

    // --- Private Methoden ---

    // Initialisierung (parallel zu Sphere::initSphere)
    void initMeteoroidSphere();
    void setup_longitudes();
    void initVertexData(std::vector<float>& alVertix);

    // Logik-Helfer (übersetzt aus meteoroid.py)
    struct CurrentPointInfo {
        bool is_visible;
        glm::vec3 position;
        AnyMeteoroidSize size;
        PolarPoint p;
    };
    CurrentPointInfo get_current_point_info(bool point_detected);

    double calculate_adaptive_speed(double current_r, double normative_r);


    std::pair<glm::vec3, PolarPoint> _get_coordinates(double longitude, double theta_regler);
    static double _theta_from_regler(double theta_regler) ;
    double _min_max_normalization(double value) const;
    AnyMeteoroidSize get_next_valid_size(MeteoroidSizeID start_id);
};