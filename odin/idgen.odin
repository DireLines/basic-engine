package main

IDGenerator :: struct {
    id:   int,
    next: proc(_: ^IDGenerator) -> int,
}
id_generator :: proc() -> IDGenerator {
    return IDGenerator{id = 0, next = proc(gen: ^IDGenerator) -> int {
                gen.id += 1
                return gen.id
            }}
}
