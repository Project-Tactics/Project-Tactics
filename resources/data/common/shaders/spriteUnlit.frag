#version 330 core
layout(location = 0) out vec4 color;
in vec2 v_TexCoord;
uniform sampler2D u_Texture;
uniform vec4 u_Color;
uniform vec2 u_SpriteUV;
uniform vec2 u_SpriteSize;
void main()
{
    vec4 texColor = texture(u_Texture, u_SpriteUV.xy + (v_TexCoord * u_SpriteSize.xy));
    color = texColor * u_Color;
}