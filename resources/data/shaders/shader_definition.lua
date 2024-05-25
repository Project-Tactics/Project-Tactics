shader(
    "main",
    [[
        #version 330 core
        layout(location = 0) in vec4 position;
        layout(location = 1) in vec2 texCoord;

        out vec2 v_TexCoord;

        uniform mat4 u_ModelViewProjection;

        void main()
        {
            gl_Position = u_ModelViewProjection * position;
            v_TexCoord = texCoord;
        }
    ]],
    [[
        #version 330 core
        layout(location = 0) out vec4 color;
        in vec2 v_TexCoord;
        uniform sampler2D u_Texture;
        void main()
        {
            vec4 texColor = texture(u_Texture, v_TexCoord);
            color = texColor;
        }
    ]]
)
