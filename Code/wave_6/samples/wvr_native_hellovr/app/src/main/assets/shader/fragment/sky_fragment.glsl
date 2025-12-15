#version 300 es
precision mediump float;

out vec4 fragColor;

void main() {
    float maxbrightness = 320.0;
    float expected_brightness = 10.0;
    float col_val = expected_brightness / maxbrightness;
    // 10 nits bei 1000 nits maximaler Helligkeit
    fragColor = vec4(col_val, col_val, col_val, 1.0);
}