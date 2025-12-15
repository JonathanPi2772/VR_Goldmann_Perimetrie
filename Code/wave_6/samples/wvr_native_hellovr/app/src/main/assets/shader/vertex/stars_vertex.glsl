#version 300 es
precision mediump float;

// Eingabe-Attribute, die zu den `glVertexAttribPointer`-Aufrufen passen
layout (location = 0) in vec3 a_position;
layout (location = 1) in float a_size;

// Uniforms für die Transformation
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;

void main() {
    // Transformiere die Position des Sterns in den Clip Space
    gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(a_position, 1.0);

    // Setze die Größe des gerenderten Punktes basierend auf dem 'a_size'-Attribut
    gl_PointSize = a_size;
}