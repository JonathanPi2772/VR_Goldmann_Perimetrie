#define LOG_TAG "Sky"
#include "scene/Sky.h"
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



Sky::Sky(bool debug) : Object() {
    mName = LOG_TAG;

    // 1. Lade die Shader, wie es die Object-Klasse vorsieht.
    loadShaderFromAsset("shader/vertex/sky_vertex.glsl", "shader/fragment/sky_fragment.glsl");
    if (mHasError) {
        LOGE("Failed to load shaders for Sky.");
        return;
    }

    // 2. Hole die Speicherorte der Uniform-Variablen aus dem Shader.
    mProjectionMatrixLocation = mShader->getUniformLocation("uProjectionMatrix");
    mModelViewMatrixLocation = mShader->getUniformLocation("uModelViewMatrix");


    // 3. Erstelle das VertexArrayObject, das in der Object-Klasse gespeichert wird.
    mVAO = new VertexArrayObject(true, true); // Maybe set hasEAB to false
    initVertices();
}

Sky::~Sky() {
    // Der Destruktor der Basisklasse 'Object' kümmert sich um das Löschen
    // von mShader und mVAO, daher ist hier nichts weiter zu tun.
}

/**
 * @brief Generiert die Vertices und Indices für eine Kugel.
 * Die Indices werden so gewickelt ("winding order"), dass die Kugel von innen sichtbar ist.
 */
void Sky::generateSkySphere(std::vector<SkyVertex>& vertices, std::vector<unsigned int>& indices, float radius, int sectors, int stacks) {
    const float PI = std::acos(-1.0f);
    float sectorStep = 2 * PI / sectors;
    float stackStep = PI / stacks;

    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = PI / 2 - i * stackStep;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * sectorStep;
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            vertices.push_back({x, y, z});
        }
    }

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < sectors; ++j) {
            int first = i * (sectors + 1) + j;
            int second = first + sectors + 1;

            // Winding order ist hier umgekehrt (z.B. 1-3-2 statt 1-2-3),
            // damit die Dreiecke von innen sichtbar sind (GL_CW statt GL_CCW).
            indices.push_back(first);
            indices.push_back(first + 1);
            indices.push_back(second);

            indices.push_back(second);
            indices.push_back(first + 1);
            indices.push_back(second + 1);
        }
    }
}

void Sky::initVertices() {
    if (!mVAO) return;

    std::vector<SkyVertex> vertices;
    std::vector<unsigned int> indices;
    // Erzeugt eine große Kugel, die garantiert hinter den Sternen liegt.
    // Die Werte 36/24 entsprechen denen aus Ihrem Python-Beispiel.
    generateSkySphere(vertices, indices, STARS_DISTANCE + 100.0f, 36, 24);
    mSkyIndexCount = static_cast<GLsizei>(indices.size());

    mVAO->bindVAO();
    mVAO->bindArrayBuffer();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SkyVertex), vertices.data(), GL_STATIC_DRAW);

    mVAO->bindElementArrayBuffer(); // Maybe bind Element_Array_Buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Vertex-Attribut für die Position (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SkyVertex), (void*)0);

    mVAO->unbindVAO();
}


void Sky::draw(const Matrix4& projection, const Matrix4& eye, const Matrix4& view, const Vector4& lightDir) {
    if (!mEnable || mHasError) {
        return;
    }

    // View-Matrix für den Himmel anpassen (gilt für Kugel und Sterne)
    Matrix4 viewMatrixForSky = view;
    viewMatrixForSky[12] = 0;
    viewMatrixForSky[13] = 0;
    viewMatrixForSky[14] = 0;
    Matrix4 modelViewMatrix = eye * viewMatrixForSky;

    // === 1. Himmelskugel zeichnen ===
    mShader->useProgram();
    glUniformMatrix4fv(mProjectionMatrixLocation, 1, GL_FALSE, projection.get());
    glUniformMatrix4fv(mModelViewMatrixLocation, 1, GL_FALSE, modelViewMatrix.get());

    // Deaktiviere Culling, um sicherzustellen, dass die Innenseite gezeichnet wird.
    glDisable(GL_CULL_FACE);

    mVAO->bindVAO();
    glDrawElements(GL_TRIANGLES, mSkyIndexCount, GL_UNSIGNED_INT, 0);
    mVAO->unbindVAO();

    // Culling wieder aktivieren für andere Objekte in der Szene.
    glEnable(GL_CULL_FACE);

    // Shader-Programm am Ende deaktivieren
    mShader->unuseProgram();
}

