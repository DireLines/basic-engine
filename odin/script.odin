package main

Script :: struct {
    awake:              proc(self_index: int, game: ^Game),
    start:              proc(self_index: int, game: ^Game),
    update:             proc(self_index: int, game: ^Game),
    on_collision_enter: proc(self_index: int, other_index: int, game: ^Game),
    on_collision_stay:  proc(self_index: int, other_index: int, game: ^Game),
    on_collision_exit:  proc(self_index: int, other_index: int, game: ^Game),
}
default_script :: proc() -> Script {
    return(
        Script {
            awake = proc(self_index: int, game: ^Game) {},
            start = proc(self_index: int, game: ^Game) {},
            update = proc(self_index: int, game: ^Game) {},
            on_collision_enter = proc(self_index: int, other_index: int, game: ^Game) {},
            on_collision_stay = proc(self_index: int, other_index: int, game: ^Game) {},
            on_collision_exit = proc(self_index: int, other_index: int, game: ^Game) {},
        } \
    )
}
test_script :: proc() -> Script {
    return Script{awake = proc(self_index: int, game: ^Game) {
                print("awake", self_index)
            }, start = proc(self_index: int, game: ^Game) {
                print("start", self_index)
            }, update = proc(self_index: int, game: ^Game) {
                print("update", self_index)
            }, on_collision_enter = proc(self_index: int, other_index: int, game: ^Game) {
                print("on collision enter", self_index, other_index)
            }, on_collision_stay = proc(self_index: int, other_index: int, game: ^Game) {
                print("on collision stay", self_index, other_index)
            }, on_collision_exit = proc(self_index: int, other_index: int, game: ^Game) {
                print("on collision_exit", self_index, other_index)
            }}
}
