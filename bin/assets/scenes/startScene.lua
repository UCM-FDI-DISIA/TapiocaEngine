scene = {
    -- Boton que carga la escena de juego
    StartButton={
        components = {
            Transform = {
                positionX = 0.0,
                positionY = 200.0,
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
                onClickId = 1, -- Identificador de la funcion del boton, esta un poco feo
                -- paddingX = 10.0,
                -- paddingY = 5.0,
                -- textSize = 20.0,
                textFontName = "impact.ttf",
                -- textColorR = 1.0,
                -- textColorG = 0.0,
                -- textColorB = 0.0,
                -- textColorA = 1.0,
                -- normalColorR = 0.0,
                -- normalColorG = 1.0,
                -- normalColorB = 0.0,
                -- normalColorA = 1.0,
                -- hoverColorR = 0.0,
                -- hoverColorG = 0.0,
                -- hoverColorB = 1.0,
                -- hoverColorA = 1.0,
                -- activeColorR = 1.0,
                -- activeColorG = 1.0,
                -- activeColorB = 0.0,
                -- activeColorA = 1.0,
                -- windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings
            }
        }
    },
    StartInputText={
        components = {
            Transform = {
                positionX = 0.0,
                positionY = 0.0,
                positionZ = 0.0, -- No importa
                scaleX = 500.0,
                scaleY = 0.0, -- No importa
                scaleZ = 0.0, -- No importa
                rotationX = 0.0,
                rotationY = 0.0,
                rotationZ = 0.0
            },
            InputText = {
                name = "StartInputText",
                text = "Dime tu nombre...",
                bufferSize = 100,
                onTextEnteredId = 1, -- Identificador de la funcion de la caja de texto, esta un poco feo
                textSize = 20.0,
                textFontName = "impact.ttf",
                textColorR = 1.0,
                textColorG = 0.0,
                textColorB = 1.0,
                textColorA = 1.0,
                -- bgColorR = 1.0,
                -- bgColorG = 1.0,
                -- bgColorB = 0.0,
                -- bgColorA = 1.0,
                -- windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings
            }
        }
    },
    StartText={
        components = {
            Transform = {
                positionX = 450.0,
                positionY = 350.0,
                positionZ = 0.0, -- No importa
                scaleX = 0.0,
                scaleY = 0.0,
                scaleZ = 0.0, -- No importa
                rotationX = 0.0,
                rotationY = 0.0,
                rotationZ = 0.0
            },
            Text = {
                name = "StartText",
                text = "Tapioca Engine",
                textSize = 36.0,
                textFontName = "AGENCYB.TTF",
                textColorR = 0.0,
                textColorG = 0.0,
                textColorB = 1.0,
                textColorA = 1.0
            }
        }
    }
}
  