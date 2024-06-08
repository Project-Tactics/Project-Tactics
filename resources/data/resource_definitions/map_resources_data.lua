local json = require "scripts.json"

local packs = {
    mapTextures = {
        texture = {},
        mesh = {}
    }
}

local mapTextureCounts = { 10, 13, 8, 5, 14 }
for mapIndex = 1, #mapTextureCounts do
    local mapName = string.format("map%02d", mapIndex - 1)
    for i = 1, mapTextureCounts[mapIndex] do
        local textureName = string.format("%s_%02d", mapName, i - 1)
        packs.mapTextures.texture[textureName] = {
            path = string.format("textures/%s/tex%02d.png", mapName, i - 1)
        }
    end

    packs.mapTextures.mesh[mapName] = {
        path = string.format("meshes/%s.fbx", mapName)
    }
end

return json.encode(packs)
