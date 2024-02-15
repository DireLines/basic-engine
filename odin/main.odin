package main

import "core:fmt"
import "core:time"

//it turns out the frequency with which the race condition is hit
//depends highly on where the definition of GameObject is
//above initialize: always broken
//below initialize: always fixed
//I have gotten half and half but lost what state the code was in when that happened
GameObject :: struct {
    name:          string,
    component_set: bit_set[Component],
    parent:        ^GameObject,
    children:      [dynamic]^GameObject,
    script:        Script,
}

//game-specific initialization code
initialize :: proc(game: ^Game) {
}

main :: proc() {
    base_width :: 2400
    game := new_game()
}

print :: fmt.println
vec2 :: [2]f32
mat3 :: matrix[3, 3]f32
frames_per_sec :: 120
ms_per_frame :: (1.0 / frames_per_sec) * 1000
frame_counter: u64
average_frame_length: f64

Game :: struct {
    systems:    [dynamic]System,
    objects:    #soa[dynamic]GameObject,
    start_tick: time.Tick,
}

new_game :: proc() -> Game {
    game := Game{}
    init(&game)
    return game
}

init :: proc(game: ^Game) {
    script_runner := script_runner()
    add_systems(game, script_runner)
}

add_system :: proc(game: ^Game, system: System) {
    append(&game.systems, system)
}

add_systems :: proc(game: ^Game, systems: ..System) {
    for system in systems {
        add_system(game, system)
    }
}

instantiate :: proc(game: ^Game, obj: GameObject) -> int {
    append_soa(&game.objects, obj)
    return 0
}

System :: struct {
    needObject: proc(system: ^System, game: ^Game, obj_index: int) -> bool,
}
script_runner :: proc() -> System {
    return System{needObject = proc(system: ^System, game: ^Game, obj_index: int) -> bool {
                obj := game.objects[obj_index] //bug lurks somewhere here
                // removing this line fixes one of the errors:
                // src/array.cpp(61): Assertion Failure: `cast(usize)index < cast(usize)count` Index 5 is out of bounds ranges 0..<3\\n
                return true
            }}
}

Script :: struct {
    awake: proc(self_index: int, game: ^Game),
}

Component :: enum {
    Transform,
    Rigidbody,
    Collider,
    Sprite,
    Animation,
    Script,
    AdditionalScripts,
    Children,
}
