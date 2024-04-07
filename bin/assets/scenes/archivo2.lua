scene = {
        {
            components = {
                Transform = {
                    positionX = 0.0,
                    positionY = 0.0,
                    positionZ = 200.0,
                    scaleX = 1.0,
                    scaleY = 1.0,
                    scaleZ = 1.0,
                    rotationX = 0.0,
                    rotationY = 0.0,
                    rotationZ = 0.0
                },
                CameraComponent = {
                    zOrder = 0,
                    bgColorR = 0.8,
                    bgColorG = 0.8,
                    bgColorB = 0.8,
                    directionX = 0.0,
                    directionY = 0.0,
                    directionZ = -1.0
                }
            }
        },
        {
            components = {
                Transform = {
                    positionX = 0.0,
                    positionY = 0.0,
                    positionZ = 0.0,
                    scaleX = 3.1,
                    scaleY = 3.1,
                    scaleZ = 3.1,
                    rotationX = 0.0,
                    rotationY = 180.0,
                    rotationZ = 0.0
                },
                MeshRenderer = {  
                    meshName = "models/Sinbad.mesh",
                }
            },
            children = {
                {
                    components = {
                        Transform = {
                            positionX = 40.0,
                            positionY = 20.0,
                            positionZ = 0.0,
                            scaleX = 1.0,
                            scaleY = 1.0,
                            scaleZ = 1.0,
                            rotationX = 0.0,
                            rotationY = 0.0,
                            rotationZ = 90.0
                        },
                        MeshRenderer = {  
                            meshName = "models/Sinbad.mesh",
                        }
                    },
                    children = {
                        {
                            components = {
                                Transform = {
                                    positionX = 40.0,
                                    positionY = 0.0,
                                    positionZ = 0.0,
                                    scaleX = 1.0,
                                    scaleY = 1.0,
                                    scaleZ = 1.0,
                                    rotationX = 90.0,
                                    rotationY = 0.0,
                                    rotationZ = 0.0
                                },
                                MeshRenderer = {  
                                    meshName = "models/Sinbad.mesh",
                                }
                            }
                        }
                    }
                }
            }
        }
    }
