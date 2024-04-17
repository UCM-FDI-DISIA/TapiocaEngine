scene = {
        {
            components = {
                Transform = {
                    positionX = 50.0,
                    positionY = 50.0,
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
                    bgColorR = 0.925,
                    bgColorG = 0.698,
                    bgColorB = 0.94
                }
            }
        },
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
                    positionY = 3.0,
                    positionZ = 0.0,
                    scaleX = 2.0,
                    scaleY = 2.0,
                    scaleZ = 2.0,
                    rotationX = -90.0,
                    rotationY = 0.0,
                    rotationZ = 0.0
                },
                RigidBody = {    
                    colShape=0, --0 BOX, 1 SPHERE, 2 PLANE, 3 CAPSULE
                    colliderScaleX=2.0,
                    colliderScaleY=2.0,
                    colliderScaleZ=2.0,
                    isTrigger=false,
                    movementType=0, --0 DYNAMIC , 1 STATIC, 2 KINEMATIC
                    mass=3.0,
                    friction=1.0,
                    damping=0.4,
                    bounciness=0.0
                },
                MeshRenderer = {
                    meshName = "models/cube.mesh",
                    initRotationX = -90.0,
                    initRotationY = 0.0,
                    initRotationZ = 0.0
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
        Hola2={
            components = {
                -- EjemploComponent = { -- Prueba de un componente de juego
                --     a = 1,
                --     b = 2.0,
                --     c = true,
                --     d = "ejemplo"
                -- },
                Transform = {
                    positionX = 5.0,
                    positionY = 8.0,
                    positionZ = 0.0,
                    scaleX = 2.0,
                    scaleY = 2.0,
                    scaleZ = 2.0,
                    rotationX = 0.0,
                    rotationY = 0.0,
                    rotationZ = 0.0
                },
                
                RigidBody = {    
                    colShape=3, --0 BOX, 1 SPHERE, 2 PLANE, 3 CAPSULE
                    colliderScaleX=1.0,
                    colliderScaleY=1.0,
                    colliderScaleZ=1.0,
                    isTrigger=false,
                    movementType=0, --0 DYNAMIC , 1 STATIC, 2 KINEMATIC
                    mass=0.05,
                    friction=0.0,
                    damping=0.4,
                    bounciness=1.0
                }
            },
           
            
        },
        Ground={
            components={
    
            Transform = {
                positionX = 0.0,
                positionY = -4.0,
                positionZ = 0.0,
                scaleX = 2.0,
                scaleY = 2.0,
                scaleZ = 2.0,
                rotationX = 0.0,
                rotationY = 0.0,
                rotationZ = 0.0
            },
            RigidBody = {    
                colShape=2, --0 BOX, 1 SPHERE, 2 PLANE, 3 CAPSULE
                colliderScaleX=0.0,
                colliderScaleY=1.0,
                colliderScaleZ=0.0,
                isTrigger=false,
                movementType=1, --0 DYNAMIC , 1 STATIC, 2 KINEMATIC
                mass=0.0,
                friction=1.0,
                damping=0.998,
                bounciness=1.0
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
