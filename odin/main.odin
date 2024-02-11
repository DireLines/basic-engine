package main

import "core:fmt"
import glm "core:math/linalg/glsl"
import "core:strings"
import "core:math"
import "core:time"
import "rigidbody"
import "sprite"
import "transform"

//game-specific initialization code
initialize :: proc(game: ^Game) {
    timer := timer()
    center_marker := GameObject {
        component_set = {.Transform, .Sprite},
        transform = transform.default_transform(),
        sprite = sprite.default_sprite(),
    }
    center_marker.color = {0, 0, 255, 255}
    instantiate(game, center_marker)
    for x in 0 ..< 200 {
        for y in 0 ..< 100 {
            a := GameObject {
                component_set = {.Transform, .Sprite, .Rigidbody},
                transform = transform.default_transform(),
                sprite = sprite.default_sprite(),
                rigidbody = rigidbody.default_rigidbody(),
            }
            a.position = {-150 + f32(x * 2), -150 + f32(y * 2)}
            a.velocity = {f32(x)*0.4,f32(y)*0.4}
            a.rotation = math.to_radians_f32(90)
            // a.scale = {f32(x)*0.1,f32(x)*0.1}
            instantiate(game, a)
        }
    }
}
main :: proc() {
    game := new_game(window_width = 1200, window_height = 800)
    defer quit(&game)
    start(&game)
}

stop :: proc() {
    panic("stopping early")
}
print :: fmt.println
printf :: fmt.printf
assert_all :: proc(args: ..bool) {
    for arg in args {
        assert(arg)
    }
}
