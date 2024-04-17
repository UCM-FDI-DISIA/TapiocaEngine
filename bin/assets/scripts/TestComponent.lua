comp.start = function(self)
	print("Hola!")
	comp.time = 0
end

function comp:initComponent(variables)
	test(variables)
	self:createAlgo(variables.aaa)
	print(variables.aaa.."")
end

function comp:createAlgo(num, aaaaaa)
	if type(num) ~= "number" then
		error("La variable num no es un numero.")
	end
end

function comp:update(deltaTime)
	if comp.time >= 2000 and not comp.done then
		Tapioca.loadScene("archivo.lua")
		comp.done = true
	elseif not comp.done then
		comp.time = comp.time + deltaTime
	end
end