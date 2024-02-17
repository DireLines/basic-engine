package main

import "core:time"

main :: proc() {
    game := Game{}
}

Input :: struct {}

Game :: struct {
    input_system: ^Input,
    objects:      #soa[dynamic]GameObject,
    start_tick:   time.Tick,
}

GameObject :: struct {
    awake: proc(self_index: int, game: ^Game),
}
