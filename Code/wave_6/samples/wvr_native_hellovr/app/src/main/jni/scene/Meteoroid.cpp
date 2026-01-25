// Meteoroid.cpp
#include "Meteoroid.h"
#include "Settings.h"
#include "HelperFunctions.h"
#include "Matrices.h"
#include "Vectors.h"
#include "log.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

// GLES Header (wie in Sphere.cpp)
#include <GLES3/gl31.h>
#include <glm/gtc/type_ptr.hpp> // Für glm::value_ptr
#include <math.h>

// --- Implementierung ---

Meteoroid::Meteoroid()
        : Object(), // Konstruktor der Basisklasse aufrufen
          m_perimetry_status("ready"),
          m_radius(METEOROID_DISTANCE),
          m_longitudes_original(METEOROID_LONGITUDES_DEG),
          m_longitudes(METEOROID_LONGITUDES_DEG),
          m_meteoroid_speed(METEOROID_SPEED),
          m_current_longitude_index(0),
          m_passed_seconds(0.0),
          m_paused_passed_seconds(0.0),
          m_rng(std::random_device{}())
{
    mName = "Meteoroid";
    m_sec_per_longitude = 90.0 / m_meteoroid_speed;
    m_R = calc_rotation_matrix(GENERAL_THALES_POINT_VEC, Vector3(0.0f, 0.0f, -m_radius));

    // Standard-Pausenwerte
    m_paused_star_position = glm::vec3(0.0f, 0.0f, -m_radius);
    m_paused_star_p = {0.0, 0.0};

    // Map für einfachen Zugriff auf die Größen-Objekte
    m_size_map = {
            {MeteoroidSizeID::I, std::make_shared<Size_I>()},
            {MeteoroidSizeID::II, std::make_shared<Size_II>()},
            {MeteoroidSizeID::III, std::make_shared<Size_III>()},
            {MeteoroidSizeID::IV, std::make_shared<Size_IV>()},
            {MeteoroidSizeID::V, std::make_shared<Size_V>()}
    };
    m_current_size = m_size_map.at(MeteoroidSizeID::V); // Standard
    m_paused_star_size = m_size_map.at(MeteoroidSizeID::I);

    // --- OpenGL-Initialisierung (parallel zu Sphere.cpp) ---

    // Annahme: Shader-Dateien heißen so
    loadShaderFromAsset("shader/vertex/meteoroid_vertex.glsl", "shader/fragment/meteoroid_fragment.glsl");
    if (mHasError) return;

    // Shader-Handles holen (für die Shader, die Sie in der letzten Antwort erhalten haben)
    mPositionHandle = mShader->getAttributeLocation("aPos");
    mNormalHandle = mShader->getAttributeLocation("aNormal");
    mModelMatrixHandle = mShader->getUniformLocation("model");
    mViewMatrixHandle = mShader->getUniformLocation("view");
    mProjectionMatrixHandle = mShader->getUniformLocation("projection");
    mColorHandle = mShader->getUniformLocation("u_color");
    m_perimetry_status = "Not Started";

    mVAO = new VertexArrayObject(true, false);
    GLenum err = glGetError();
    initMeteoroidSphere();
}

Meteoroid::~Meteoroid() {
    if (mVAO) {
        delete mVAO;
        mVAO = nullptr;
    }
}

// 1:1-Kopie von Sphere::initVertexData
void Meteoroid::initVertexData(std::vector<float>& alVertix) {
    const float r = 0.8f; // Wir verwenden einen Normal-Radius, Skalierung erfolgt im Shader
    const float UNIT_SIZE = 1.0f;
    auto toRadians = [](double vAngle) { return vAngle * M_PI / 180.0; };

    int angleSpan = 10;
    for (int vAngle = -90; vAngle < 90; vAngle = vAngle + angleSpan) {
        for (int hAngle = 0; hAngle <= 360; hAngle = hAngle + angleSpan) {
            float x0 = (float) (r * UNIT_SIZE * cos(toRadians(vAngle)) * cos(toRadians(hAngle)));
            float y0 = (float) (r * UNIT_SIZE * cos(toRadians(vAngle)) * sin(toRadians(hAngle)));
            float z0 = (float) (r * UNIT_SIZE * sin(toRadians(vAngle)));
            float x1 = (float) (r * UNIT_SIZE * cos(toRadians(vAngle)) * cos(toRadians(hAngle + angleSpan)));
            float y1 = (float) (r * UNIT_SIZE * cos(toRadians(vAngle)) * sin(toRadians(hAngle + angleSpan)));
            float z1 = (float) (r * UNIT_SIZE * sin(toRadians(vAngle)));
            float x2 = (float) (r * UNIT_SIZE * cos(toRadians(vAngle + angleSpan)) * cos(toRadians(hAngle + angleSpan)));
            float y2 = (float) (r * UNIT_SIZE * cos(toRadians(vAngle + angleSpan)) * sin(toRadians(hAngle + angleSpan)));
            float z2 = (float) (r * UNIT_SIZE * sin(toRadians(vAngle + angleSpan)));
            float x3 = (float) (r * UNIT_SIZE * cos(toRadians(vAngle + angleSpan)) * cos(toRadians(hAngle)));
            float y3 = (float) (r * UNIT_SIZE * cos(toRadians(vAngle + angleSpan)) * sin(toRadians(hAngle)));
            float z3 = (float) (r * UNIT_SIZE * sin(toRadians(vAngle + angleSpan)));

            alVertix.push_back(x1); alVertix.push_back(y1); alVertix.push_back(z1);
            alVertix.push_back(x3); alVertix.push_back(y3); alVertix.push_back(z3);
            alVertix.push_back(x0); alVertix.push_back(y0); alVertix.push_back(z0);
            alVertix.push_back(x1); alVertix.push_back(y1); alVertix.push_back(z1);
            alVertix.push_back(x2); alVertix.push_back(y2); alVertix.push_back(z2);
            alVertix.push_back(x3); alVertix.push_back(y3); alVertix.push_back(z3);
        }
    }
    vCount = alVertix.size() / 3;
}

