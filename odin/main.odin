package main

import "core:fmt"
import "core:math"
import glm "core:math/linalg/glsl"
import "core:strings"
import "core:time"

vec2 :: [2]f32
mat3 :: matrix[3, 3]f32

//game-specific initialization code
initialize :: proc(game: ^Game) {
}
main :: proc() {
    base_width :: 2400
    game := new_game(window_width = base_width, window_height = (9.0 / 16.0) * base_width)
}

print :: fmt.println
printf :: fmt.printf

Input :: struct {
    buttons: map[u8]struct {}, //removing this field fixes one of the two errors:
    //    Internal Compiler Error: Type_Info for 'Tick' could not be found
}

System :: struct {
    name:              string,
    components_needed: bit_set[Component],
    start:             proc(system: ^System, game: ^Game),
    update:            proc(system: ^System, game: ^Game),
    needObject:        proc(system: ^System, game: ^Game, obj_index: int) -> bool,
}

Script :: struct {
    awake: proc(self_index: int, game: ^Game),
}
test_script :: proc() -> Script {
    test_awake :: proc(self_index: int, game: ^Game) {
        print("awake", self_index)
    }
    return Script{awake = test_awake}
}
