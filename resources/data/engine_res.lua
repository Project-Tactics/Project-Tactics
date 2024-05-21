resourcePack({
    name = "initialization",
    inis = function()
        file("configFile", "config.ini", [[
            [window]
            width=1280
            height=720
        ]])
    end
})
