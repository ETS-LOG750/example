#version 430 core

layout(location = 1) in  vec4  vPosition;

void
main()
{
    gl_Position = vPosition;
}
