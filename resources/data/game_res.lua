resourcePack({
    name = "mainPackage",
    shaders = function()
        script("shaders\\shader_definition.lua")
    end,
    textures = function()
        script("textures\\texture_definition.lua")
    end,
    meshes = function()
        file("book", "meshes\\book.fbx")
    end
})
