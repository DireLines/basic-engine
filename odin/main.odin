package main

import "core:fmt"
import "core:math"
import glm "core:math/linalg/glsl"
import "core:strings"
import "core:time"
import "rigidbody"
import "sprite"
import "transform"

vec2 :: [2]f32
mat3 :: matrix[3, 3]f32

mat3_mult :: proc(a, b: mat3) -> (result: mat3) {
    for i in 0 ..< 3 {
        for j in 0 ..< 3 {
            for k in 0 ..< 3 {
                result[i, j] += a[i, k] * b[k, j]
            }
        }
    }
    return
}
print_matrix :: proc(m: mat3) {
    using fmt
    println("[")
    for i in 0 ..< 3 {
        print("\t")
        for j in 0 ..< 3 {
            printf("%4.2f ", m[i, j])
        }
        print("\n")
    }
    println("]")
}

//game-specific initialization code
initialize :: proc(game: ^Game) {
    using math, transform
    for x in 0 ..< 2 {
        for y in 0 ..< 2 {
            a := GameObject {
                component_set = {.Transform, .Sprite, .Script},
                transform = transform.default_transform(),
                sprite = sprite.default_sprite(),
                rigidbody = rigidbody.default_rigidbody(),
                script = test_script(),
            }
            a.position = {-150 + f32(x * 2), -150 + f32(y * 2)}
            a.velocity = {500 * sin(f32(x + y)), 500 * sin(f32(x * y))}
            a.rotation = to_radians_f32(f32(y))
            a.scale = {f32(x) * 0.01, f32(x) * 0.01}
            instantiate(game, a)
        }
    }
}
main :: proc() {
    base_width :: 2400
    game := new_game(window_width = base_width, window_height = (9.0 / 16.0) * base_width)
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
