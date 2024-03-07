# Tapioca Engine

<div align="center">
    <img src="https://img.shields.io/badge/status-building...-red" alt="YouTube Badge"/>
</div>

## Autores

- Matt Castellanos Silva [GitHub](https://github.com/MattCastUCM)
- Pedro León Miranda [GitHub](https://github.com/P4179)
- Francisco Mollá Astrar [GitHub](https://github.com/frmolla)
- Nanxi Qin [GitHub](https://github.com/NanxiQin)
- María Sachez Carrasco [GitHub](https://github.com/marsache)
- Rocío Sánchez Horcajuelo López [GitHub](https://github.com/Bimbloc)
- Santiago Tienda Perus [GitHub](https://github.com/Santienper)
- Laura Wang Qiu [GitHub](https://github.com/LauraWangQiu)
- Jianuo Wen [GitHub](https://github.com/Jjianuo)
- Likang Wu [GitHub](https://github.com/likangwu03)

## Descripción

<p align="justify">
El motor de videojuegos <strong>Tapioca Engine</strong> es un proyecto de software libre desarrollado por estudiantes de la Universidad Complutense de Madrid. El motor está siendo desarrollado en el marco de la asignatura de <strong>Proyectos 3</strong> del Grado en Desarrollo de Videojuegos.
</p>

## Arquitectura y proceso de desarrollo

<p align="justify">
<strong>Tapioca Engine</strong> está desarrollado en <strong>C++</strong> con <strong>Visual Studio</strong>.

El motor se divide en varios módulos que se encargan de diferentes aspectos del desarrollo de videojuegos. Estos módulos son:

- <strong>Bridge</strong>: Módulo encargado de la creación de escenas, entidades y componentes cargados desde archivos .lua
- <strong>Core</strong>: Módulo encargado de la creación de las factorías, la gestión de eventos y bucle principal
- <strong>Graphics</strong>: Módulo encargado del renderizado de los gráficos
- <strong>Input</strong>: Módulo encargado de la gestión de la entrada
- <strong>Physics</strong>: Módulo encargado de la simulación física
- <strong>Audio</strong>: Módulo encargado de la gestión del audio (sin implementar aún)
- <strong>UI</strong>: Módulo encargado de la gestión de la interfaz gráfica
- <strong>Main</strong>: Módulo encargado de crear los módulos anteriores

Los motores de tecnología hacen uso de diferentes bibliotecas para su funcionamiento. <strong>Tapioca Engine</strong> se hace uso de las siguientes:

- [OGRE](https://www.ogre3d.org/): renderizado de los gráficos
- [SDL](https://www.libsdl.org/): gestión de la entrada
- [Dear ImGui](https://github.com/ocornut/imgui): interfaz gráfica
- [Bullet](https://github.com/bulletphysics/bullet3): simulación física
- [FMOD](https://fmod.com/): gestión del audio
- [LUA](https://www.lua.org/): scripting

Se sigue una arquitectura basada en entidades y componentes (EC), y se hace uso de patrones de diseño, especialmente el patrón de factoría o <strong>Factory Method</strong>.

El proceso de desarrollo del motor sigue una metodología ágil, concretamente <strong>Scrum</strong>. Para ver el tablero de tareas, haz click [aquí](https://github.com/orgs/UCM-FDI-DISIA/projects/38/views/1)
</p>

## Pipeline de generación de contenido

### Motor

<p align="justify">
Los distintos módulos que componen el motor se desarrollarán en la rama main y se crearán ramas separadas únicamente para la implementación que en el momento no se pueda compilar.
</p>

### Juegos

<p align="justify">
La funcionalidad del juego vendrá dada por los componentes que conforman sus entidades. Siempre habrá una versión estable del motor que se podrá utilizar para probar el correcto funcionamiento de los componentes de juego que se vayan desarrollando. El proyecto contará con 2 juegos (ambos se desarrollarán en paralelo) y a medida que se amplíen las funcionalidades de motor se podrán incorporar nuevos elementos al juego.
</p>

Los links a los repositorios de los juegos son los siguientes:

- [Billiards adrift](https://github.com/UCM-FDI-DISIA/BilliardsAdrift)
- [Mar.io](https://github.com/UCM-FDI-DISIA/Mar.io)

### Assets

<ul>
<li>
<p align="justify">
<strong>Assets visuales 2D</strong>: se utilizarán imágenes en formato <strong>.png</strong> para que se respeten las transparencias correctamente con 72 dpi que es la resolución óptima para pantallas.
</p>
</li>
<li>
<p align="justify">
<strong>Assets visuales 3D</strong>: OGRE no permite trabajar  directamente con .fbx o .glb como otros motores, si no que trabaja con sus propios formatos usando .mesh para mallas , .material para materiales y .skeleton para los rigs.

Estos formatos <strong>no</strong> son los más utilizados y la mayoría de assets que se pueden encontrar online (sean gratis o de pago) suelen estar en formatos como .fbx. Para obtener contenido utilizable para nuestro motor de render usaremos [Blender](https://www.blender.org/) y el plugin gratuito [blender2Ogre](https://github.com/OGRECave/blender2ogre).

Con <strong>blender2Ogre</strong> podremos importar assets de terceros o crear las nuestras propias con Blender y exportar  las mallas en .mesh y además se exporta un .mesh.xml más legible para leer donde se especifican los vértices de la malla y el esqueleto que tiene asociado.

Los materiales en .material de acuerdo con los estándares .glTF (glTF 2.0 — Blender Manual) implica que no todo lo que podemos hacer en Blender con materiales se exportará correctamente, pero sí los atributos básicos como el color y las componentes metálica y difusa.
</p>
</li>
<li>
<p align="justify">
<strong>Assets sonoros</strong>: existe multitud de formatos de audio y uno de los más usados sin pérdida de calidad es el .wav que además es un formato que se usa de forma muy extendida.

En el caso de encontrar assets en otros formatos como mp3 será sencillo usar programas como [Audacity](https://www.audacityteam.org/) o [VLC Media](https://www.videolan.org/vlc/download-windows.html) para convertirlos a .wav.
</p>
</li>
</ul>

<hr>

<div align="center">
<h3>¡Síguenos en nuestras redes sociales!</h3>

<a href="https://www.youtube.com/channel/UCsvZMJ8wfEXIIFbLuvrqQ-g" target="_blank"><img src="https://img.shields.io/youtube/channel/subscribers/UCsvZMJ8wfEXIIFbLuvrqQ-g?label=Bubble Studios&logo=youtube&style=plastic" alt="YouTube Badge"/></a><a href="https://twitter.com/BubbleStudios24" target="_blank"><img src="https://img.shields.io/twitter/follow/BubbleStudios24?color=blue&label=Bubble Studios&logo=twitter&style=plastic" alt="Twitter Badge"/></a>

<h3>¡Contáctanos por correo!</h3>
<a href="mailto:contactbubblestudios@gmail.com" target="_blank">
    <img src="https://img.shields.io/badge/CONTACTO-green?style=plastic" alt="Contact Badge"/>
</a>
</div>
