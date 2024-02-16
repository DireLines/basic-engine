package main

import "core:fmt"
import "core:slice"
import "core:strings"
import "core:time"


frames_per_sec :: 120
ms_per_frame :: (1.0 / frames_per_sec) * 1000
frame_counter: u64
average_frame_length: f64

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


Component :: enum {
    Transform,
}

GameObject :: struct {
    name:          string,
    component_set: bit_set[Component],
    parent:        ^GameObject,
    children:      [dynamic]^GameObject,
    script:        Script,
}
