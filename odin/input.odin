package main

InputState :: struct {
    buttons:           map[u8]struct {},
    prev_buttons:      map[u8]struct {},
    clicks:            map[u8]struct {},
    prev_clicks:       map[u8]struct {},
    left_button_held:  bool,
    right_button_held: bool,
    left_button_down:  bool,
    right_button_down: bool,
}
Input :: struct {
    using input_state: InputState,
    update:            proc(input: ^Input),
    postupdate:        proc(input: ^Input),
    button_pressed:    proc(input: ^Input, button: u8) -> bool,
    button_held:       proc(input: ^Input, button: u8) -> bool,
    button_released:   proc(input: ^Input, button: u8) -> bool,
}

input_system :: proc() -> ^Input {

    button_held_last_frame :: proc(input: ^Input, button: u8) -> bool {
        return button in input.prev_buttons
    }
    return new_clone(Input {
        input_state = InputState {
            left_button_held = false,
            right_button_held = false,
            left_button_down = false,
            right_button_down = false,
        },
        update = proc(input: ^Input) {
        },
        postupdate = proc(input: ^Input) {
            using input
            copy_map :: proc(from, to: ^map[$K]$V) {
                clear(to)
                for k, v in from {
                    to[k] = v
                }
            }
            copy_map(&buttons, &prev_buttons)
            copy_map(&clicks, &prev_clicks)
        },
        button_pressed = proc(input: ^Input, button: u8) -> bool {
            return input->button_held(button) && !button_held_last_frame(input, button)
        },
        button_held = proc(input: ^Input, button: u8) -> bool {
            return button in input.buttons
        },
        button_released = proc(input: ^Input, button: u8) -> bool {
            return !input->button_held(button) && button_held_last_frame(input, button)
        },
    })
}
