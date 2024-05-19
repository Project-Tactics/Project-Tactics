-- The
resourcePack({
    name = "mainPackage",
    shaders = function()
        shaderDef("shaders\\shader_definition.lua")
    end,
    textures = function()
        textureDef("textures\\texture_definition.lua")
    end,
})
