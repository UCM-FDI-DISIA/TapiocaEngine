local serpent = require("assets/serpent")

function test(tab)
	print(serpent.block(tab))
end

function testMeta(tab)
	print(serpent.block(getmetatable(tab)))
end

print("Cargado test.lua")