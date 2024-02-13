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
    // input_system:  ^Input,
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

add_sprite :: proc(game: ^Game, sprite_filename: string) -> ^raylib.Texture2D {
    if sprite_filename in game.textures {
        return &game.textures[sprite_filename]
    }
    game.textures[sprite_filename] = raylib.LoadTexture(strings.clone_to_cstring(sprite_filename))
    return &game.textures[sprite_filename]
}

quit :: proc(game: ^Game) {
    using game
    quit_raylib(game)
    average_frame_length /= f64(frame_counter)
    print("average frame took", average_frame_length, "milliseconds")
    average_fps := 1000.0 / average_frame_length
    print("target fps:", frames_per_sec)
    print("actual fps:", average_fps)
}

quit_raylib :: proc(game: ^Game) {
    using raylib
    CloseWindow()
}

start :: proc(game: ^Game) {
    for system in game.systems {
        system->start(game)
    }
    initialize(game)
    start_tick := time.tick_now()
    game.start_tick = start_tick
    should_quit := false
    for !should_quit {
        now := time.tick_now()
        duration := time.duration_milliseconds(time.tick_diff(start_tick, now))
        if duration > ms_per_frame {
            average_frame_length += duration
            start_tick = now
            should_quit = update(game)
        }
    }
}
update :: proc(game: ^Game) -> (should_quit: bool) {
    using game
    for system in systems {
        system->update(game)
    }
    //TODO: clear(objects_to_delete)
    frame_counter += 1
    return raylib.WindowShouldClose()
}
instantiate :: proc(game: ^Game, obj: GameObject) -> int {
    using game
    //TODO instantiate child GameObjects
    append_soa(&game.objects, obj)
    index := len(game.objects) - 1
    for system in game.systems {
        if system->needObject(game, index) {
            system->addObject(game, index)
        }
    }
    return index
}
destroy :: proc(game: ^Game, obj_index: int) {
    for system in game.systems {
        system->removeObject(game, obj_index)
    }
    game.objects[obj_index] = {}
}
