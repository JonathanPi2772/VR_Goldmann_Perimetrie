#pragma once

#include "object/Object.h" // (Stellt sicher, dass du den richtigen Pfad hast)
#include "Matrices.h"
// #include <math/Matrix4.h> // (Stellt sicher, dass du den richtigen Pfad hast)
// #include <math/Vector4.h> // (Stellt sicher, dass du den richtigen Pfad hast)
# include "cmath"
#include <vector>

/**
 * @class Terrain
 * @brief Verwaltet die Erzeugung und das Rendern eines prozeduralen Berg-Terrains.
 *
 * Diese Klasse generiert ein Gitternetz (Mesh) basierend auf einer Gaußschen Kurve
 * und rendert es mit Beleuchtung (diffuses Licht). Sie folgt der Struktur der
 * Object-Basisklasse, ähnlich wie Sky und Stars.
 */
class Terrain : public Object {
public:
    /**
     * @brief Konstruktor für das Terrain.
     * @param debug (Nicht verwendet, aber zur Konsistenz mit Sky/Stars beibehalten)
     */
    Terrain(bool debug);

    /**
     * @brief Virtueller Destruktor.
     */
    virtual ~Terrain();

    /**
     * @brief Zeichnet das Terrain.
     *
     * @param projection Die Projektionsmatrix.
     * @param eye Die Augen-Transformationsmatrix (Inverse der HMD-Basis).
     * @param view Die View-Matrix (Kameratransformation, in VR oft mHMDPose).
     * @param lightDir Die Richtung der globalen Lichtquelle (im Weltkoordinatensystem).
     */
    virtual void draw(const Matrix4& projection, const Matrix4& eye, const Matrix4& view, const Vector4& lightDir) override;

private:
    /**
     * @brief Initialisiert die Vertex-Buffer (VBO, EAB) und das Vertex-Array-Objekt (VAO).
     */
    void initVertices();

    /**
     * @brief Generiert das Terrain-Mesh (Vertices, Normalen, Indices).
     *
     * @param vertices Ein Vektor, der mit den Vertex-Daten gefüllt wird.
     * @param indices Ein Vektor, der mit den Index-Daten gefüllt wird.
     * @param extent Die halbe Ausdehnung des Terrains in X/Z-Richtung.
     * @param resolution Die Anzahl der Vertices pro Achse.
     * @param peak Die Amplitude der Gaußschen Kurve (Höhe des Gipfels).
     * @param sigma Die Standardabweichung (Breite) der Gaußschen Kurve.
     * @param summit_y Die Y-Koordinate, auf der sich der Gipfel befinden soll.
     */
    void generateTerrainMesh(
            std::vector<struct TerrainVertex>& vertices,
            std::vector<unsigned int>& indices,
            float extent, int resolution, float peak, float sigma, float summit_y
    );

    // Speicherorte für Shader-Uniforms
    GLint mProjectionMatrixLocation;
    GLint mModelViewMatrixLocation;
    GLint mNormalMatrixLocation;
    GLint mLightDirViewLocation;
    GLint mTerrainColorLocation;

    // Anzahl der zu zeichnenden Indices
    GLsizei mIndexCount;
};