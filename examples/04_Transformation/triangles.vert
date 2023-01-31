#version 400 core
uniform mat4 m;
in vec4 vPosition;

void main()
{
    vec4 p = m * vPosition;
    gl_Position = vec4(p.x, p.y, -p.z, p.w);
}

