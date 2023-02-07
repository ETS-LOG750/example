#version 400 core

uniform mat4 m;
uniform mat3 mNormal;

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec3 vNormal;

out vec3 fNormal;
out vec3 fPosition;

void main()
{
    // Output (pour le fragment shader)
    fPosition = vec3(m * vPosition);
    fNormal = mNormal * vNormal;

    // Sortie dans l'espace NDC (normalized device coordinate)
    gl_Position = vec4(fPosition.x, fPosition.y, -fPosition.z, 1.0);
}