// Angepasste Version von Sphere::initSphere
void Meteoroid::initMeteoroidSphere() {
    GLuint offset = 0;
    std::vector<float> alVertix;
    initVertexData(alVertix);

    // Wir verwenden die Vertices auch als Normalen
    GLfloat normals[vCount * 3];
    for (int i = 0; i < alVertix.size(); i++) {
        normals[i] = alVertix[i];
    }

    if (!mVAO) return;
    mVAO->bindVAO();

    // 1. Daten verschachteln (interleave)
    std::vector<float> interleavedData;
    interleavedData.resize(vCount * 6); // 3 für Pos, 3 für Normal
    for(int i = 0; i < vCount; ++i) {
        // Position
        interleavedData[i * 6 + 0] = alVertix[i * 3 + 0];
        interleavedData[i * 6 + 1] = alVertix[i * 3 + 1];
        interleavedData[i * 6 + 2] = alVertix[i * 3 + 2];
        // Normale (da sie identisch sind)
        interleavedData[i * 6 + 3] = alVertix[i * 3 + 0];
        interleavedData[i * 6 + 4] = alVertix[i * 3 + 1];
        interleavedData[i * 6 + 5] = alVertix[i * 3 + 2];
    }

    // 2. Buffer binden und füllen
    mVAO->bindArrayBuffer();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * interleavedData.size(), &interleavedData[0], GL_STATIC_DRAW);
    GLenum err = glGetError();

    // 3. Pointer setzen
    GLsizei stride = 6 * sizeof(float); // 6 Floats pro Vertex (3 Pos + 3 Normal)

    // Position (Offset 0)
    glVertexAttribPointer(mPositionHandle, 3, GL_FLOAT, false, stride, (const void*)0);
    glEnableVertexAttribArray(mPositionHandle);
    err = glGetError();

    // Normale (Offset 3 * sizeof(float))
    const void* normalOffset = (const void*)(3 * sizeof(float));
    glVertexAttribPointer(mNormalHandle, 3, GL_FLOAT, false, stride, normalOffset);
    err = glGetError();
    glEnableVertexAttribArray(mNormalHandle);
    err = glGetError();

    mVAO->unbindVAO();
}


