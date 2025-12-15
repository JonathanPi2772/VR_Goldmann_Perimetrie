#version 300 es

// Per-Vertex Attribute
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

// Uniforms (Matrizen)
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform mat3 uNormalMatrix; // Inverse Transponierte von mat3(uModelViewMatrix)

// Ausgänge zum Fragment Shader (im View-Space)
out vec3 vNormalView;

void main() {
    // Transformiere die Normale in den View-Space
    // (uNormalMatrix stellt sicher, dass sie auch bei ungleichmäßiger Skalierung korrekt bleibt)
    vNormalView = normalize(uNormalMatrix * aNormal);

    // Transformiere die Position
    gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aPosition, 1.0);
}