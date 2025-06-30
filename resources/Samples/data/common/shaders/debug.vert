#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out vec4 v_Color;

uniform mat4 u_ModelViewProjection;

void main()
{
    gl_Position = u_ModelViewProjection * position;
    v_Color = color;
}