#include "Panel.h"
#include <Context.h>
#include <Shader.h>
#include <VertexArrayObject.h>
#include <GLES3/gl31.h>
#include <cmath>

Panel::Panel(Vector3 position, float width, float height)
        : Object(), mPosition(position), mWidth(width), mHeight(height) {

    mName = "Panel";
    // Reuse existing shaders that handle textures (from Picture/Skybox)
    loadShaderFromAsset("shader/vertex/vt_vertex.glsl", "shader/fragment/t_fragment.glsl");

    if (mHasError) return;

    mEnable = true;
    mMatrixLocation = mShader->getUniformLocation("matrix");
    mVAO = new VertexArrayObject(true, false);

    initPanel();
}

Panel::~Panel() {
    if (mTexture) delete mTexture;
    if (mVAO) delete mVAO;
}

void Panel::setTexture(const char* path) {
    mTexture = Texture::loadTexture(path);
    if (mTexture == NULL) {
        mHasError = true;
        return;
    }

    // --- FIX STARTS HERE ---

    // 1. Bind the texture so we can operate on it
    mTexture->bindTexture();

    // 2. Upload the pixel data to the GPU
    // GL_RGB5_A1 is a standard format used in Picture.cpp;
    // it ensures the image data is actually put into the texture memory.
    mTexture->bindBitmap(GL_RGB5_A1);

    // 3. Set Texture Parameters (Required for rendering)
    // Wrap: Clamp to edge prevents weird artifacts at the border
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Filter: Linear makes it smooth, Nearest makes it pixelated.
    // Without these, OpenGL expects mipmaps (which we don't have), resulting in black.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // 4. Unbind and Clean up CPU memory
    mTexture->unbindTexture();
    mTexture->cleanBitmap(); // Free the RAM copy since it's now on the GPU
}

void Panel::initPanel() {
    if (!mVAO) return;

    // Create a Quad centered at (0,0,0) relative to the object transform
    // We will handle the actual position via the Model Matrix in draw()
    float w = mWidth / 2.0f;
    float h = mHeight / 2.0f;

    // Vertices (X, Y, Z) and Texture Coordinates (U, V)
    // Assuming panel faces +Z or -Z. Let's make it face +Z (towards user if user is at +Z looking -Z)
    // Standard billboard usually faces +Z.
    const float vertices[] = {
            // Triangle 1
            -w,  h, 0.0f,   0.0f, 0.0f, // Top Left
            -w, -h, 0.0f,   0.0f, 1.0f, // Bottom Left
            w, -h, 0.0f,   1.0f, 1.0f, // Bottom Right

            // Triangle 2
            -w,  h, 0.0f,   0.0f, 0.0f, // Top Left
            w, -h, 0.0f,   1.0f, 1.0f, // Bottom Right
            w,  h, 0.0f,   1.0f, 0.0f  // Top Right
    };

    mVAO->bindVAO();
    mVAO->bindArrayBuffer();

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position Attribute (Index 0, 3 floats)
    int stride = 5 * sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (const void*)0);

    // Texture Coord Attribute (Index 1, 2 floats)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, stride, (const void*)(3 * sizeof(float)));

    mVAO->unbindVAO();
    mVAO->unbindArrayBuffer();
}

void Panel::draw(const Matrix4& projection, const Matrix4& eye, const Matrix4& view, const Vector4& lightDir) {
    if (!mEnable || !mTexture || !mVAO) return;

    mShader->useProgram();
    mVAO->bindVAO();

    // 1. Calculate Model Matrix (Translation to mPosition)
    Matrix4 model;
    model.identity();
    model.translate(mPosition.x, mPosition.y, mPosition.z);

    // Optional: If you want the panel to rotate to face the user, add rotation logic here.
    // For now, it is a fixed billboard facing Z.

    // 2. Calculate MVP: Projection * Eye * View * Model
    Matrix4 mvp = projection * eye * view * model;

    // 3. Send to shader
    glUniformMatrix4fv(mMatrixLocation, 1, false, mvp.get());

    // 4. Bind Texture and Draw
    glActiveTexture(GL_TEXTURE0);
    mTexture->bindTexture();

    glDrawArrays(GL_TRIANGLES, 0, 6);

    mTexture->unbindTexture();
    mVAO->unbindVAO();
    mShader->unuseProgram();
}

bool Panel::intersect(const Vector3& rayOrigin, const Vector3& rayDir) {
    if (!mEnable) return false;

    // Simple Ray-Plane intersection for a plane aligned with Z axis
    // Plane Normal is (0, 0, 1)

    // Check if ray is parallel to plane
    if (std::abs(rayDir.z) < 0.0001f) return false;

    // t = (PlaneZ - RayOriginZ) / RayDirZ
    float t = (mPosition.z - rayOrigin.z) / rayDir.z;

    // If t < 0, the panel is behind the ray origin
    if (t < 0) return false;

    // Calculate Hit Point
    Vector3 hitPoint = rayOrigin + (rayDir * t);

    // Check if Hit Point is within Width and Height bounds
    float halfW = mWidth / 2.0f;
    float halfH = mHeight / 2.0f;

    bool inX = (hitPoint.x >= mPosition.x - halfW) && (hitPoint.x <= mPosition.x + halfW);
    bool inY = (hitPoint.y >= mPosition.y - halfH) && (hitPoint.y <= mPosition.y + halfH);

    return inX && inY;
}