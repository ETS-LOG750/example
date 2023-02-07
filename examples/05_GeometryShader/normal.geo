#version 400 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

uniform float scale;

in vec3 fNormal[];
  
void GenerateLine(int index)
{
    vec4 projection = vec4(1, 1, -1, 1);

    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projection * (gl_in[index].gl_Position + 
                                vec4(fNormal[index], 0.0) * scale);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}  