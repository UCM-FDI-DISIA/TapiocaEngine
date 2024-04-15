comp.start = function(self)
	print("Hola!")
end

function comp:initComponent(variables)
	test(variables)
end

function comp:createAlgo(num, aaaaaa)
	if typeof(num) ~= "number" then
		error("La variable num no es un numero.")
	end
end