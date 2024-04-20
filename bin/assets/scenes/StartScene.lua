scene = {
    {
        components = {
            CameraComponent = {
                zOrder = 0,
                bgColorR = 0.0,
                bgColorG = 0.659,
                bgColorB = 0.929
            },
            SplashScreenComponent = {
                changeTime = 2000,
                gameName = "game"
            }
        }
    },
    madeWithText = {
        components = {
            Transform = {
                positionX = WINDOW_WIDTH / 2,
                positionY = WINDOW_HEIGHT / 2 - 20
            },
            Text = {
                name = "madeWithText",
                text = "made with",
                textFontName = "AGENCYB.TTF"
            }
        }
    },
    TapiocaText = {
        components = {
            Transform = {
                positionX = WINDOW_WIDTH / 2,
                positionY = WINDOW_HEIGHT / 2
            },
            Text = {
                name = "TapiocaText",
                text = "Tapioca Engine",
                textSize = 36.0,
                textFontName = "AGENCYB.TTF"
            }
        }
    },
    -- StartButton = {
    --     -- zIndex = 0, -- No importa si es 0
    --     components = {
    --         Transform = {
    --             positionX = WINDOW_WIDTH / 2,
    --             positionY = WINDOW_HEIGHT / 2,
    --             scaleX = 130.0,
    --             scaleY = 40.0
    --         },
    --         Button = {
    --             name = "StartButton",
    --             text = "Play",
    --             onClickId = "debug",
    --             textFontName = "impact.ttf",
    --             textColorNormalR = 1.0,
    --             textColorNormalG = 0.0,
    --             textColorNormalB = 0.0,
    --             textColorNormalA = 1.0,
    --             textColorHoverR = 0.0,
    --             textColorHoverG = 1.0,
    --             textColorHoverB = 0.0,
    --             textColorHoverA = 1.0,
    --             textColorActiveR = 0.0,
    --             textColorActiveG = 0.0,
    --             textColorActiveB = 1.0,
    --             textColorActiveA = 1.0,
    --             -- windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings
    --         }
    --     }
    -- },
    -- StartInputText={
    --     -- zIndex = 0, -- No importa si es 0
    --     components = {
    --         Transform = {
    --             positionX = WINDOW_WIDTH / 2,
    --             scaleX = 200.0
    --         },
    --         InputText = {
    --             name = "StartInputText",
    --             placeHolderText = "Dime tu nombre...",
    --             bufferSize = 100,
    --             onTextEnteredId = "debug",
    --             textSize = 20.0,
    --             textFontName = "AGENCYB.TTF",
    --             textColorR = 1.0,
    --             textColorG = 0.0,
    --             textColorB = 1.0,
    --             textColorA = 1.0,
    --             -- bgColorR = 1.0,
    --             -- bgColorG = 0.0,
    --             -- bgColorB = 0.0,
    --             -- bgColorA = 1.0,
    --             -- flags = 0 | ImGuiInputTextFlags_Password,
    --             -- windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings
    --         }
    --     }
    -- },
    -- StartImageTextButton={
    --     -- zIndex = 0, -- No importa si es 0
    --     components = {
    --         Transform = {
    --             positionX = WINDOW_WIDTH / 2,
    --             positionY = 100.0,
    --             scaleX = 100.0,
    --             scaleY = 100.0
    --         },
    --         ImageTextButton = {
    --             name = "StartImageButton",
    --             onClickId = "debug",
    --             wantText = true,
    --             text = "Play",
    --             textSize = 35.0,
    --             textFontName = "arial.ttf",
    --             textColorNormalR = 1.0,
    --             textColorNormalG = 0.0,
    --             textColorNormalB = 0.0,
    --             textColorNormalA = 1.0,
    --             textColorHoverR = 0.0,
    --             textColorHoverG = 1.0,
    --             textColorHoverB = 0.0,
    --             textColorHoverA = 1.0,
    --             textColorActiveR = 0.0,
    --             textColorActiveG = 0.0,
    --             textColorActiveB = 1.0,
    --             textColorActiveA = 1.0,
    --             imagePathNormal = "textures/normal.png",
    --             imagePathHover = "textures/hover.png",
    --             imagePathActive = "textures/active.png",
    --             -- uv0X = 0.0,
    --             -- uv0Y = 0.0,
    --             -- uv1X = 1.0,
    --             -- uv1Y = 1.0,
    --             -- imageBgColorR = 1.0,
    --             -- imageBgColorG = 0.0,
    --             -- imageBgColorB = 1.0,
    --             -- imageBgColorA = 1.0,
    --             -- imageTintR = 1.0,
    --             -- imageTintG = 1.0,
    --             -- imageTintB = 0.0,
    --             -- imageTintA = 1.0,
    --             normalColorR = 0.0, -- Con el alpha en 0.0 en todos los estados solo se vera las imagenes del boton
    --             normalColorG = 1.0,
    --             normalColorB = 0.0,
    --             normalColorA = 0.0,
    --             hoverColorR = 0.0,
    --             hoverColorG = 0.0,
    --             hoverColorB = 1.0,
    --             hoverColorA = 0.0,
    --             activeColorR = 1.0,
    --             activeColorG = 1.0,
    --             activeColorB = 0.0,
    --             activeColorA = 0.0,
    --             -- windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings
    --         }
    --     }
    -- }
}
  