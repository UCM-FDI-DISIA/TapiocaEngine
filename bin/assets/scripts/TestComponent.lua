comp.start = function(self)
	print("Hola!")
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