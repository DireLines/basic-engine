package main

import "core:fmt"
import glm "core:math/linalg/glsl"
import "core:time"
import "ecs"
import "transform"

import gl "vendor:OpenGL"
import SDL "vendor:sdl2"

//game-specific initialization code
initialize :: proc(game: ^Game) {
}

main :: proc() {
    print("start")
    using ecs
    using transform
    timer := timer()
    ctx := init_ecs();world := &ctx;defer deinit_ecs(world)
    for i in 0 ..< 10000 {
        e := create_entity(world)
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
    timer->time("create 10000")
    for i in 0 ..< 2500 {
        destroy_entity(world, Entity(i))
    }
    timer->time("destroy 2500")
    for i in 0 ..< 10000 {
        e := create_entity(world)
        if i % 2 == 0 {
            add_component(world, e, u64(i))
            add_component(world, e, "hello")
        } else {
            add_component(world, e, "hello")
        }
    }
    timer->time("create 10000 more")
    ents := get_entities_with_components(world, {string,u64,bool})
    print(len(ents))
    timer->time("find entities")
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
