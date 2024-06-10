local json = require "common.scripts.json"

local packs = {
    demoMaps = {
        texture = {},
        mesh = {},
        prefab = {
            mapCamera = {
                path = "_demoMaps/mapCamera.prefab"
            }
        }
    }
}

local mapTextureCounts = { 10, 13, 8, 5, 14 }
for mapIndex = 1, #mapTextureCounts do
    local mapName = string.format("map%02d", mapIndex - 1)
    for i = 1, mapTextureCounts[mapIndex] do
        local textureName = string.format("%s_%02d", mapName, i - 1)
        packs.demoMaps.texture[textureName] = {
            path = string.format("_demoMaps/%s/tex%02d.png", mapName, i - 1)
        }
    end

    packs.demoMaps.mesh[mapName] = {
        path = string.format("_demoMaps/%s/%s.fbx", mapName, mapName)
    }
    packs.demoMaps.prefab[mapName] = {
        path = string.format("_demoMaps/%s/%s.prefab", mapName, mapName)
    }
end

return json.encode(packs)
