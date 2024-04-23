-- No es necesario crear el start asi, esto es para mostrar lo que significa crear una funcion con ':' en vez de con '.'.
comp.start = function(self)
	print("Hola!")
end
-- Se pueden definir variables propias de la clase de esta manera, fuera de las funciones.
comp.time = 0

function comp:initComponent(variables)
	-- "test" es una funcion creada por nosotros usando una biblioteca para mostrar por pantalla toda variable.
	_internal.test(variables)
	-- Para usar los datos del objeto, usar "self".
	-- No usar "comp" dentro de las funciones, ya que es un acceso temporal a la tabla de la clase.
	self:createAlgo(variables.aaa)
	print(variables.aaa, "Hiiiii :3")
	-- Si se quiere devolver true o false como en C++, se puede.
	-- Tambien se puede no devolver nada en vez de devolver true.
	-- Tambien se puede mandar un error en vez de devolver false.
end

function comp:createAlgo(num, aaaaaa)
	-- Si se quiere comprobar un tipo, se puede hacer con la funcion "type".
	if type(num) ~= "number" then
		-- Para mandar un error, se usa la funcion "error", esto manda una excepcion capturada por el componente.
		error("La variable num no es un numero.")
	end
end

function comp:update(deltaTime)
	if self.time >= 2000 and not self.done then
		-- Por ahora, todas las funciones del motor estan dentro de la tabla "Tapioca".
		-- Esto hay que hablarlo entre todo el grupo.
		Tapioca.loadScene("StartScene2.lua")
		Tapioca.deleteScene(Tapioca.getMainSceneName())
		self.done = true
	elseif not self.done then
		self.time = self.time + deltaTime
	end
end