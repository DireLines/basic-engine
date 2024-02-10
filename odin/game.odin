package main

import "core:fmt"
import "core:strings"
import "core:time"

import "vendor:raylib"

import "rigidbody"
import "transform"


frames_per_sec :: 120
ms_per_frame :: (1.0 / frames_per_sec) * 1000
frame_counter: u64
average_frame_length: f64

Game :: struct {
    game_timer:    GameTimer,
    id_generator:  IDGenerator,
    window_width:  i32,
    window_height: i32,
    input_system:  ^Input,
    systems:       [dynamic]^System,
    textures:      map[string]raylib.Texture2D,
    objects:      [dynamic]GameObject,
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
    game.input_system = input_system()
    game.objects = make([dynamic]GameObject)
    game.textures = make(map[string]raylib.Texture2D)
    physics_system := physics_system()
    collision_system := collision_system()
    script_runner := script_runner()
    renderer := renderer()
    add_systems(game, physics_system, collision_system, script_runner, renderer)
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
    if sprite_filename in game.textures{
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
    game.game_timer = GameTimer{}
    start_tick := time.tick_now()
    game.start_tick = start_tick
    should_quit := false
    for !should_quit {
        now := time.tick_now()
        duration := time.duration_milliseconds(time.tick_diff(start_tick, now))
        if duration > ms_per_frame {
            average_frame_length += duration
            start_tick = now
            game.game_timer.delta_time = duration / 1000
            game.game_timer.time += game.game_timer.delta_time
            game.input_system->update(game)
            should_quit = update(game)
            game.input_system->postupdate(game)
        }
    }
}
update :: proc(game: ^Game) -> (should_quit: bool) {
    using game
    timer := timer()
    for system in systems {
        system->update(game)
        timer->time(system.name)
    }
    //TODO: clear(objects_to_delete)
    frame_counter += 1
    return raylib.WindowShouldClose()
}
instantiate :: proc(game: ^Game, obj: GameObject) {
    using game
    //TODO instantiate child GameObjects
    append(&game.objects, obj)
    new_obj := &game.objects[len(game.objects)-1]
    for system in game.systems {
        if system->needObject(game, new_obj) {
            system->addObject(game, new_obj)
        }
    }
}
destroy :: proc(game: ^Game, obj: ^GameObject) {
    for system in game.systems {
        system->removeObject(game, obj)
    }
}
