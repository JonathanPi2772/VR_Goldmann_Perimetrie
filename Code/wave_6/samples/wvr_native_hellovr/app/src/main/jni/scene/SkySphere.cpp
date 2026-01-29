#include "SkySphere.h"
#include <Context.h>
#include <Shader.h>
#include <VertexArrayObject.h>
#include <GLES3/gl31.h>

// Constants for sphere resolution
const int STACKS = 40; // Horizontal slices (latitude)
const int SLICES = 40; // Vertical slices (longitude)
const float RADIUS = 50.0f; // Size of the sphere

SkySphere::SkySphere() : Object(), mVertexCount(0) {
    mName = "SkySphere";

    // Reuse the same shaders as Panel. 
    // Basic texture shaders work perfectly for Skyboxes if UVs are correct.
    loadShaderFromAsset("shader/vertex/vt_vertex.glsl", "shader/fragment/t_fragment.glsl");

    if (mHasError) return;

    mEnable = true;
    mMatrixLocation = mShader->getUniformLocation("matrix");
    mVAO = new VertexArrayObject(true, false);

    initSphere();
}

SkySphere::~SkySphere() {
    if (mTexture) delete mTexture;
    if (mVAO) delete mVAO;
}

void SkySphere::setTexture(const char* path) {
    // Exact same logic as your FIXED Panel class
    mTexture = Texture::loadTexture(path);
    if (mTexture == NULL) {
        mHasError = true;
        return;
    }

    mTexture->bindTexture();
    mTexture->bindBitmap(GL_RGB5_A1);

    // Essential for 360 images: Clamp to Edge to avoid seam lines at the back
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Linear filtering for smooth nature details
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    mTexture->unbindTexture();
    mTexture->cleanBitmap();
}

void SkySphere::initSphere() {
    if (!mVAO) return;

    std::vector<float> vertexData;

    // Generate vertices for a sphere
    for (int i = 0; i < STACKS; ++i) {
        for (int j = 0; j < SLICES; ++j) {

            // We create 2 triangles (quad) for every sector of the sphere
            // Logic: Calculate corners of the current "rectangle" on the sphere surface

            float phi1 = (float)M_PI * (float)i / STACKS;
            float phi2 = (float)M_PI * (float)(i + 1) / STACKS;
            float theta1 = 2.0f * (float)M_PI * (float)j / SLICES;
            float theta2 = 2.0f * (float)M_PI * (float)(j + 1) / SLICES;

            // Helper lambda to push a single vertex
            auto pushVertex = [&](float phi, float theta) {
                // Position (Spherical to Cartesian)
                // Note: We swap Y and Z or adjust depending on your engine's up-axis.
                // Assuming Y is UP based on Panel.cpp
                float x = RADIUS * std::sin(phi) * std::cos(theta);
                float y = RADIUS * std::cos(phi);
                float z = RADIUS * std::sin(phi) * std::sin(theta);

                vertexData.push_back(x);
                vertexData.push_back(y);
                vertexData.push_back(z);

                // Texture Coordinates (Equirectangular mapping)
                // u = theta / 2pi, v = phi / pi
                float u = theta / (2.0f * (float)M_PI);
                float v = phi / (float)M_PI;

                vertexData.push_back(1.0f - u); // Flip U usually required for inside viewing
                vertexData.push_back(v);
            };

            // Triangle 1
            pushVertex(phi1, theta1); // Top Left
            pushVertex(phi2, theta1); // Bottom Left
            pushVertex(phi2, theta2); // Bottom Right

            // Triangle 2
            pushVertex(phi1, theta1); // Top Left
            pushVertex(phi2, theta2); // Bottom Right
            pushVertex(phi1, theta2); // Top Right
        }
    }

    mVertexCount = vertexData.size() / 5; // 5 floats per vertex (x,y,z,u,v)

    mVAO->bindVAO();
    mVAO->bindArrayBuffer();

    // Upload data
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    int stride = 5 * sizeof(float);
    // Position (0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (const void*)0);

    // UV (1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, stride, (const void*)(3 * sizeof(float)));

    mVAO->unbindVAO();
    mVAO->unbindArrayBuffer();
}

void SkySphere::draw(const Matrix4& projection, const Matrix4& eye, const Matrix4& view, const Vector4& lightDir) {
    if (!mEnable || !mTexture || !mVAO) return;

    mShader->useProgram();

    // --- SPECIAL SKYBOX RENDERING STATES ---
    // 1. Disable Culling: We are INSIDE the sphere, so we see the "back" of the triangles.
    glDisable(GL_CULL_FACE);

    // 2. Disable Depth Write: Skybox should be the "farthest" thing. 
    // We draw it, but don't update the depth buffer so other objects draw on top.
    glDepthMask(GL_FALSE);

    // 3. Skybox View Logic: 
    // The skybox should not move when the player walks (translation), 
    // but it MUST rotate when the player looks around (rotation).
    Matrix4 skyView = view;

    // Zero out the translation components of the view matrix
    // (Assuming Column-Major 4x4 matrix, indices 12, 13, 14 are translation)
    float* m = const_cast<float*>(skyView.get());
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;

    // MVP Calculation
    // We do NOT multiply by a Model matrix (or we treat Model as Identity at 0,0,0)
    Matrix4 mvp = projection * eye * skyView;

    glUniformMatrix4fv(mMatrixLocation, 1, false, mvp.get());

    mVAO->bindVAO();
    glActiveTexture(GL_TEXTURE0);
    mTexture->bindTexture();

    // Draw all triangles
    glDrawArrays(GL_TRIANGLES, 0, mVertexCount);

    mTexture->unbindTexture();
    mVAO->unbindVAO();
    mShader->unuseProgram();

    // --- RESTORE STATES ---
    glDepthMask(GL_TRUE); // Re-enable depth writing
    glEnable(GL_CULL_FACE); // Re-enable culling
}