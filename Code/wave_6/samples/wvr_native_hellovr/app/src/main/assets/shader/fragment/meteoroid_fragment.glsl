#version 300 es
precision mediump float; // WICHTIG: Im Fragment-Shader hinzufügen

out vec4 FragColor; // Ausgabefarbe
uniform vec3 u_color;
in vec3 vNormal;

void main()
{
    vec3 final_color = u_color;
    final_color += vNormal * 0.00000001;

    // 3. Setze die endgültige Farbe
    FragColor = vec4(final_color, 1.0);
}