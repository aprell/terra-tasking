local C = terralib.includecstring [[
#include <stdio.h>
#include <stdlib.h>
]]

import "tasking"

task fib :: int -> int

terra check(test : bool)
    if not test then
        C.fprintf(C.stderr, "Check failed\n")
        C.abort()
    end
end

terra fib(n : int) : int
    if n < 2 then return n end
    var x = async.fib(n-1)
    var y = fib(n-2)
    return await.fib(x) + y
end

terra main(argc : int, argv : &rawstring)
    tasking.init(&argc, &argv)
    defer tasking.exit()
    check(fib(30) == 832040)
end

terralib.saveobj (
    "fib.o",
    {main = main, fib = fib}
)
