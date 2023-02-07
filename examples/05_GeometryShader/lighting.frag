#version 400 core

out vec4 oColor;

in vec3 fNormal;
in vec3 fPosition;

void main()
{
    // Light info
    vec3 lightPos = vec3(0, 0, 2);
    vec3 lightIntensity = vec3(2.0);
    // Material info
    vec3 kd = vec3(1, 0, 0);

    vec3 nNormal = normalize(fNormal);
    
    vec3 l = lightPos - fPosition;
    float dist = length(l);
    l /= dist;
    
    float cosTheta = max(0, dot(nNormal, l));
    oColor = vec4(cosTheta * kd * lightIntensity / (dist*dist), 1);
}
