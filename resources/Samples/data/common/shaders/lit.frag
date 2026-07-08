#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 fragPos;

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_DiffuseTexture;
uniform vec4 u_DiffuseColor;
uniform float u_UseTexture;
uniform float u_AmbientStrength;
uniform vec3 u_AmbientColor;

void main()
{
    vec3 lightPosition = vec3(-20.0, -10.0, -2000.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    vec3 normalizedNormal = normalize(normal);
    vec3 lightDirection = normalize(lightPosition - fragPos);
    vec4 lighting = vec4(lightColor * max(dot(normalizedNormal, lightDirection), 0.0), 1);

    vec4 texColor = texture(u_DiffuseTexture, uv);
    vec4 diffuseColor = mix(u_DiffuseColor, texColor * u_DiffuseColor, u_UseTexture);
    vec4 ambientColor = vec4(u_AmbientColor * u_AmbientStrength, 1.0);
    o_Color = diffuseColor * (ambientColor + lighting);
}