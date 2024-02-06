package main

import "core:c"
import SDL "vendor:sdl2"

InputState :: struct {
    keys:                    map[SDL.Scancode]struct {},
    prev_keys:               map[SDL.Scancode]struct {},
    buttons:                 map[u8]struct {},
    prev_buttons:            map[u8]struct {},
    clicks:                  map[u8]struct {},
    prev_clicks:             map[u8]struct {},
    cursor_pos:              SDL.Point,
    last_clicked_cursor_pos: SDL.Point,
    left_button_held:        bool,
    right_button_held:       bool,
    left_button_down:        bool,
    right_button_down:       bool,
}
Input :: struct {
    using input_state:         InputState,
    poll:                      proc(input: ^Input, event: SDL.Event),
    update:                    proc(input: ^Input, game: ^Game),
    postupdate:                proc(input: ^Input, game: ^Game),
    button_pressed:            proc(input: ^Input, button: u8) -> bool,
    button_held:               proc(input: ^Input, button: u8) -> bool,
    button_released:           proc(input: ^Input, button: u8) -> bool,
    key_pressed:               proc(input: ^Input, key: SDL.Scancode) -> bool,
    key_held:                  proc(input: ^Input, key: SDL.Scancode) -> bool,
    key_released:              proc(input: ^Input, key: SDL.Scancode) -> bool,
    selection_corners:         proc(input: ^Input) -> (SDL.Point, SDL.Point),
    get_cursor_position:       proc(input: ^Input) -> SDL.Point,
    get_last_clicked_position: proc(input: ^Input) -> SDL.Point,
}

input_system :: proc() -> ^Input {
    key_held_last_frame :: proc(input: ^Input, key: SDL.Scancode) -> bool {
        return key in input.prev_keys
    }

    button_held_last_frame :: proc(input: ^Input, button: u8) -> bool {
        return button in input.prev_buttons
    }
    return new_clone(
        Input {
            input_state = InputState {
                cursor_pos = SDL.Point{},
                last_clicked_cursor_pos = SDL.Point{},
                left_button_held = false,
                right_button_held = false,
                left_button_down = false,
                right_button_down = false,
            },
            poll = proc(input: ^Input, event: SDL.Event) {
                using input
                #partial switch event.type {
                case .KEYDOWN:
                    keys[event.key.keysym.scancode] = {}
                case .KEYUP:
                    delete_key(&keys, event.key.keysym.scancode)
                case .MOUSEBUTTONDOWN:
                    clicks[event.button.button] = {}
                case .MOUSEBUTTONUP:
                    delete_key(&clicks, event.button.button)
                case .JOYBUTTONDOWN:
                    buttons[event.jbutton.button] = {}
                case .JOYBUTTONUP:
                    delete_key(&buttons, event.jbutton.button)
                }
            },
            update = proc(input: ^Input, game: ^Game) {
                using input
                get_cursor_pos :: proc() -> SDL.Point {
                    x, y: c.int
                    SDL.GetMouseState(&x, &y)
                    return SDL.Point{x, y}
                }
                cursor_pos = get_cursor_pos()
                left_button_this_frame := false
                left_button_down = false
                right_button_this_frame := false
                right_button_down = false
                for click in clicks {
                    switch click {
                    case SDL.BUTTON_LEFT:
                        left_button_this_frame = true
                        if !left_button_held {
                            left_button_held = true
                            left_button_down = true
                            last_clicked_cursor_pos = cursor_pos
                        }
                    case SDL.BUTTON_RIGHT:
                        right_button_this_frame = true
                        if !right_button_held {
                            right_button_held = true
                            right_button_down = true
                        }
                    }
                }
                if !left_button_this_frame {
                    left_button_held = false
                }
                if !right_button_this_frame {
                    right_button_held = false
                }
            },
            postupdate = proc(input: ^Input, game: ^Game) {
                using input
                copy_map :: proc(from, to: ^map[$K]$V) {
                    clear(to)
                    for k, v in from {
                        to[k] = v
                    }
                }
                copy_map(&keys, &prev_keys)
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
            key_pressed = proc(input: ^Input, key: SDL.Scancode) -> bool {
                return input->key_held(key) && !key_held_last_frame(input, key)
            },
            key_held = proc(input: ^Input, key: SDL.Scancode) -> bool {
                return key in input.keys
            },
            key_released = proc(input: ^Input, key: SDL.Scancode) -> bool {
                return !input->key_held(key) && key_held_last_frame(input, key)
            },
            selection_corners = proc(input: ^Input) -> (SDL.Point, SDL.Point) {
                using input
                // TODO: figure out top left / bottom right points, always return those
                return last_clicked_cursor_pos, cursor_pos
            },
            get_cursor_position = proc(input: ^Input) -> SDL.Point {
                return input.cursor_pos
            },
            get_last_clicked_position = proc(input: ^Input) -> SDL.Point {
                return input.last_clicked_cursor_pos
            },
        },
    )
}
