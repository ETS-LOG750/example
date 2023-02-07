#version 400 core

uniform mat4 mvMatrix;
uniform mat4 projMatrix;
uniform mat3 normalMatrix;

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;

out vec4 ifColor;
out vec3 fNormal;
out vec3 fPosition;

void
main()
{
     vec4 vEyeCoord = mvMatrix * vPosition;
     gl_Position = projMatrix * vEyeCoord;
     fPosition = vEyeCoord.xyz;
     fNormal = normalMatrix*vNormal;
     ifColor = vColor;
}

