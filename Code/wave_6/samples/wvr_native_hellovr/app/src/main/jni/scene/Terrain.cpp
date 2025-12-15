#define LOG_TAG "Terrain"
#include "scene/Terrain.h" // (Passe den Pfad an, z.B. "scene/Terrain.h")
#include "object/VertexArrayObject.h"
#include "object/Shader.h"
#include "log.h"
#include "Matrices.h"
#include <vector>
#include <cmath>
#include <array>
#include "Settings.h"

/**
 * @struct TerrainVertex
 * @brief Definiert das Datenlayout für einen einzelnen Vertex des Terrains.
 * Enthält Position und Normale (für die Beleuchtung).
 */
struct TerrainVertex {
    float x, y, z;      // Position
    float nx, ny, nz;   // Normale
};


Terrain::Terrain(bool debug) : Object() {
    mName = LOG_TAG;
    mIndexCount = 0;

    // 1. Shader laden
    loadShaderFromAsset("shader/vertex/terrain_vertex.glsl", "shader/fragment/terrain_fragment.glsl");
    if (mHasError) {
        LOGE("Failed to load shaders for Terrain.");
        return;
    }

    // 2. Uniform-Speicherorte holen
    mProjectionMatrixLocation = mShader->getUniformLocation("uProjectionMatrix");
    mModelViewMatrixLocation = mShader->getUniformLocation("uModelViewMatrix");
    mNormalMatrixLocation = mShader->getUniformLocation("uNormalMatrix");
    mLightDirViewLocation = mShader->getUniformLocation("uLightDirView");
    mTerrainColorLocation = mShader->getUniformLocation("uTerrainColor");

    // 3. VAO erstellen (hat VBO und EAB)
    mVAO = new VertexArrayObject(true, true);

    // 4. Vertex-Daten generieren und in GPU-Buffer laden
    initVertices();
}

Terrain::~Terrain() {
    // Basisklasse kümmert sich um mShader und mVAO
}

/**
 * @brief Portierung der Python-Funktion _generate_mesh
 */
void Terrain::generateTerrainMesh(
        std::vector<TerrainVertex>& vertices,
        std::vector<unsigned int>& indices,
        float extent, int resolution, float peak, float sigma, float summit_y)
{
    // Basis-Y-Level berechnen
    float base_y = summit_y - peak;

    // Linspace-Äquivalent für X- und Z-Koordinaten
    std::vector<float> xs(resolution);
    std::vector<float> zs(resolution);
    float step = 2.0f * extent / (resolution - 1);
    float dx = step;
    float dz = step;

    for (int i = 0; i < resolution; ++i) {
        xs[i] = -extent + i * step;
        zs[i] = -extent + i * step;
    }

    // 1. Höhen berechnen (Gaußsche Kurve)
    std::vector<std::vector<float>> heights(resolution, std::vector<float>(resolution));
    for (int i = 0; i < resolution; ++i) {
        for (int j = 0; j < resolution; ++j) {
            float x = xs[i];
            float z = zs[j];
            float r_squared = x * x + z * z;
            float g = peak * std::exp(-r_squared / (2.0f * sigma * sigma));
            heights[i][j] = base_y + g;
        }
    }

    // 2. Normalen berechnen (Central Differences)
    std::vector<std::vector<std::array<float, 3>>> normals(resolution, std::vector<std::array<float, 3>>(resolution));
    for (int i = 0; i < resolution; ++i) {
        for (int j = 0; j < resolution; ++j) {
            float hL = (i > 0) ? heights[i - 1][j] : heights[i][j];
            float hR = (i < resolution - 1) ? heights[i + 1][j] : heights[i][j];
            float hD = (j > 0) ? heights[i][j - 1] : heights[i][j];
            float hU = (j < resolution - 1) ? heights[i][j + 1] : heights[i][j];

            float dhdx = (hR - hL) / (2.0f * dx);
            float dhdz = (hU - hD) / (2.0f * dz);

            // Normale (unnormalisiert)
            std::array<float, 3> n = {-dhdx, 1.0f, -dhdz};

            // Normalisieren
            float len = std::sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
            if (len > 0.0f) {
                n[0] /= len;
                n[1] /= len;
                n[2] /= len;
            }
            normals[i][j] = n;
        }
    }

    // 3. Vertices und Normalen in flachen Vektor füllen
    vertices.reserve(resolution * resolution);
    for (int i = 0; i < resolution; ++i) {
        for (int j = 0; j < resolution; ++j) {
            float x = xs[i];
            float y = heights[i][j];
            float z = zs[j];
            auto& n = normals[i][j];
            vertices.push_back({x, y, z, n[0], n[1], n[2]});
        }
    }

    // 4. Indices generieren
    indices.reserve((resolution - 1) * (resolution - 1) * 6);
    auto get_index = [resolution](int i, int j) {
        return i * resolution + j;
    };

    for (int i = 0; i < resolution - 1; ++i) {
        for (int j = 0; j < resolution - 1; ++j) {
            unsigned int a = get_index(i, j);
            unsigned int b = get_index(i + 1, j);
            unsigned int c = get_index(i + 1, j + 1);
            unsigned int d = get_index(i, j + 1);

            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(c);

            indices.push_back(a);
            indices.push_back(c);
            indices.push_back(d);
        }
    }
}

