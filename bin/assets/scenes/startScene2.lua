scene = {
    {
        components = {
            CameraComponent = {
                zOrder = 1, -- Cuando se arregle, se podrá cambiar a 0
                bgColorR = 0.0,
                bgColorG = 0.0,
                bgColorB = 0.0
            }
        }
    },
    Text = {
        components = {
            Transform = {
                positionX = WINDOW_WIDTH / 2,
                positionY = 50.0
            },
            Text = {
                name = "Hola",
                text = "HOLAAAAA",
                textFontName = "AGENCYB.TTF",
            }
        }
    }
}
  