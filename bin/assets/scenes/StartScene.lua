scene = {
    {
        components = {
            CameraComponent = {
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
                text = "made with",
                textFontName = "AGENCYB.TTF",
            },
            AudioSourceComponent={
                sourcepath="assets/BlankSpace.mp3",
                ispaused = false

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
                text = "Tapioca Engine",
                textSize = 36.0,
                textFontName = "AGENCYB.TTF"
            },
            MeshRenderer={
                meshName="PirateRacoon.mesh"

            }
        }
    }
}
