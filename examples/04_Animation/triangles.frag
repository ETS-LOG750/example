#version 400 core

out vec4 fColor;

in vec3 fNormal;
in vec3 fPosition;

void main()
{
    vec3 nNormal = normalize(fNormal);
    fColor = vec4(nNormal*0.5 + 0.5, 1.0);
}
