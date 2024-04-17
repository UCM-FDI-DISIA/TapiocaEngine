comp.start = function(self)
	comp.time = 0
end

function comp:initComponent(variables)
	if type(variables.changeTime) ~= "number" then
		error("La variable changeTime no es un numero.")
	end
	comp.changeTime = variables.changeTime
	comp.gameName = variables.gameName
end

function comp:update(deltaTime)
	if comp.time >= comp.changeTime and not comp.done then
		if not Tapioca.initGame(comp.gameName) then
			Tapioca.exit()
		end
		Tapioca.deleteScene(Tapioca.getMainSceneName())
		comp.done = true
	elseif not comp.done then
		comp.time = comp.time + deltaTime
	end
end