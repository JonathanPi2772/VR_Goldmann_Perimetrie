#define LOG_TAG "Stars"
#include "scene/Stars.h"
#include "object/VertexArrayObject.h"
#include "object/Shader.h"
#include "log.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>
#include "Settings.h"
#include <random>
#include <array>
#include <numbers> // Für std::numbers::pi (C++20)



Stars::Stars(bool debug) : Object() {
    mName = LOG_TAG;

    // 1. Lade die Shader, wie es die Object-Klasse vorsieht.
    loadShaderFromAsset("shader/vertex/stars_vertex.glsl", "shader/fragment/stars_fragment.glsl");
    if (mHasError) {
        LOGE("Failed to load shaders for Stars.");
        return;
    }

    // 2. Hole die Speicherorte der Uniform-Variablen aus dem Shader.
    mProjectionMatrixLocation = mShader->getUniformLocation("uProjectionMatrix");
    mModelViewMatrixLocation = mShader->getUniformLocation("uModelViewMatrix");

    // 3. Erstelle das VertexArrayObject, das in der Object-Klasse gespeichert wird.
    mVAO = new VertexArrayObject(true, false);

    // 4. Generiere die Vertex-Daten für die Sterne.
    StarData stars = generateStars(
            NUM_STARS,
            STARS_DISTANCE,
            STAR_SIZE_VARIATION,
            STAR_POS_VARIATION
            );
    mNumStars = static_cast<GLsizei>(stars.positions.size());
    mStarVertices.reserve(mNumStars);
    for (int i = 0; i < mNumStars; ++i) {
        mStarVertices.push_back({
                                        static_cast<float>(stars.positions[i][0]),
                                        static_cast<float>(stars.positions[i][1]),
                                        static_cast<float>(stars.positions[i][2]),
                                        static_cast<float>(stars.sizes[i])
                                });
    }
    // 5. Initialisiere die OpenGL-Buffer mit den Vertex-Daten.
    initVertices();
}

Stars::~Stars() {
    // Der Destruktor der Basisklasse 'Object' kümmert sich um das Löschen
    // von mShader und mVAO, daher ist hier nichts weiter zu tun.
}


/**
 * @brief Generiert gleichmäßig verteilte Sternpositionen und -größen auf der Oberfläche einer Kugel.
 * Verwendet ein Fibonacci-Gitter und fügt zufälliges Rauschen für ein natürliches Aussehen hinzu.
 *
 * @param num_stars Die Anzahl der zu generierenden Sterne.
 * @param distance Der Abstand der Sterne vom Ursprung (Radius der Kugel).
 * @param size_variation Ein Faktor, der die zufällige Abweichung der Position beeinflusst.
 * @param size_multiplier_upper_bound Die Obergrenze für den zufälligen Größenmultiplikator.
 * (In Python "position_noise" genannt, aber für die Größe verwendet).
 * @return Ein StarData-Objekt mit Vektoren für Positionen und Größen.
 */
StarData Stars::generateStars(int num_stars, double distance, double size_variation, double size_multiplier_upper_bound) {
    StarData result;
    // Vorab Speicher reservieren, um wiederholte Reallokationen zu vermeiden (Effizienz)
    result.positions.reserve(num_stars);
    result.sizes.reserve(num_stars);

    const double golden_ratio = (1.0 + std::sqrt(5.0)) / 2.0;

    // In C++20 ist std::numbers::pi der Standard. Für ältere Compiler könnte M_PI aus <cmath>
    // verwendet werden, ist aber nicht Teil des C++-Standards.
    const double base_angle_rad = 0.2 * M_PI / 180.0;

    // C++ Zufallszahlen-Setup (ausführlicher als in Python)
    // 1. Ein "seed" (Samen) von einem echten Zufallsgerät
    std::random_device rd;
    // 2. Die Mersenne-Twister-Engine, initialisiert mit dem Seed
    std::mt19937 gen(rd());
    // 3. Definition der Verteilungen für die Zufallszahlen
    std::uniform_real_distribution<> pos_deviation_dist(-0.05 * size_variation, 0.05 * size_variation);
    std::uniform_real_distribution<> size_factor_dist(0.7, size_multiplier_upper_bound);

    for (int i = 0; i < num_stars; ++i) {
        // Fibonacci-Gitter für die Basisposition
        double theta = 2 * M_PI * i / golden_ratio;
        double phi = std::acos(1 - 2 * (static_cast<double>(i) + 0.5) / num_stars);

        // Kleine zufällige Abweichungen für ein natürlicheres Aussehen hinzufügen
        theta += pos_deviation_dist(gen);
        phi += pos_deviation_dist(gen);

        // Konvertierung von Kugelkoordinaten in kartesische Koordinaten für die Position
        double x = distance * std::sin(phi) * std::cos(theta);
        double y = distance * std::sin(phi) * std::sin(theta);
        double z = distance * std::cos(phi);
        result.positions.push_back({x, y, z});

        // Sterngröße basierend auf einem Basiswinkel und einem Zufallsfaktor berechnen
        double base_radius = std::tan(base_angle_rad / 2.0) * distance;
        double star_radius = base_radius * size_factor_dist(gen);
        result.sizes.push_back(star_radius);
    }

    return result;
}



