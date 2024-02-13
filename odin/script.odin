package main

Script :: struct {
    awake: proc(self_index: int, game: ^Game),
}
test_script :: proc() -> Script {
    test_awake :: proc(self_index: int, game: ^Game) {
        print("awake", self_index)
    }
    return Script{awake = test_awake}
}
