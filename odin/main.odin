package main

import "core:fmt"
import "core:math"
import glm "core:math/linalg/glsl"
import "core:strings"
import "core:time"

vec2 :: [2]f32
mat3 :: matrix[3, 3]f32

//game-specific initialization code
initialize :: proc(game: ^Game) {
}
main :: proc() {
    base_width :: 2400
    game := new_game(window_width = base_width, window_height = (9.0 / 16.0) * base_width)
}

stop :: proc() {
    panic("stopping early")
}
print :: fmt.println
printf :: fmt.printf
