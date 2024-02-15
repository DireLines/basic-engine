package main

import "core:fmt"
import "core:math"
import glm "core:math/linalg/glsl"
import "core:time"
import "vendor:raylib"


System :: struct {
    name:              string,
    components_needed: bit_set[Component],
    start:             proc(system: ^System, game: ^Game),
    update:            proc(system: ^System, game: ^Game),
    needObject:        proc(system: ^System, game: ^Game, obj_index: int) -> bool,
}
script_runner :: proc() -> ^System {
    return new_clone(
        System {
            name = "ScriptRunner",
            components_needed = {.Script},
            start = proc(system: ^System, game: ^Game) {
            },
            update = proc(system: ^System, game: ^Game) {
            },
            needObject = proc(system: ^System, game: ^Game, obj_index: int) -> bool {
                // making it return true fixes one of the bugs:
                // src/array.cpp(61): Assertion Failure: `cast(usize)index < cast(usize)count` Index 5 is out of bounds ranges 0..<3\\n
                //return true

                //bug lurks somewhere here
                obj := game.objects[obj_index]
                return has_desired_components(&obj, system.components_needed)
            },
        },
    )
}
