package.path = ".\\TapiocaFiles\\Lua\\lua\\?.lua;.\\TapiocaFiles\\Lua\\lua\\?\\init.lua;.\\TapiocaFiles\\Lua\\?.lua;.\\TapiocaFiles\\Lua\\?\\init.lua;.\\assets\\scripts\\lua\\?.lua;.\\assets\\scripts\\lua\\?\\init.lua;.\\assets\\scripts\\?.lua;.\\assets\\scripts\\?\\init.lua;" .. package.path
--package.path = package.path .. ";.\\TapiocaFiles\\Lua\\?.dll;.\\assets\\scripts\\?.dll" -- No se si funciona

local serpent = require("serpent")

_internal = {
	showGlobal = { },
	nativePrint = print
}
setmetatable(_G, { __index = _internal.showGlobal })
print = nil

function _internal.test(tab)
	print(serpent.block(tab))
end

function _internal.testMeta(tab)
	print(serpent.block(getmetatable(tab)))
end

function _internal.callGlobal(name, parameters)
	_G[name](table.unpack(parameters))
end

function _internal.call(object, name, parameters)
	object[name](object, table.unpack(parameters))
end
