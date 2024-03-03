- controls --> archivos del mapeo de controles
- materials --> materiales comunes
- racoon --> archivos del mesh del mapache (también usa white.material, que está en materials)
- scenes --> archivo .lua con la información de las escenas, gameobjects y componentes
- El resto de archivos son de configuración y tienen que estar en assets

INFORMACIÓN RUTAS
- Los MESHES se referencian a partir de la carpeta assets. Por ejemplo, si quiero usar mapache.mesh, que está en el directorio racoon, tendría que usar "racoon/mapache.mesh"
- Los MATERIALES se referencian siempre igual independientemente de la carpeta en que encuentran. Por ejemplo, si quiero usar el material white.material, sin importar en la carpeta en la que está, uso "white".
- Las TEXTURAS se indican desde el archivo.material que las utiliza. Se deben referenciar desde la carpeta assets, sin importar donde se encuentra el .material que las utiliza. Por ejemplo, si racoon.material utiliza ratex.png, estando ambos en el mismo directorio racoon, la ruta que tendría que usar es "racoon/ratex.png".
