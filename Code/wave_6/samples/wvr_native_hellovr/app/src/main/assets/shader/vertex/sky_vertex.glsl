#version 300 es
precision mediump float;

layout (location = 0) in vec3 a_position;

uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;

void main() {
    // Nur die Position transformieren, keine weiteren Berechnungen nötig
    gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(a_position, 1.0);
}