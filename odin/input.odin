package main

Input :: struct {
    buttons:         map[u8]struct {},
    update:          proc(input: ^Input),
    postupdate:      proc(input: ^Input),
    button_pressed:  proc(input: ^Input, button: u8) -> bool,
    button_held:     proc(input: ^Input, button: u8) -> bool,
    button_released: proc(input: ^Input, button: u8) -> bool,
}