void Meteoroid::draw(const Matrix4& projection, const Matrix4& eye, const Matrix4& view, const Vector4& lightDir) {
    if (!mEnable || mHasError || !mVAO) {
        return;
    }

    // 1. Logik-Update (bleibt in glm)
    CurrentPointInfo info = get_current_point_info();
    if (!info.is_visible) {
        return;
    }

    // 2. Skalierungsfaktor berechnen (bleibt in glm)
    double area = info.size->get_size_meter_sq();
    float radius_m = static_cast<float>(std::sqrt(area / M_PI));
    float scale_factor = radius_m / 0.8f; // 0.8f = Basis-Radius aus initVertexData

    // 3. Model-Matrix mit GLM bauen (unsere interne Logik)
    glm::mat4 model_matrix_glm = glm::mat4(1.0f);
    model_matrix_glm = glm::translate(model_matrix_glm, info.position); // info.position ist glm::vec3
    model_matrix_glm = glm::scale(model_matrix_glm, glm::vec3(scale_factor, scale_factor, scale_factor));

    // 4. *** KONVERTIERUNG (Der wichtige Schritt) ***
    // Konvertiere glm::mat4 in ein C-Array (float[16])
    float model_matrix_c_array[16];
    memcpy(model_matrix_c_array, glm::value_ptr(model_matrix_glm), sizeof(float) * 16);

    // Erstelle die Engine-spezifische Matrix4 aus dem C-Array.
    // Wir nehmen an, dass Matrix4 einen Konstruktor hat, der float* akzeptiert,
    // basierend auf Sphere.cpp's `Matrix4(currMatrix)`.
    Matrix4 model_matrix_engine(model_matrix_c_array);

    // 5. Matrizenberechnung mit den Engine-Typen
    // (jetzt, wo alles der gleiche Typ ist, funktioniert der * Operator)
    // Matrix4 modelview = eye * view * model_matrix_engine;
    // Matrix4 modelview_projection = projection * modelview;
    Matrix4 final_view_matrix = eye * view;

    // 6. Shader & VAO binden
    mShader->useProgram();
    mVAO->bindVAO();

    // 7. Uniforms senden
    // Wir verwenden die .get() Methode, die in Sphere.cpp für
    // die Übergabe an glUniformMatrix4fv genutzt wird.
    //glUniformMatrix4fv(mModelMatrixHandle, 1, GL_FALSE, model_matrix_engine.get());
    //glUniformMatrix4fv(mModelviewProjectionLocation, 1, GL_FALSE, modelview_projection.get());
    glUniformMatrix4fv(mModelMatrixHandle, 1, GL_FALSE, model_matrix_engine.get());
    glUniformMatrix4fv(mViewMatrixHandle, 1, GL_FALSE, final_view_matrix.get());
    glUniformMatrix4fv(mProjectionMatrixHandle, 1, GL_FALSE, projection.get());
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGE("glGetError() in draw(): %d", err);
    }


    // 8. Farbe senden
    float target_db = TARGET_LUMINANCE_DB;
    float color_value = static_cast<float>(std::pow(10.0, target_db / 10.0));
    glUniform3f(mColorHandle,
                color_value,
                color_value,
                color_value);

    // 9. Zeichnen
    glDrawArrays(GL_TRIANGLES, 0, vCount);

    mShader->unuseProgram();
    mVAO->unbindVAO();
}


// --- Logik-Funktionen (übersetzt aus meteoroid.py) ---

void Meteoroid::start_animation() {
    if (METEOROID_RANDOM) {
        std::shuffle(m_longitudes.begin(), m_longitudes.end(), m_rng);
    }
    m_current_longitude_start_time = std::chrono::high_resolution_clock::now();
    m_current_longitude_index = 0;

    // Finde die erste gültige Größe
    m_current_size = get_next_valid_size(MeteoroidSizeID::V);
    if (m_current_size == nullptr) {
        mHasError = true;
        return;
        "Perimetry can not be started with no Size in settings set to true";
    }
    m_current_size->set_distance(m_radius);

    m_perimetry_status = "running";
}

void Meteoroid::pause_animation() {
    if (m_perimetry_status == "running") {
        CurrentPointInfo info = get_current_point_info();
        m_paused_star_position = info.position;
        m_paused_star_size = info.size;
        m_paused_star_p = info.p;

        double elapsed_total = m_passed_seconds;
        int longitude_index = m_current_longitude_index + static_cast<int>(elapsed_total / m_sec_per_longitude);
        double sec_in_longitude = std::fmod(elapsed_total, m_sec_per_longitude);

        m_current_longitude_index = longitude_index;
        m_paused_passed_seconds = sec_in_longitude;
        m_perimetry_status = "paused";
    }
}

void Meteoroid::resume_animation() {
    if (m_perimetry_status == "paused") {
        auto now = std::chrono::high_resolution_clock::now();
        auto new_start_time = now - std::chrono::duration<double>(m_paused_passed_seconds);
        m_current_longitude_start_time = std::chrono::time_point_cast<
                std::chrono::high_resolution_clock::duration>(new_start_time);

        m_paused_star_position = glm::vec3(0.0f, 0.0f, -m_radius);
        m_paused_star_size = m_size_map.at(MeteoroidSizeID::I); // Standard
        m_paused_star_p = {0.0, 0.0};
        m_perimetry_status = "running";
    }
}

void Meteoroid::reset_animation() {
    m_current_longitude_start_time = std::chrono::time_point<std::chrono::high_resolution_clock>();
    m_current_longitude_index = 0;
    m_passed_seconds = 0.0;
    m_perimetry_status = "Not Started";
}

void Meteoroid::star_position_changed(const Vector3& star_position) {
    m_R = calc_rotation_matrix(GENERAL_THALES_POINT_VEC, star_position);
}

