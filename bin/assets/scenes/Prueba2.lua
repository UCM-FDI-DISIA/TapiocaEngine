scene = {
    {
        components = {
            Transform = {
                positionX = 40.0,
                positionY = 40.0,
                positionZ = 50.0,
                scaleX = 1.0,
                scaleY = 1.0,
                scaleZ = 1.0,
                rotationX = 0.0,
                rotationY = 0.0,
                rotationZ = 0.0
            },
            CameraComponent = {
                zOrder = 0,
                bgColorR = 0.3,
                bgColorG = 0.5,
                bgColorB = 0.5,
                targetToLookX = 0.0,
                targetToLookY = 0.0,
                targetToLookZ = 0.0
            },
            LightDirComp = {
                directionX = 0.0,
                directionY = -1.0,
                directionZ = -1.0,
                mainLight = true
            }
        }
    },
    {
        components = {
            Transform = {
                positionX = 0.0,
                positionY = 0.0,
                positionZ = 0.0,
                scaleX = 2.0,
                scaleY = 2.0,
                scaleZ = 2.0,
                rotationX = 0.0,
                rotationY = 0.0,
                rotationZ = 0.0
            },
            MeshRenderer = {
                meshName  = "cube2.mesh",
            },
            KeyFrameAnimator = {
                duration = 5.0,
                posKeyX1 = 20.0,
                posKeyY1 = 0.0,
                posKeyZ1 = 0.0,
                rotKeyX1 = 0.0,
                rotKeyY1 = 0.0,
                rotKeyZ1 = -90.0,
                posKeyX2 = 0.0,
                posKeyY2 = 0.0,
                posKeyZ2 = 0.0,
                rotKeyX2 = 0.0,
                rotKeyY2 = 0.0,
                rotKeyZ2 = 0.0
            }
        }
    }
}
  