/* Für die Sterne */
void Stars::initVertices() {
    if (!mVAO || mStarVertices.empty()) {
        return;
    }

    mVAO->bindVAO();
    mVAO->bindArrayBuffer();

    // 1. Lade die kombinierten (interleaved) Daten in den GPU-Buffer.
    glBufferData(GL_ARRAY_BUFFER, mStarVertices.size() * sizeof(StarVertex), mStarVertices.data(), GL_STATIC_DRAW);

    // 2. Definiere das Layout der Vertex-Attribute.

    // Attribut 0: Position (vec3)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0,                                  // location = 0 (im Shader)
            3,                                  // 3 Komponenten (x, y, z)
            GL_FLOAT,                           // Datentyp
            GL_FALSE,                           // Nicht normalisieren
            sizeof(StarVertex),                 // Stride: Abstand zwischen zwei Vertices
            (void*)offsetof(StarVertex, x)      // Offset: Wo beginnt dieses Attribut im Struct?
    );

    // Attribut 1: Größe (float)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
            1,                                  // location = 1 (im Shader)
            1,                                  // 1 Komponente (size)
            GL_FLOAT,                           // Datentyp
            GL_FALSE,                           // Nicht normalisieren
            sizeof(StarVertex),                 // Stride: Abstand zwischen zwei Vertices
            (void*)offsetof(StarVertex, size)   // Offset: Wo beginnt dieses Attribut im Struct?
    );

    mVAO->unbindArrayBuffer();
    mVAO->unbindVAO();
}

void Stars::draw(const Matrix4& projection, const Matrix4& eye, const Matrix4& view, const Vector4& lightDir) {
    if (!mEnable || mHasError || mNumStars == 0) {
        return;
    }

    // View-Matrix für den Himmel anpassen (unverändert)
    Matrix4 viewMatrixForSky = view;
    viewMatrixForSky[12] = 0;
    viewMatrixForSky[13] = 0;
    viewMatrixForSky[14] = 0;
    Matrix4 modelViewMatrix = eye * viewMatrixForSky;

    // Shader aktivieren und Matrizen übergeben (unverändert)
    mShader->useProgram();
    glUniformMatrix4fv(mProjectionMatrixLocation, 1, GL_FALSE, projection.get());
    glUniformMatrix4fv(mModelViewMatrixLocation, 1, GL_FALSE, modelViewMatrix.get());

    // VAO binden
    mVAO->bindVAO();

    // WICHTIG: Erlaube dem Vertex-Shader, die Größe der Punkte zu setzen.
    // glEnable(GL_PROGRAM_POINT_SIZE);

    // Zeichne die Sterne als Punkte.
    glDrawArrays(GL_POINTS, 0, mNumStars);

    // Deaktiviere die Punktgrößen-Einstellung wieder (gute Praxis).
    // glDisable(GL_PROGRAM_POINT_SIZE);

    // VAO unbinden
    mVAO->unbindVAO();

    mShader->unuseProgram();
}

