![](TapiocaEngine_logo.png)

## Autores

- [Matt Castellanos Silva](https://github.com/MattCastUCM)
- [Pedro León Miranda](https://github.com/P4179)
- [Francisco Mollá Astrar](https://github.com/frmolla)
- [Nanxi Qin](https://github.com/NanxiQin)
- [María Sachez Carrasco](https://github.com/marsache)
- [Rocío Sánchez Horcajuelo López](https://github.com/Bimbloc)
- [Santiago Tienda Perus](https://github.com/Santienper)
- [Laura Wang Qiu](https://github.com/LauraWangQiu)
- [Jianuo Wen](https://github.com/Jjianuo)
- [Likang Wu](https://github.com/likangwu03)

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
- <strong>Audio</strong>: Módulo encargado de la gestión del audio
- <strong>UI</strong>: Módulo encargado de la gestión de la interfaz gráfica
- <strong>Exports</strong>: Módulo encargado de crear una librería dinámica que incluye los módulos anteriores
- <strong>Main</strong>: Módulo encargado de ejecutar el motor

Los motores de tecnología hacen uso de diferentes bibliotecas para su funcionamiento. <strong>Tapioca Engine</strong> se hace uso de las siguientes:

- [OGRE](https://www.ogre3d.org/): renderizado de los gráficos
- [SDL](https://www.libsdl.org/): gestión de la entrada
- [Dear ImGui](https://github.com/ocornut/imgui): interfaz gráfica
- [Bullet](https://github.com/bulletphysics/bullet3): simulación física
- [IrrKlang](https://www.ambiera.com/irrklang/): gestión del audio
- [LUA](https://www.lua.org/): scripting
- [LUABridge](https://github.com/vinniefalco/LuaBridge): integración de LUA en C++

Se sigue una arquitectura basada en entidades y componentes (EC), y se hace uso de patrones de diseño, especialmente el patrón de factoría o <strong>Factory Method</strong>.

El proceso de desarrollo del motor sigue una metodología ágil, concretamente <strong>Scrum</strong>. Para ver el tablero de tareas, haz click [aquí](https://github.com/orgs/UCM-FDI-DISIA/projects/38/views/1)
</p>

## Pipeline de generación de contenido

### Motor

<p align="justify">
Los distintos módulos que componen el motor han sido desarrollados en la rama main, utilizando ramas separadas únicamente para subir código que no compilaba en ese momento.
</p>

### Juegos

<p align="justify">
La funcionalidad de cada juego viene dada por los componentes que conforman sus entidades. El proyecto cuenta con 2 juegos (ambos desarrollados en paralelo), y a medida que se han ido ampliando las funcionalidades de motor se han ido incorporando nuevos elementos al juego.
</p>

Los links a los repositorios de los juegos son los siguientes:

- [Billiards adrift](https://github.com/UCM-FDI-DISIA/BilliardsAdrift)
- [Mar.io](https://github.com/UCM-FDI-DISIA/Mar.io)

### Assets

<ul>
<li>
<p align="justify">
<strong>Assets visuales 2D</strong>: se utilizan imágenes en formato <strong>.png</strong> para que se respeten las transparencias correctamente, con 72 dpi, que es la resolución óptima para pantallas.
</p>
</li>
<li>
<p align="justify">
<strong>Assets visuales 3D</strong>: OGRE no permite trabajar directamente con .fbx o .glb como otros motores, si no que trabaja con sus propios formatos usando .mesh para mallas, .material para materiales y .skeleton para los rigs.

Estos formatos <strong>no</strong> son los más utilizados y la mayoría de assets que se pueden encontrar online (sean gratis o de pago) suelen estar en formatos como .fbx. Para obtener contenido utilizable para nuestro motor de render hemos utilizado [Blender](https://www.blender.org/) y el plugin gratuito [blender2Ogre](https://github.com/OGRECave/blender2ogre).

Con <strong>blender2Ogre</strong> hemos importado assets de terceros o creado los nuestros propios con <strong>Blender</strong>, y hemos exportado las mallas en .mesh. Además, también se exporta un .mesh.xml más legible para leer donde se especifican los vértices de la malla y el esqueleto que tiene asociado.

Los materiales en .material de acuerdo con los estándares .glTF (glTF 2.0 — Blender Manual) implica que no todo lo que podemos hacer en Blender con materiales se exportará correctamente, pero sí los atributos básicos como el color y las componentes metálica y difusa.
</p>
</li>
<li>
<p align="justify">
<strong>Assets sonoros</strong>: Existe multitud de formatos de audio en nuestro caso hemos utilizado el formato .mp3, ya que <strong>IrrKlang</strong> soporta dicho formato de manera nativa.
</p>
</li>
</ul>

## Guía de uso del mapeo de teclas

Para el mapeo de controles, se usa un archivo llamado `controlsMapping.Lua` con el siguiente formato:
```
events = {
        ev_NOMBRE_EVENTO = {
            src = origen_del_evento,
            control = valorDelInput
        },
        ...
}
```
Para mapear el mismo evento a varios controles, hay que añadir al final del nombre del evento un `_N`, siendo N un número que lo diferencie del resto de eventos con el mismo nombre.

<br>

Los posibles valores de src son los siguientes:
```
ie_closeWindow          cerrar vebtaba

ie_keyUp                pulsar tecla
ie_keyDown              soltar tecla

ie_mouseMoving          mover ratón
ie_mouseButtonUp        soltar botón del ratón
ie_mouseButtonDown      pulsar botón del ratón
ie_mouseWheel           girar rueda del ratón

ie_ctrlAxisMotion       mover eje del mando
ie_ctrlButtonUp         soltar botón del mando
ie_ctrlButtonDown       pulsar botón del mando
```

Y los posibles valores de control son los siguientes:
- Teclado:
```
UNKNOWN = 0

RETURN = 13
ESCAPE = 27
BACKSPACE = 8
TAB = 9
SPACE = 32
EXCLAIM = 33
QUOTEDBL = 34
HASH = 35
PERCENT = 37
DOLLAR = 36
AMPERSAND = 38
QUOTE = 39
LEFTPAREN = 40
RIGHTPAREN = 41
ASTERISK = 42
PLUS = 43
COMMA = 44
MINUS = 45
PERIOD = 46
SLASH = 47

0 = 48
1 = 49
2 = 50
3 = 51
4 = 52
5 = 53
6 = 54
7 = 55
8 = 56
9 = 57

COLON = 58
SEMICOLON = 59
LESS = 60
EQUALS = 61
GREATER = 62
QUESTION = 63
AT = 64
LEFTBRACKET = 91
BACKSLASH = 92
RIGHTBRACKET = 93
CARET = 94
UNDERSCORE = 95
BACKQUOTE = 96

a = 97
b = 98
c = 99
d = 100
e = 101
f = 102
g = 103
h = 104
i = 105
j = 106
k = 107
l = 108
m = 109
n = 110
o = 111
p = 112
q = 113
r = 114
s = 115
t = 116
u = 117
v = 118
w = 119
x = 120
y = 121
z = 122

CAPSLOCK = 1073741881

F1 = 1073741882
F2 = 1073741883
F3 = 1073741884
F4 = 1073741885
F5 = 1073741886
F6 = 1073741887
F7 = 1073741888
F8 = 1073741889
F9 = 1073741890
F10 = 1073741891
F11 = 1073741892
F12 = 1073741893

PRINTSCREEN = 1073741894
SCROLLLOCK = 1073741895
PAUSE = 1073741896
INSERT = 1073741897
HOME = 1073741898
PAGEUP = 1073741899
DELETE = 1073741900
END = 1073741901
PAGEDOWN = 1073741902
RIGHT = 1073741903
LEFT = 1073741904
DOWN = 1073741905
UP = 1073741906

NUMLOCKCLEAR = 1073741907
KP_DIVIDE = 1073741908
KP_MULTIPLY = 1073741909
KP_MINUS = 1073741910
KP_PLUS = 1073741911
KP_ENTER = 1073741912
KP_1 = 1073741913
KP_2 = 1073741914
KP_3 = 1073741915
KP_4 = 1073741916
KP_5 = 1073741917
KP_6 = 1073741918
KP_7 = 1073741919
KP_8 = 1073741920
KP_9 = 1073741921
KP_0 = 1073741922
KP_PERIOD = 1073741923

APPLICATION = 1073741925
POWER = 1073741926
KP_EQUALS = 1073741927
F13 = 1073741928
F14 = 1073741929
F15 = 1073741930
F16 = 1073741931
F17 = 1073741932
F18 = 1073741933
F19 = 1073741934
F20 = 1073741935
F21 = 1073741936
F22 = 1073741937
F23 = 1073741938
F24 = 1073741939
EXECUTE = 1073741940
HELP = 1073741941
MENU = 1073741942
SELECT = 1073741943
STOP = 1073741944
AGAIN = 1073741945
UNDO = 1073741946
CUT = 1073741947
COPY = 1073741948
PASTE = 1073741949
FIND = 1073741950
MUTE = 1073741951
VOLUMEUP = 1073741952
VOLUMEDOWN = 1073741953
KP_COMMA = 1073741957
KP_EQUALSAS400 = 1073741958

ALTERASE = 1073741977
SYSREQ = 1073741978
CANCEL = 1073741979
CLEAR = 1073741980
PRIOR = 1073741981
RETURN2 = 1073741982
SEPARATOR = 1073741983
OUT = 1073741984
OPER = 1073741985
CLEARAGAIN = 1073741986
CRSEL = 1073741987
EXSEL = 1073741988

KP_00 = 1073741989
KP_000 = 1073741990
THOUSANDSSEPARATOR = 1073741991
DECIMALSEPARATOR = 1073741992
CURRENCYUNIT = 1073741993
CURRENCYSUBUNIT = 1073741994
KP_LEFTPAREN = 1073741995
KP_RIGHTPAREN = 1073741996
KP_LEFTBRACE = 1073741997
KP_RIGHTBRACE = 1073741998
KP_TAB = 1073741999
KP_BACKSPACE = 1073742000
KP_A = 1073742001
KP_B = 1073742002
KP_C = 1073742003
KP_D = 1073742004
KP_E = 1073742005
KP_F = 1073742006
KP_XOR = 1073742007
KP_POWER = 1073742008
KP_PERCENT = 1073742009
KP_LESS = 1073742010
KP_GREATER = 1073742011
KP_AMPERSAND = 1073742012
KP_DBLAMPERSAND = 1073742013
KP_VERTICALBAR = 1073742014
KP_DBLVERTICALBAR = 1073742015
KP_COLON = 1073742016
KP_HASH = 1073742017
KP_SPACE = 1073742018
KP_AT = 1073742019
KP_EXCLAM = 1073742020
KP_MEMSTORE = 1073742021
KP_MEMRECALL = 1073742022
KP_MEMCLEAR = 1073742023
KP_MEMADD = 1073742024
KP_MEMSUBTRACT = 1073742025
KP_MEMMULTIPLY = 1073742026
KP_MEMDIVIDE = 1073742027
KP_PLUSMINUS = 1073742028
KP_CLEAR = 1073742029
KP_CLEARENTRY = 1073742030
KP_BINARY = 1073742031
KP_OCTAL = 1073742032
KP_DECIMAL = 1073742033
KP_HEXADECIMAL = 1073742034

LCTRL = 1073742035
LSHIFT = 1073742036
LALT = 1073742037
LGUI = 1073742038
RCTRL = 1073742039
RSHIFT = 1073742040
RALT = 1073742041
RGUI = 1073742042

MODE = 1073742043
AUDIONEXT = 1073742044
AUDIOPREV = 1073742045
AUDIOSTOP = 1073742046
AUDIOPLAY = 1073742047
AUDIOMUTE = 1073742048
MEDIASELECT = 1073742049
WWW = 1073742050
MAIL = 1073742051
CALCULATOR = 1073742052
COMPUTER = 1073742053
AC_SEARCH = 1073742054
AC_HOME = 1073742055
AC_BACK = 1073742056
AC_FORWARD = 1073742057
AC_STOP = 1073742058
AC_REFRESH = 1073742059
AC_BOOKMARKS = 1073742060
BRIGHTNESSDOWN = 1073742061
BRIGHTNESSUP = 1073742062
DISPLAYSWITCH = 1073742063
KBDILLUMTOGGLE = 1073742064
KBDILLUMDOWN = 1073742065
KBDILLUMUP = 1073742066
EJECT = 1073742067
SLEEP = 1073742068
APP1 = 1073742069
APP2 = 1073742070

AUDIOREWIND = 1073742071
AUDIOFASTFORWARD = 1073742072

SOFTLEFT = 1073742073
SOFTRIGHT = 1073742074
CALL = 1073742075
ENDCALL = 1073742076
```

- Ratón:
```
BUTTON_LEFT = 1
BUTTON_MIDDLE = 2
BUTTON_RIGHT = 3
BUTTON_X1 = 4
BUTTON_X2 = 5
```

- Ejes del mando:
```
CONTROLLER_BUTTON_INVALID = -1
CONTROLLER_BUTTON_A = 0
CONTROLLER_BUTTON_B = 1
CONTROLLER_BUTTON_X = 2
CONTROLLER_BUTTON_Y = 3
CONTROLLER_BUTTON_BACK = 4
CONTROLLER_BUTTON_GUIDE = 5
CONTROLLER_BUTTON_START = 6
CONTROLLER_BUTTON_LEFTSTICK = 7
CONTROLLER_BUTTON_RIGHTSTICK = 8
CONTROLLER_BUTTON_LEFTSHOULDER = 9
CONTROLLER_BUTTON_RIGHTSHOULDER = 10
CONTROLLER_BUTTON_DPAD_UP = 11
CONTROLLER_BUTTON_DPAD_DOWN = 12
CONTROLLER_BUTTON_DPAD_LEFT = 13
CONTROLLER_BUTTON_DPAD_RIGHT = 14
CONTROLLER_BUTTON_MISC1 = 15            /* Xbox Series X share button PS5 microphone button Nintendo Switch Pro capture button Amazon Luna microphone button */
CONTROLLER_BUTTON_PADDLE1 = 16          /* Xbox Elite paddle P1 (upper left facing the back) */
CONTROLLER_BUTTON_PADDLE2 = 17          /* Xbox Elite paddle P3 (upper right facing the back) */
CONTROLLER_BUTTON_PADDLE3 = 18          /* Xbox Elite paddle P2 (lower left facing the back) */
CONTROLLER_BUTTON_PADDLE4 = 19          /* Xbox Elite paddle P4 (lower right facing the back) */
CONTROLLER_BUTTON_TOUCHPAD = 20         /* PS4/PS5 touchpad button */
CONTROLLER_BUTTON_MAX = 21
```

- Botones del mando:
```
CONTROLLER_AXIS_INVALID = -1
CONTROLLER_AXIS_LEFTX = 0
CONTROLLER_AXIS_LEFTY = 1
CONTROLLER_AXIS_RIGHTX = 2
CONTROLLER_AXIS_RIGHTY = 3
CONTROLLER_AXIS_TRIGGERLEFT = 4
CONTROLLER_AXIS_TRIGGERRIGHT = 5
CONTROLLER_AXIS_MAX = 6
```

<hr>

<div align="center">
<h3>¡Síguenos en nuestras redes sociales!</h3>

<a href="https://www.youtube.com/channel/UCsvZMJ8wfEXIIFbLuvrqQ-g" target="_blank"><img src="https://img.shields.io/youtube/channel/subscribers/UCsvZMJ8wfEXIIFbLuvrqQ-g?label=Bubble Studios&logo=youtube&style=plastic" alt="YouTube Badge"/></a><a href="https://twitter.com/BubbleStudios24" target="_blank"><img src="https://img.shields.io/twitter/follow/BubbleStudios24?color=blue&label=Bubble Studios&logo=twitter&style=plastic" alt="Twitter Badge"/></a>

<h3>¡Contáctanos por correo!</h3>
<a href="mailto:contactbubblestudios@gmail.com" target="_blank">
    <img src="https://img.shields.io/badge/CONTACTO-green?style=plastic" alt="Contact Badge"/>
</a>
</div>
