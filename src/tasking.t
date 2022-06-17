local function c_returntype(ty)
    assert(terralib.types.istype(ty), "Requires a Terra type")
    local ty = ty.returntype
    if ty == nil or tostring(ty) == "{}" then
        return "void"
    else
        return ty:cstring()
    end
end

local function c_type(ty)
    assert(terralib.types.istype(ty), "Requires a Terra type")
    local name, n = ty:cstring():gsub("ptr_", "")
    return name .. ("*"):rep(n)
end

local function c_symbol(ty)
    assert(terralib.types.istype(ty), "Requires a Terra type")
    return tostring(symbol(ty))
end

local function make_task(name, ty)
    local rettype = c_returntype(ty)
    local params = ty.parameters
    local ctypes = terralib.newlist()
    local args = terralib.newlist()
    for i = 1, #params do
        local sym, ctype = c_symbol(params[i]), c_type(params[i])
        ctypes:insert(ctype .. " " .. sym)
        args:insert(sym)
    end

    local function emit_fundecl(file)
        file:write(("%s %s(%s);\n"):format(rettype, name, ctypes:concat(", ")))
    end

    local function emit_async_macro(file)
        if rettype == "void" then
            file:write(("ASYNC_DECL(%s, %s, %s);\n"):
                format(name, ctypes:concat("; "), args:concat(", ")))
        else
            file:write(("FUTURE_DECL_FREELIST(%s);\n"):format(rettype))
            file:write(("FUTURE_DECL(%s, %s, %s, %s);\n"):
                format(rettype, name, ctypes:concat("; "), args:concat(", ")))
        end
    end

    local function emit_async_fundecl(file)
        if rettype == "void" then
            file:write(("void async_%s(%s);\n"):format(name, ctypes:concat(", ")))
        else
            file:write(("future future_%s(%s);\n"):format(name, ctypes:concat(", ")))
            file:write(("%s await_%s(future f);\n"):format(rettype, name))
        end
    end

    local function emit_async_fundef(file)
        if rettype == "void" then
            file:write(("void async_%s(%s) { ASYNC(%s, %s); }\n"):
                format(name, ctypes:concat(", "), name, args:concat(", ")))
        else
            file:write(("future future_%s(%s) { return FUTURE(%s, %s); }\n"):
                format(name, ctypes:concat(", "), name, args:concat(", ")))
            file:write(("%s await_%s(future f) { return AWAIT(f, %s); }\n"):
                format(rettype, name, rettype))
        end
    end

    local sourcename = ("terra_tasking_%s.c"):format(name)
    local headername = ("terra_tasking_%s.h"):format(name)

    local header = assert(io.open("../include/" .. headername, "w"))
    header:write "#include <stdint.h>\n"
    header:write "#include \"tasking.h\"\n"

    local source = assert(io.open(sourcename, "w"))
    source:write("#include \"" .. headername .. "\"\n")

    emit_async_fundecl(header)
    emit_fundecl(source)
    emit_async_macro(source)
    emit_async_fundef(source)

    header:close()
    source:close()

    local C = terralib.includec("../include/" .. headername)
    for k, v in pairs(C) do
        local name = k:match("^async_([_%w]+)") or k:match("^future_([_%w]+)")
        if name ~= nil then
            async[name] = v
        else
            name = k:match("^await_([_%w]+)")
            if name ~= nil then
                await[name] = v
            end
        end
    end
end

tasking = {}
async = {}
await = {}

local C = terralib.includec "../include/terra_tasking.h"
for k, v in pairs(C) do
    local name = k:match("^terra_tasking_([_%w]+)")
    if name ~= nil then tasking[name] = v end
end

return { -- Parser extension
    name = "tasking",
    entrypoints = {"task"},
    keywords = {},
    statement = function (self, lex)
        lex:expect("task")
        local name = lex:expect(lex.name)
        lex:expect("::")
        local ty = lex:luaexpr()
        return function (env)
            make_task(name.value, ty(env()).type)
        end
    end
}
