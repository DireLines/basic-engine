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
    check_relevant_tracking_correct :: proc(ctx: ^ecs.Context) -> (correct: bool) {
        correct = true
        for k, &v in ctx.relevant_entities {
            predicted_comps := get_relevant_components(ctx, v.components[:])
            for entity, comps in predicted_comps {
                for comp, i in comps {
                    true_index := ctx.entity_indices[entity][comp]
                    if true_index != i {
                        print("mismatch for", entity, "and", comp, ":", i, "!=", true_index)
                        correct = false
                    }
                }
            }
        }
        return correct
    }
    for repetition in 0 ..< 5 {
        timer := timer()
        ctx := init_ecs();world := &ctx;defer deinit_ecs(world)
        timer->time("init")
        track_entities_with_components(world, {string, u64, bool})
        timer->time("register systems")
        for i in 0 ..< 10 {
            e := create_entity(world)
            add_component(world, e, Transform{})
            add_component(world, e, "hello")
            add_component(world, e, u64(i))
            add_component(world, e, false)
        }
        destroy_entity(world, Entity(1))
        e := create_entity(world)
        add_component(world, e, Transform{})
        add_component(world, e, "hello")
        add_component(world, e, false)
        add_component(world, e, u64(e))
        destroy_entity(world, Entity(e))
        add_component(world, e, Transform{})
        add_component(world, e, "hello")
        add_component(world, e, false)
        add_component(world, e, u64(e))
        destroy_entity(world, Entity(0))
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
