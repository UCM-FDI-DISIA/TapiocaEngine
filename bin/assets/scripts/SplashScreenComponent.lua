-- Es mejor ponerlo en la tabla de clase
function comp:start()
	self.time = 0
end

comp.time = 0

function comp:initComponent(variables)
	if type(variables.changeTime) ~= "number" then
		error("La variable changeTime no es un numero.")
	end
	self.changeTime = variables.changeTime
	self.gameName = variables.gameName
end

function comp:update(deltaTime)
	if self.time >= self.changeTime and not self.done then
		if not Tapioca.initGame(self.gameName) then
			Tapioca.exit()
		end
		Tapioca.deleteScene(Tapioca.getMainSceneName())
		self.done = true
	elseif not self.done then
		self.time = self.time + deltaTime
	end
end