package main

import "core:fmt"
import "core:slice"
import "core:strings"
import "core:time"

import "vendor:raylib"


frames_per_sec :: 120
ms_per_frame :: (1.0 / frames_per_sec) * 1000
frame_counter: u64
average_frame_length: f64

Game :: struct {
    id_generator:  IDGenerator,
    window_width:  i32,
    window_height: i32,
    input_system:  ^Input,
    systems:       [dynamic]^System,
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

add_system :: proc(game: ^Game, system: ^System) {
    append(&game.systems, system)
}

add_systems :: proc(game: ^Game, systems: ..^System) {
    for system in systems {
        add_system(game, system)
    }
}
