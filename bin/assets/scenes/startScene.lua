scene = {
    StartButton={
        components = {
            Transform = {
                positionX = 0.0,
                positionY = 0.0,
                positionZ = 0.0, -- No importa
                scaleX = 130.0,
                scaleY = 40.0,
                scaleZ = 0.0, -- No importa
                rotationX = 0.0,
                rotationY = 0.0,
                rotationZ = 0.0
            },
            Button = {
                name = "StartButton",
                text = "Play",
                -- onClick = function()
                --     print("Play")
                -- end,
                paddingX = 10.0,
                paddingY = 5.0,
                textFontName = "impact.ttf",
                textSize = 20.0,
                textColorR = 1.0,
                textColorG = 0.0,
                textColorB = 0.0,
                textColorA = 1.0,
                normalColorR = 0.0,
                normalColorG = 1.0,
                normalColorB = 0.0,
                normalColorA = 1.0,
                hoverColorR = 0.0,
                hoverColorG = 0.0,
                hoverColorB = 1.0,
                hoverColorA = 1.0,
                activeColorR = 1.0,
                activeColorG = 1.0,
                activeColorB = 0.0,
                activeColorA = 1.0,
                windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings
            }
        }
    }
}
