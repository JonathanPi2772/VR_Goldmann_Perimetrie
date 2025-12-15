#version 300 es
// (Oder #version 310 es)

precision mediump float;

// Eingänge vom Vertex Shader
in vec3 vNormalView;

// Uniforms (Szenen-Parameter)
uniform vec3 uLightDirView; // Lichtrichtung, bereits im View-Space
uniform vec3 uTerrainColor;

// Ausgabe
out vec4 FragColor;

void main() {
    // Ambient-Anteil (damit Schatten nicht komplett schwarz sind)
    float ambient = 0.3;

    // Diffuse-Anteil
    vec3 norm = normalize(vNormalView);
    vec3 lightDir = normalize(uLightDirView);

    // Berechne den Lichteinfallswinkel (Dot-Produkt)
    // max(..., 0.0) klemmt den Wert ab, damit "Rückseiten" nicht beleuchtet werden.
    float diffuse = max(dot(norm, lightDir), 0.0);

    // Kombiniere die Farbe
    vec3 finalColor = uTerrainColor * (ambient + diffuse * 0.7); // (0.7 ist die Stärke des diffusen Lichts)

    FragColor = vec4(finalColor, 1.0);
}