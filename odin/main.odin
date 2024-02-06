package main

import "core:fmt"
import glm "core:math/linalg/glsl"
import "core:time"

import gl "vendor:OpenGL"
import SDL "vendor:sdl2"

//game-specific initialization code
initialize :: proc(game: ^Game) {
}

main :: proc() {
    timer := timer()
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
