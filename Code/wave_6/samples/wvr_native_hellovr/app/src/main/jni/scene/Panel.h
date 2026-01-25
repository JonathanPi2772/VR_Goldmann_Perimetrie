#pragma once
#include <Object.h>
#include <vector>
#include <Texture.h>

class Panel : public Object {
private:
    int mMatrixLocation;
    Vector3 mPosition;
    float mWidth;
    float mHeight;

public:
    // Constructor: Position in world space, Width, Height
    Panel(Vector3 position, float width, float height);
    ~Panel();

    // Loads the image for the menu
    void setTexture(const char* path);

    // Checks if a ray (from controller) hits this panel
    bool intersect(const Vector3& rayOrigin, const Vector3& rayDir);

    // Standard draw function
    virtual void draw(const Matrix4& projection, const Matrix4& eye, const Matrix4& view, const Vector4& lightDir);

private:
    void initPanel();
};