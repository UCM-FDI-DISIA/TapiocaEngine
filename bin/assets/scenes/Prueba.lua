scene = {
    {
        components = {
            Transform = {
                positionX = 0.0,
                positionY = 0.0,
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
                bgColorR = 0.0,
                bgColorG = 0.0,
                bgColorB = 0.0
            },
            LightDirComp = {
                directionX = 0.0,
                directionY = 0.0,
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
            }
        },
        children = {
            {
                components = {
                    Transform = {
                        positionX = 5.0,
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
                    }
                },
                children = {
                    {
                        components = {
                            Transform = {
                                positionX = 5.0,
                                positionY = 0.0,
                                positionZ = 0.0,
                                scaleX = 1.0,
                                scaleY = 1.0,
                                scaleZ = 1.0,
                                rotationX = 0.0,
                                rotationY = 0.0,
                                rotationZ = 0.0
                            },
                            MeshRenderer = {
                                meshName  = "cube2.mesh",
                            }
                        }
                    }
                }
            }
        }
    }
}
  