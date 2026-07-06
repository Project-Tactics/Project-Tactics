#version 330 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec2 i_UV;
layout(location = 2) in vec3 i_Normal;

out vec2 uv;
out vec3 normal;
out vec3 fragPos;

uniform mat4 u_ModelViewProjection;
uniform mat4 u_Model;

void main()
{
    uv = i_UV;
    normal = i_Normal;
    fragPos = vec3(u_Model * vec4(i_Pos, 1.0));
    gl_Position = u_ModelViewProjection * vec4(i_Pos, 1.0);
}