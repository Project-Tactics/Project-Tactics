#version 330 core
layout(location = 0) out vec4 color;
in vec2 v_TexCoord;
uniform sampler2D u_TextureSample0;
void main()
{
    vec4 texColor = texture(u_TextureSample0, v_TexCoord);
    color = texColor;
}