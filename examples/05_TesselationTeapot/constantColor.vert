#version 400 core
uniform mat4  MV;
uniform mat4  P;

layout(location = 0) in vec4 vPosition;

void
main()
{
  gl_Position = P * MV * vPosition;
}

