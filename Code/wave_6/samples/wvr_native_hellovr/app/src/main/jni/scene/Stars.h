#pragma once

#include "object/Object.h"
#include "Settings.h"
#include <vector>
#include <numbers> // Für std::numbers::pi (C++20)
#include <cmath>

// Eine Struktur, um die generierten Daten sauber zurückzugeben.
// Das ist die C++-Entsprechung zur Rückgabe eines Tupels (positions, sizes).

struct StarData {
    std::vector<std::array<double, 3>> positions;
    std::vector<double> sizes;
};
struct StarVertex {
    float x, y, z; // Position
    float size;    // Größe des Sterns
};


class Stars : public Object {
private:
    GLsizei mNumStars;

    GLint mProjectionMatrixLocation;
    GLint mModelViewMatrixLocation ;
    std::vector<StarVertex> mStarVertices;

public:
    Stars(bool debug);
    ~Stars();

    void setDebug(bool debug);

private:
    StarData generateStars(int num_stars, double distance, double size_variation, double size_multiplier_upper_bound);
    void initVertices();

public:
    virtual void draw(const Matrix4& projection, const Matrix4& eye, const Matrix4& view, const Vector4& lightDir);

};

