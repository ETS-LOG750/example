#version 400 core

uniform mat4 m;
uniform mat3 mNormal;

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec3 vNormal;

out vec3 fNormal;

void main()
{
    // Output (pour le fragment shader)
    fNormal = mNormal * vNormal;

    // Sortie dans l'espace NDC (normalized device coordinate)
    vec3 fPosition = vec3(m * vPosition);
    gl_Position = vec4(fPosition, 1.0); // La projection sera effectue dans le geometry shader
}

