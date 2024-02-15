package main

import "core:fmt"
import "core:math"
import glm "core:math/linalg/glsl"
import "core:time"
import "vendor:raylib"


System :: struct {
    needObject: proc(system: ^System, game: ^Game, obj_index: int) -> bool,
}
script_runner :: proc() -> System {
    return System{needObject = proc(system: ^System, game: ^Game, obj_index: int) -> bool {
                obj := game.objects[obj_index] //bug lurks somewhere here
                // removing this line fixes one of the errors:
                // src/array.cpp(61): Assertion Failure: `cast(usize)index < cast(usize)count` Index 5 is out of bounds ranges 0..<3\\n
                return true
            }}
}
