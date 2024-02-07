package main

import "core:fmt"
import "core:time"

import SDL "vendor:sdl2"
import SDL_IMG "vendor:sdl2/image"
import SDL_TTF "vendor:sdl2/ttf"

import "ecs"

frames_per_sec :: 120
ms_per_frame :: (1.0 / frames_per_sec) * 1000
frame_counter: u64
average_frame_length: f64

Game :: struct {
    game_timer: GameTimer,
    renderer:      ^SDL.Renderer,
    window:        ^SDL.Window,
    id_generator:  IDGenerator,
    window_width:  i32,
    window_height: i32,
    input_system:  ^Input,
    systems:       [dynamic]^System,
    objects:       ecs.Context,
    start_tick:    time.Tick,
}
new_game :: proc(window_width, window_height: i32) -> Game {
    game := Game{}
    init(&game, window_width, window_height)
    return game
}
initSDL :: proc(game: ^Game) {
    SDL_TTF.Init()
    SDL_IMG.Init(SDL_IMG.INIT_PNG)
    SDL.Init(SDL.INIT_VIDEO)
    game.window = SDL.CreateWindow(
        "Game",
        SDL.WINDOWPOS_UNDEFINED,
        SDL.WINDOWPOS_UNDEFINED,
        game.window_width,
        game.window_height,
        {.OPENGL},
    )
    if game.window == nil {
        fmt.eprintln("Failed to create window")
        return
    }
}

quitSDL :: proc(game: ^Game) {
    print("Quitting sdl")
    SDL.DestroyWindow(game.window)
    SDL_IMG.Quit()
    SDL_TTF.Quit()
}

init :: proc(game: ^Game, window_width, window_height: i32) {
    game.id_generator = id_generator()
    game.window_width = window_width
    game.window_height = window_height
    game.input_system = input_system()
    game.objects = ecs.init_ecs()
    append(&game.systems, physics_system(), collision_system(), script_runner(), renderer())
    initSDL(game)
}

add_system :: proc(game: ^Game, system: ^System) {
    append(&game.systems, system)
    ecs.track_entities_with_components(&game.objects,system.components_needed[:])
}

add_systems :: proc(game: ^Game, systems: ..^System) {
    for system in systems {
        add_system(game, system)
    }
}

quit :: proc(game: ^Game) {
    using game
    average_frame_length /= f64(frame_counter)
    print("average frame took", average_frame_length, "milliseconds")
    average_fps := 1000.0 / average_frame_length
    print("target fps:", frames_per_sec)
    print("actual fps:", average_fps)
    quitSDL(game)
    ecs.deinit_ecs(&objects)
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
update :: proc(game: ^Game) -> bool {
    using game
    event: SDL.Event
    timer := timer()
    for system in systems {
        system->update(game)
        timer->time(system.name)
        SDL.PollEvent(&event)
        #partial switch event.type {
        case .KEYDOWN:
            #partial switch event.key.keysym.sym {
            case .ESCAPE:
                return true
            case:
                input_system->poll(event)
            // print(event.key.keysym.sym, "pressed")
            }
        case .QUIT:
            return true
        case:
            input_system->poll(event)
        }
    }
    //TODO: clear(objects_to_delete)
    frame_counter += 1
    return false
}
instantiate :: proc(game: ^Game, obj: ^GameObject) {
    using game
    using ecs
    entity := create_entity(&objects)
    //TODO instantiate child GameObjects
    for system in systems {
        if system->needObject(game, obj) {
            system->addObject(game, obj)
        }
    }
}
destroy :: proc(game: ^Game, obj: ^GameObject) {
    for system in game.systems {
        system->removeObject(game, obj)
    }
}
