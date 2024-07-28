local json = require "common.scripts.json"

local packs = {
    demoMap = {
        Texture = {
            spriteSheet00 = {
                path = "_demoMap/spriteSheet00.png",
                filter = "Nearest"
            },
            charShadow = {
                path = "_demoMap/charShadow.png"
            }
        },
        SpriteSheet = {
            char00 = {
                texture = "spriteSheet00",
                spriteSize = {
                    x = 32,
                    y = 48
                },
                animations = {
                    idleSouth = {
                        frames = { 8, 9, 10, 11, 12, 11, 10, 9 },
                        duration = 1
                    },
                    idleNorth = {
                        frames = { 16, 17, 18, 19, 20, 19, 18, 17 },
                        duration = 1
                    }
                }
            }
        },
        Mesh = {},
        Prefab = {
            mapCamera = {
                path = "_demoMap/mapCamera.prefab"
            },
            character = {
                path = "_demoMap/character.prefab"
            },
            charShadow = {
                path = "_demoMap/charShadow.prefab"
            }
        },
        InputAction = {
            loadNextMap = {
                type = "Scalar"
            },
            moveCamera = {
                type = "Scalar"
            }
        },
        InputMap = {
            mapDemoInput = {
                bindings = {
                    {
                        action = "loadNextMap",
                        gestures = {
                            {
                                gesture = {
                                    type = "simple",
                                    input = "KeyReturn"
                                },
                                conditions = {
                                    {
                                        type = "press",
                                        actuationThreshold = 0.5
                                    }
                                }
                            }
                        }
                    },
                    {
                        action = "moveCamera",
                        gestures = {
                            {
                                gesture = {
                                    type = "simple",
                                    input = "KeyA"
                                },
                                conditions = {
                                    {
                                        type = "press",
                                        actuationThreshold = 0.5
                                    }
                                }
                            },
                            {
                                gesture = {
                                    type = "simple",
                                    input = "KeyD"
                                },
                                conditions = {
                                    {
                                        type = "press",
                                        actuationThreshold = 0.5
                                    }
                                },
                                modifiers = {
                                    {
                                        type = "negate",
                                        axis = "X"
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

local mapTextureCounts = { 10, 13, 8, 5, 14 }
for mapIndex = 1, #mapTextureCounts do
    local mapName = string.format("map%02d", mapIndex - 1)
    for i = 1, mapTextureCounts[mapIndex] do
        local textureName = string.format("%s_%02d", mapName, i - 1)
        packs.demoMap.Texture[textureName] = {
            path = string.format("_demoMap/%s/tex%02d.png", mapName, i - 1)
        }
    end

    packs.demoMap.Mesh[mapName] = {
        path = string.format("_demoMaps/%s/%s.fbx", mapName, mapName)
    }
    packs.demoMap.Prefab[mapName] = {
        path = string.format("_demoMaps/%s/%s.prefab", mapName, mapName)
    }
end

return json.encode(packs)
