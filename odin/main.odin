package main

// src/array.cpp(61): Assertion Failure: `cast(usize)index < cast(usize)count` Index 3 is out of bounds ranges 0..<3\\n
//                                                                                   ^ matches number of fields in GameObject struct
main :: proc() {
    game := new_game()
}

//it turns out the frequency with which the race condition is hit
//depends highly on where the definition of GameObject is
// above first mention of the type Game: fails
// below first mention of the type Game: works
//I have gotten half and half but lost what state the code was in when that happened
GameObject :: struct {
    name:          string, //commenting out either of these produces crazy error src/llvm_backend_expr.cpp(2254): Panic: Invalid type conversion: 'Allocator' to 'int' for procedure '_proclit$anon-1'
    component_set: bit_set[Component], //commenting out either of these produces crazy error src/llvm_backend_expr.cpp(2254): Panic: Invalid type conversion: 'Allocator' to 'int' for procedure '_proclit$anon-1'
    script:        Script,
}

Game :: struct {
    systems: [dynamic]System,
    objects: #soa[dynamic]GameObject,
}

new_game :: proc() -> Game {
    game := Game{}
    append(&game.systems, script_runner())
    return game
}

System :: struct {
    needObject: proc(system: ^System, game: ^Game, obj_index: int) -> bool,
}
script_runner :: proc() -> System {
    return System{needObject = proc(system: ^System, game: ^Game, obj_index: int) -> bool {
                obj := game.objects[obj_index] //bug lurks somewhere here
                // removing this line fixes one of the errors:
                // src/array.cpp(61): Assertion Failure: `cast(usize)index < cast(usize)count` Index 3 is out of bounds ranges 0..<3\\n
                return true
            }}
}

Script :: struct {
    awake: proc(self_index: int, game: ^Game),
}

Component :: enum {
    Script,
}
