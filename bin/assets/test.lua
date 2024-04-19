local serpent = require("assets/serpent")

function test(tab)
	print(serpent.block(tab))
end

function testMeta(tab)
	print(serpent.block(getmetatable(tab)))
end

_internal = { }

function _internal.call(name, parameters)
	_G[name](table.unpack(parameters))
end

print("Cargado test.lua")