void Terrain::initVertices() {
    if (!mVAO) return;

    std::vector<TerrainVertex> vertices;
    std::vector<unsigned int> indices;

    // Mesh generieren
    generateTerrainMesh(
            vertices,
            indices,
            MOUNTAIN_EXTENT,
            MOUNTAIN_RESOLUTION,
            PEAK_AMPLITUDE,
            SIGMA,
            SUMMIT_Y
    );

    if (vertices.empty() || indices.empty()) {
        LOGE("Terrain mesh generation failed or resulted in empty buffers.");
        mHasError = true;
        return;
    }

    mIndexCount = static_cast<GLsizei>(indices.size());

    mVAO->bindVAO();

    // Vertex-Buffer
    mVAO->bindArrayBuffer();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TerrainVertex), vertices.data(), GL_STATIC_DRAW);

    // Index-Buffer
    mVAO->bindElementArrayBuffer();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Vertex-Attribute definieren
    // Attribut 0: Position (vec3)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0,                                  // location = 0
            3,                                  // 3 Komponenten (x, y, z)
            GL_FLOAT,                           // Typ
            GL_FALSE,                           // Nicht normalisieren
            sizeof(TerrainVertex),              // Stride
            (void*)offsetof(TerrainVertex, x)   // Offset
    );

    // Attribut 1: Normale (vec3)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
            1,                                  // location = 1
            3,                                  // 3 Komponenten (nx, ny, nz)
            GL_FLOAT,                           // Typ
            GL_FALSE,                           // Nicht normalisieren (sind schon normalisiert)
            sizeof(TerrainVertex),              // Stride
            (void*)offsetof(TerrainVertex, nx)  // Offset
    );

    mVAO->unbindVAO();
}

void Terrain::draw(const Matrix4& projection, const Matrix4& eye, const Matrix4& view, const Vector4& lightDir) {
    if (!mEnable || mHasError || mIndexCount == 0) {
        return;
    }

    // Model-View-Matrix (Terrain ist Teil der Szene, also volle Transformation)
    Matrix4 modelViewMatrix = eye * view;

    // Normalen-Matrix (zum Transformieren der Normalen in den View-Space)
    // (Inverse Transponierte der oberen 3x3-ModelView-Matrix)
    Matrix3 normalMatrix(
            modelViewMatrix[0], modelViewMatrix[1], modelViewMatrix[2],  // 1. Spalte
            modelViewMatrix[4], modelViewMatrix[5], modelViewMatrix[6],  // 2. Spalte
            modelViewMatrix[8], modelViewMatrix[9], modelViewMatrix[10]  // 3. Spalte
    );

    // Berechne die Normalen-Matrix (Inverse Transponierte)
    normalMatrix.invert();
    normalMatrix.transpose();

    // Lichtrichtung in den View-Space transformieren (nur Rotation anwenden)
    Vector4 lightDirView = modelViewMatrix * lightDir;

    // Shader aktivieren
    mShader->useProgram();

    // Uniforms setzen
    glUniformMatrix4fv(mProjectionMatrixLocation, 1, GL_FALSE, projection.get());
    glUniformMatrix4fv(mModelViewMatrixLocation, 1, GL_FALSE, modelViewMatrix.get());
    glUniformMatrix3fv(mNormalMatrixLocation, 1, GL_FALSE, normalMatrix.get());

    glUniform3f(mLightDirViewLocation, lightDirView.x, lightDirView.y, lightDirView.z);
    glUniform3f(mTerrainColorLocation, MOUNTAIN_COLOR[0], MOUNTAIN_COLOR[1], MOUNTAIN_COLOR[2]);

    // Backface Culling aktivieren (Standard für undurchsichtige Objekte)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // (Standard, aber explizit ist sicher)

    // Zeichnen
    mVAO->bindVAO();
    glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, 0);
    mVAO->unbindVAO();

    // Culling muss nicht deaktiviert werden, da Sky es für sich selbst regelt.

    mShader->unuseProgram();
}