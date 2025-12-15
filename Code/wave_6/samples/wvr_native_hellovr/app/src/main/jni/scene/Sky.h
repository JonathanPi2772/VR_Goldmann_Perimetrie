#pragma once

#include "object/Object.h"
#include "Settings.h"
#include <vector>
#include <numbers> // Für std::numbers::pi (C++20)
#include <cmath>

struct SkyVertex {
    float x, y, z;
};

class Sky : public Object {
private:
    GLsizei mSkyIndexCount = 0;

    GLint mProjectionMatrixLocation;
    GLint mModelViewMatrixLocation ;

public:
    Sky(bool debug);
    ~Sky();

    void setDebug(bool debug);

private:
    void generateSkySphere(std::vector<SkyVertex>& vertices, std::vector<unsigned int>& indices, float radius, int sectors, int stacks);
    void initVertices();

public:
    virtual void draw(const Matrix4& projection, const Matrix4& eye, const Matrix4& view, const Vector4& lightDir);

};