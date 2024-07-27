local json = require "common.scripts.json"

local packs = {
    demoMaps = {
        Texture = {
            spriteSheet00 = {
                path = "_demoMaps/spriteSheet00.png",
                filter = "nearest"
            },
            charShadow = {
                path = "_demoMaps/charShadow.png"
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
                path = "_demoMaps/mapCamera.prefab"
            },
            character = {
                path = "_demoMaps/character.prefab"
            },
            charShadow = {
                path = "_demoMaps/charShadow.prefab"
            }
        },
        InputAction = {
            loadNextMap = {
                type = "scalar"
            },
            moveCamera = {
                type = "scalar"
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
                                    data = {
                                        input = "keyReturn"
                                    }
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
                                    data = {
                                        input = "keyA"
                                    }
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
                                    data = {
                                        input = "keyD"
                                    }
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
                                        data = {
                                            axis = "x"
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
}

local mapTextureCounts = { 10, 13, 8, 5, 14 }
for mapIndex = 1, #mapTextureCounts do
    local mapName = string.format("map%02d", mapIndex - 1)
    for i = 1, mapTextureCounts[mapIndex] do
        local textureName = string.format("%s_%02d", mapName, i - 1)
        packs.demoMaps.Texture[textureName] = {
            path = string.format("_demoMaps/%s/tex%02d.png", mapName, i - 1)
        }
    end

    packs.demoMaps.Mesh[mapName] = {
        path = string.format("_demoMaps/%s/%s.fbx", mapName, mapName)
    }
    packs.demoMaps.Prefab[mapName] = {
        path = string.format("_demoMaps/%s/%s.prefab", mapName, mapName)
    }
end

return json.encode(packs)
