#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_ModelViewProjection;
uniform mat4 u_View;

void main()
{
    vec3 cameraRightWorldSpace = vec3(u_View[0][0], u_View[1][0], u_View[2][0]);
    vec3 cameraUpWorldSpace = vec3(u_View[0][1], u_View[1][1], u_View[2][1]);
    v_TexCoord = texCoord;
    gl_Position = u_ModelViewProjection * position;
}