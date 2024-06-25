#version 330 core
layout(location = 0) out vec4 color;
in vec2 v_TexCoord;
uniform sampler2D u_Texture;
uniform vec4 u_Color;
uniform vec2 u_SpriteUV;
uniform vec2 u_SpriteSize;
uniform vec2 u_FlipUV;
void main()
{
    vec2 uvFlip = vec2(u_FlipUV.x, -u_FlipUV.y);
    vec2 flipFactor = (uvFlip + 1.0) * 0.5;
    vec2 uvOffset = ((1 - flipFactor) * ( 1 - v_TexCoord ) + ( flipFactor * v_TexCoord )) * vec2(u_SpriteSize.x, -u_SpriteSize.y);

    vec4 texColor = texture(u_Texture, u_SpriteUV.xy + uvOffset.xy);
    color = texColor * u_Color;
}