// Helfer zum Finden der nächsten zu testenden Größe
std::shared_ptr<MeteoroideSize> Meteoroid::get_next_valid_size(MeteoroidSizeID start_id) {
    MeteoroidSizeID current_id = start_id;
    while (current_id != MeteoroidSizeID::None) {
        // Prüfen, ob die Größe in den Settings auf 'true' steht
        if (METEOROID_SIZES.count(current_id) && METEOROID_SIZES.at(current_id)) {
            return m_size_map.at(current_id);
        }
        // Nächste Größe holen und Schleife fortsetzen
        current_id = m_size_map.at(current_id)->get_next_size_id();
    }
    return nullptr; // Keine gültige Größe mehr gefunden
}


Meteoroid::CurrentPointInfo Meteoroid::get_current_point_info() {
    if (m_perimetry_status == "paused") {
        return {true, m_paused_star_position, m_paused_star_size, m_paused_star_p};
    }
    if (m_perimetry_status != "running") {
        return {false, {}, nullptr, {}};
    }

    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = now - m_current_longitude_start_time;
    m_passed_seconds = elapsed.count();

    int longitude_index = m_current_longitude_index + static_cast<int>(m_passed_seconds / m_sec_per_longitude);

    if (longitude_index < m_longitudes.size()) {
        double sec_in_longitude = std::fmod(m_passed_seconds, m_sec_per_longitude);
        double deg = sec_in_longitude * m_meteoroid_speed;
        double theta_regler = (deg / 90.0);

        auto [light_point, p] = _get_coordinates(m_longitudes[longitude_index], theta_regler);
        return {true, light_point, m_current_size, p};
    } else {
        // Min. eine Größe fertig. Finde die nächste.
        MeteoroidSizeID next_id = m_current_size->get_next_size_id();
        std::shared_ptr<MeteoroideSize> next_size = get_next_valid_size(next_id);

        if (next_size != nullptr) {
            m_current_size = next_size;
            m_current_size->set_distance(m_radius);

            std::vector<double> next_longitudes = m_longitudes_original;
            if (METEOROID_RANDOM) {
                std::shuffle(next_longitudes.begin(), next_longitudes.end(), m_rng);
            }
            m_longitudes.insert(m_longitudes.end(), next_longitudes.begin(), next_longitudes.end());

            return get_current_point_info(); // Rekursiver Aufruf für den neuen Zustand
        } else {
            // Alle Größen fertig
            m_perimetry_status = "Done";
            return {false, {}, nullptr, {}};
        }
    }
}

void Meteoroid::point_detected() {
    pause_animation();
    if (m_perimetry_status != "paused") return;

    m_goldmann_sheet.add_point(m_paused_star_p, m_paused_star_size);

    /* Original Code
    // Nächste Longitude starten
    resume_animation();
    m_current_longitude_index++;
    m_current_longitude_start_time = std::chrono::high_resolution_clock::now();
     */
    // New Code
    m_current_longitude_index++;

    // 4. Reset time for the NEW animation path
    m_current_longitude_start_time = std::chrono::high_resolution_clock::now();
    m_passed_seconds = 0.0;

    // 5. Set status to running (Manually, instead of calling resume_animation)
    m_perimetry_status = "running";

    // Optional: Log it
    LOGI("Point detected! Moving to longitude index: %d", m_current_longitude_index);
}

// --- Private Python-Helfer, jetzt in C++ ---

double Meteoroid::_theta_from_regler(double theta_regler) {
    return theta_regler * (M_PI / 2.0) + (M_PI / 2.0);
}

double Meteoroid::_min_max_normalization(double value) const {
    return 2.0 * ((value - (-m_radius)) / (m_radius - (-m_radius))) - 1.0;
}

std::pair<glm::vec3, PolarPoint> Meteoroid::_get_coordinates(double longitude, double theta_regler) {
    double theta = _theta_from_regler(theta_regler);
    double phi = glm::radians(longitude);

    auto x = static_cast<float>(std::sin(theta) * std::cos(phi) * m_radius);
    auto y = static_cast<float>(std::sin(theta) * std::sin(phi) * m_radius);
    auto z = static_cast<float>(std::cos(theta) * m_radius);

    glm::vec3 light_point_raw(x, y, z);
    glm::vec3 p_vec = light_point_raw - GENERAL_THALES_POINT;

    auto theta_p = static_cast<float>(_min_max_normalization(p_vec.y) * 90.0);
    auto phi_t = static_cast<float>(_min_max_normalization(p_vec.x) * 90.0);
    PolarPoint p = {theta_p, phi_t};

    // Wende die Rotationsmatrix an (alles bleibt in GLM)
    // m_R ist glm::mat4, light_point_raw ist glm::vec3
    glm::vec4 transformed_point_v4 = m_R * glm::vec4(light_point_raw, 1.0f);
    // Konvertiere v4 -> v3
    glm::vec3 light_point = glm::vec3(transformed_point_v4);

    return {light_point, p};
}