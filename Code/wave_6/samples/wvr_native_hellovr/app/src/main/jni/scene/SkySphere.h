#pragma once
#include <Object.h>
#include <vector>
#include <Texture.h>
#include <cmath>

class SkySphere : public Object {
private:
    int mMatrixLocation;
    int mVertexCount; // We need to keep track of how many vertices we generated

public:
    // Constructor
    SkySphere();
    ~SkySphere();

    // Loads the 360 image (equirectangular)
    void setTexture(const char* path);

    // Draw function
    virtual void draw(const Matrix4& projection, const Matrix4& eye, const Matrix4& view, const Vector4& lightDir);

private:
    void initSphere();
};