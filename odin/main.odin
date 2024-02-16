package main

import "core:fmt"
import "core:math"
import glm "core:math/linalg/glsl"
import "core:strings"
import "core:time"

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
    name:       string,
    start:      proc(system: ^System, game: ^Game),
    update:     proc(system: ^System, game: ^Game),
    needObject: proc(system: ^System, game: ^Game, obj_index: int) -> bool,
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

Game :: struct {
    window_width:  i32,
    window_height: i32,
    input_system:  ^Input,
    systems:       [dynamic]^System,
    objects:       #soa[dynamic]GameObject,
    start_tick:    time.Tick,
}

new_game :: proc(window_width, window_height: i32) -> Game {
    game := Game{}
    init(&game, window_width, window_height)
    return game
}

init :: proc(game: ^Game, window_width, window_height: i32) {
    game.window_width = window_width
    game.window_height = window_height
}


GameObject :: struct {
    name:   string,
    script: Script,
}
