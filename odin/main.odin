package main

import "core:fmt"
import glm "core:math/linalg/glsl"
import "core:strings"
import "core:time"
import "ecs"
import "transform"

import gl "vendor:OpenGL"
import SDL "vendor:sdl2"

//game-specific initialization code
initialize :: proc(game: ^Game) {
}

main :: proc() {
    using ecs
    using transform
    for repetition in 0 ..< 5 {
        timer := timer()
        ctx := init_ecs();world := &ctx;defer deinit_ecs(world)
        timer->time("init")
        track_entities_with_components(world, {string, u64, bool})
        timer->time("register systems")
        for i in 0 ..< 100000 {
            e := create_entity(world)
            add_component(world, e, Transform{})
            if i % 2 == 0 {
                add_component(world, e, "hello")
            }
            if i % 3 == 0 {
                add_component(world, e, i)
            }
            if i % 4 == 0 {
                add_component(world, e, false)
            }
            if i % 5 == 0 {
                add_component(world, e, u64(i))
            }
        }
        timer->time("create 100000")
        print("num transforms", world.components[Transform]^.len)
        for i in 0 ..< 25000{
            destroy_entity(world, Entity(i))
        }
        timer->time("destroy 25000")
        print("num transforms", world.components[Transform]^.len)
        for i in 0 ..< 100000 {
            e := create_entity(world)
            add_component(world, e, Transform{})
            if i % 2 == 0 {
                add_component(world, e, "hello")
            }
            if i % 3 == 0 {
                add_component(world, e, i)
            }
            if i % 4 == 0 {
                add_component(world, e, false)
            }
            if i % 5 == 0 {
                add_component(world, e, u64(i))
            }
            destroy_entity(world, Entity(e))
        }
        timer->time("create and destroy 100000 more")
        print("num transforms", world.components[Transform]^.len)
        comps := get_relevant_components(world, {string, u64, bool})
        total_comps := 0
        for e, c in comps {
            total_comps += len(c)
        }
        print(total_comps)
        timer->time("find relevant components (w cache)")
        ents := get_entities_with_components(world, {string, u64, bool})
        print(len(ents))
        timer->time("find entities with components (w cache)")
        ents = get_entities_with_components_prev(world, {string, u64, bool})
        print(len(ents))
        timer->time("find entities with components (no cache)")
        print(check_relevant_tracking_correct(world))
    }
    // WINDOW_WIDTH :: 1200
    // WINDOW_HEIGHT :: 800
    // game := new_game(WINDOW_WIDTH, WINDOW_HEIGHT)
    // defer quit(&game)
    // start(&game)
}

stop :: proc() {
    panic("stopping early")
}
print :: fmt.println
