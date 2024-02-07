package main

import "core:fmt"
import glm "core:math/linalg/glsl"
import "core:strings"
import "core:time"
import "ecs"
import "rigidbody"
import "transform"

import gl "vendor:OpenGL"
import SDL "vendor:sdl2"

//game-specific initialization code
initialize :: proc(game: ^Game) {
    using ecs, transform, rigidbody
    timer := timer()
    world := &game.objects
    timer->time("init")
    track_entities_with_components(world, {Transform, Rigidbody})
    timer->time("register systems")
    for i in 0 ..< 15000 {
        e := create_entity(world)
        add_component(world, e, default_transform())
        add_component(world, e, default_rigidbody())
    }
    timer->time("create 15000 entities")
}

assert_all :: proc(args: ..bool) {
    for arg in args {
        assert(arg)
    }
}
check_both_indices_correct :: proc(ctx: ^ecs.Context) -> (correct: bool) {
    using ecs
    correct = true
    for entity, &m in ctx.entity_indices {
        for comp, i in m {
            true_index := ctx.component_indices[comp][entity]
            if true_index != i {
                print(#procedure, "mismatch for", entity, "and", comp, ":", i, "!=", true_index)
                correct = false
            }
        }
    }
    return correct
}
main :: proc() {
    WINDOW_WIDTH :: 1200
    WINDOW_HEIGHT :: 800
    game := new_game(WINDOW_WIDTH, WINDOW_HEIGHT)
    defer quit(&game)
    start(&game)
}

stop :: proc() {
    panic("stopping early")
}
print :: fmt.println
