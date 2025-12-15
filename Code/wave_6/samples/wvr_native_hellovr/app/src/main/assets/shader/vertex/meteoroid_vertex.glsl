#version 300 es

// (location = 0) ist an mPositionHandle gebunden
layout (location = 0) in vec3 aPos;
// (location = 1) ist an mNormalHandle gebunden
layout (location = 1) in vec3 aNormal;


// Matrizen, die von C++ gesetzt werden
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vNormal;


void main()
{
    // 1. Position berechnen (wie bisher)
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // 2. Normale berechnen und weitergeben (DER WICHTIGE TEIL)
    //    Wir transformieren die Normale mit der Model-Matrix,
    //    um sie in den World-Space zu bringen (für die Beleuchtung).
    //    Die mat3(transpose(inverse(model))) ist die korrekte
    //    Möglichkeit, Normalen zu transformieren, falls das Modell
    //    nicht-uniform skaliert wird.
    vNormal = mat3(transpose(inverse(model))) * aNormal;
}