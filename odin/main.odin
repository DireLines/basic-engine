package main

import "core:fmt"
import glm "core:math/linalg/glsl"
import "core:strings"
import "core:time"
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
    center_marker.color = {0,0,255,255}
    a := GameObject {
        component_set = {.Transform, .Sprite},
        transform = transform.default_transform(),
        sprite = sprite.default_sprite(),
    }
    a.position = {400, 100}
    // a.rotation = -1
    // a.scale = {2,2}
    instantiate(game, a)
    instantiate(game, center_marker)
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
