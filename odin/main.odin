package main

import "core:fmt"
import "core:math"
import glm "core:math/linalg/glsl"
import "core:strings"
import "core:time"
import "vendor:raylib"

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

frames_per_sec :: 120
ms_per_frame :: (1.0 / frames_per_sec) * 1000
frame_counter: u64
average_frame_length: f64

Game :: struct {
    id_generator:  IDGenerator,
    window_width:  i32,
    window_height: i32,
    systems:       [dynamic]System,
    textures:      map[string]raylib.Texture2D,
    objects:       #soa[dynamic]GameObject,
    start_tick:    time.Tick,
}

new_game :: proc(window_width, window_height: i32) -> Game {
    game := Game{}
    init(&game, window_width, window_height)
    return game
}

init :: proc(game: ^Game, window_width, window_height: i32) {
    game.id_generator = id_generator()
    game.window_width = window_width
    game.window_height = window_height
    script_runner := script_runner()
    add_systems(game, script_runner)
    init_raylib(game)
}

init_raylib :: proc(game: ^Game) {
    using raylib
    SetTraceLogLevel(.NONE)
    InitWindow(game.window_width, game.window_height, "Game")
    SetTargetFPS(120)
}

add_system :: proc(game: ^Game, system: System) {
    append(&game.systems, system)
}

add_systems :: proc(game: ^Game, systems: ..System) {
    for system in systems {
        add_system(game, system)
    }
}

instantiate :: proc(game: ^Game, obj: GameObject) -> int {
    append_soa(&game.objects, obj) //removing this field fixes one of the two errors:
    // Internal Compiler Error: Type_Info for 'Tick' could not be found
    return 0
}

IDGenerator :: struct {
    id:   int,
    next: proc(_: ^IDGenerator) -> int,
}
id_generator :: proc() -> IDGenerator {
    return IDGenerator{id = 0, next = proc(gen: ^IDGenerator) -> int {
                gen.id += 1
                return gen.id
            }}
}
