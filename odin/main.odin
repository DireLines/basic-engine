package main

import "core:time"

main :: proc() {
    game := Game{}
}

Input :: struct {}

Game :: struct {
    input_system: ^Input, //removing this or reordering below objects fixes the bug 
    objects:      #soa[dynamic]GameObject,
    start_tick:   time.Tick, //removing this fixes the bug 
}

GameObject :: struct {
    awake: proc(game: ^Game), //making it so that no args are of type ^Game fixes the bug
}
