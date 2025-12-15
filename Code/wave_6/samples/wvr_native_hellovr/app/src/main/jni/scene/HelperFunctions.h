// HelperFunctions.h
#pragma once

#include <Matrices.h>
#include <Vectors.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp> // Für glm::length2


inline glm::mat4 calc_rotation_matrix(Vector3 v_a, Vector3 v_b, glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f)) {
    // Vektoren normalisieren (normalize() in Vectors.h modifiziert das Objekt direkt)
    glm::vec3 a = glm::vec3(v_a.x, v_a.y, v_a.z);
    glm::vec3 b = glm::vec3(v_b.x, v_b.y, v_b.z);
    a = glm::normalize(a);
    b = glm::normalize(b);
    world_up = glm::normalize(world_up);

    // --- Orientierungsmatrix für A ---
    glm::vec3 z_axis_a = a;
    glm::vec3 x_axis_a = glm::cross(world_up, z_axis_a); //

    if (glm::length2(x_axis_a) < 1e-6f) { //
        x_axis_a = glm::cross(glm::vec3(0, 0, 1), z_axis_a);
        if (glm::length2(x_axis_a) < 1e-6f) {
            x_axis_a = glm::cross(glm::vec3(1, 0, 0), z_axis_a);
        }
    }
    x_axis_a = glm::normalize(x_axis_a);
    glm::vec3 y_axis_a = glm::cross(z_axis_a, x_axis_a);
    glm::mat3 matrix_a(x_axis_a, y_axis_a, z_axis_a);

    // --- Orientierungsmatrix für B ---
    glm::vec3 z_axis_b = b;
    glm::vec3 x_axis_b = glm::cross(world_up, z_axis_b);

    if (glm::length2(x_axis_b) < 1e-6f) {
        x_axis_b = glm::cross(glm::vec3(0, 0, 1), z_axis_b);
        if (glm::length2(x_axis_b) < 1e-6f) {
            x_axis_b = glm::cross(glm::vec3(1, 0, 0), z_axis_b);
        }
    }
    x_axis_b = glm::normalize(x_axis_b);
    glm::vec3 y_axis_b = glm::cross(z_axis_b, x_axis_b);
    glm::mat3 matrix_b(x_axis_b, y_axis_b, z_axis_b);

    // --- Finale Rotationsmatrix: R = B * A^T ---
    // In glm ist die Matrizen-Multiplikations-Reihenfolge umgekehrt zu numpy @
    // R = B * A^T
    glm::mat3 R = matrix_b * glm::transpose(matrix_a);

    // Konvertiere 3x3 zu 4x4
    return glm::mat4(R);
}