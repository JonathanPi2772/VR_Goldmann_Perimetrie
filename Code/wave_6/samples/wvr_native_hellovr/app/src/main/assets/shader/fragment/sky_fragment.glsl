#version 300 es
precision mediump float;

out vec4 fragColor;
uniform vec3 uSkyColor; // Receive the corrected color from C++

void main() {
    fragColor = vec4(uSkyColor, 1.0);
}