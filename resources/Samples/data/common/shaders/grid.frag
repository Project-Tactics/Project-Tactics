#version 330 core
layout(location = 0) out vec4 color;
in vec2 v_TexCoord;
void main()
{
    vec2 uv = v_TexCoord;
    float size = 1.0/10.0;   // size of the tile
    float edge = size/32.0; // size of the edge
    uv = (mod(uv, size) - mod(uv - edge, size) - edge) * 1.0/size;
    color = vec4(length(uv), length(uv), length(uv), 0.5); // 0.9 for the face of the tile, 0.5 for the edge
    //color = vec4(0.9 - length(uv) * 0.5); // 0.9 for the face of the tile, 0.5 for the edge
}