scene = {
        Hola={
            components = {
                -- EjemploComponent = { -- Prueba de un componente de juego
                --     a = 1,
                --     b = 2.0,
                --     c = true,
                --     d = "ejemplo"
                -- },
                Transform = {
                    positionX = 0.0,
                    positionY = 0.0,
                    positionZ = 6.0,
                    scaleX = 2.0,
                    scaleY = 2.0,
                    scaleZ = 2.0,
                    rotationX = -90.0,
                    rotationY = 0.0,
                    rotationZ = 0.0
                },
                MeshRenderer = {
                    meshName = "racoon/pirateRaccoon/PirateRacoon.mesh"
                },
                RigidBody = {    
                    colShape=0, --0 BOX, 1 SPHERE, 2 PLANE, 3 CAPSULE
                    colliderScaleX=2.0,
                    colliderScaleY=2.0,
                    colliderScaleZ=2.0,
                    isTrigger=false,
                    movementType=1, --0 DYNAMIC , 1 STATIC, 2 KINEMATIC
                    friction=1.0,
                    mass=3.0,
                    bounciness=0.0
                }
            },
            children ={
                a1={
                    components = {
                        Transform = {
                            positionX = 22.0,
                            positionY = 12.0,
                            positionZ = -56.0,
                            scaleX = 25.0,
                            scaleY = 25.0,
                            scaleZ = 25.0,
                            rotationX = 25.0,
                            rotationY = 25.0,
                            rotationZ = 25.0
                        },
                    },
                     children ={
                        a3={
                            components = {
                                Transform = {
                                    positionX = 22.0,
                                    positionY = 12.0,
                                    positionZ = -56.0,
                                    scaleX = 25.0,
                                    scaleY = 25.0,
                                    scaleZ = 25.0,
                                    rotationX = 25.0,
                                    rotationY = 25.0,
                                    rotationZ = 25.0
                                }
                            }
                        },
                        a4={
                            components = {
                                Transform = {
                                    positionX = 22.0,
                                    positionY = 12.0,
                                    positionZ = -56.0,
                                    scaleX = 25.0,
                                    scaleY = 25.0,
                                    scaleZ = 25.0,
                                    rotationX = 25.0,
                                    rotationY = 25.0,
                                    rotationZ = 25.0
                                }
                            }
                            
                        }
                    }
                },
                a2={
                    components = {
                        Transform = {
                            positionX = 22.0,
                            positionY = 12.0,
                            positionZ = -56.0,
                            scaleX = 25.0,
                            scaleY = 25.0,
                            scaleZ = 25.0,
                            rotationX = 25.0,
                            rotationY = 25.0,
                            rotationZ = 25.0
                        }
                    }
                    
                }
            }
            
        },
        Entity1={
            components = {
                Transform = {
                    positionX = 22.0,
                    positionY = 12.0,
                    positionZ = -56.0,
                    scaleX = 25.0,
                    scaleY = 25.0,
                    scaleZ = 25.0,
                    rotationX = 25.0,
                    rotationY = 25.0,
                    rotationZ = 25.0
                }
            }
        },
        Entity2={
            components = {
                Transform = {
                    positionX = 22.0,
                    positionY = 12.0,
                    positionZ = -56.0,
                    scaleX = 25.0,
                    scaleY = 25.0,
                    scaleZ = 25.0,
                    rotationX = 25.0,
                    rotationY = 25.0,
                    rotationZ = 25.0
                }
            }
            
        },
        Entity3={
            components = {
                Transform = {
                    positionX = 22.0,
                    positionY = 12.0,
                    positionZ = -56.0,
                    scaleX = 25.0,
                    scaleY = 25.0,
                    scaleZ = 25.0,
                    rotationX = 25.0,
                    rotationY = 25.0,
                    rotationZ = 25.0
                }
            }
            
        },
        Entity4={
            components = {
                Transform = {
                    positionX = 22.0,
                    positionY = 12.0,
                    positionZ = -56.0,
                    scaleX = 25.0,
                    scaleY = 25.0,
                    scaleZ = 25.0,
                    rotationX = 25.0,
                    rotationY = 25.0,
                    rotationZ = 25.0
                }
            }
            
        }
        
    }
