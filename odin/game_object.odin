package main
import "rigidbody"
import "sprite"
import "transform"

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

GameObject :: struct {
    name:            string,
    component_set:   bit_set[Component],
    // components_disabled:   bit_set[Component],
    parent:          ^GameObject,
    children:        [dynamic]^GameObject,
    using transform: transform.Transform,
    using rigidbody: rigidbody.Rigidbody,
    using sprite:    sprite.Sprite,
    using script:    Script,
}


slice_to_bit_set :: proc(slice: []$T) -> (bitset: bit_set[T]) {
    for thing in slice {
        bitset |= bit_set[T]{thing}
    }
    return
}
add_component :: proc(obj: ^GameObject, components: ..Component) {
    obj.component_set |= slice_to_bit_set(components)
}
remove_component :: proc(obj: ^GameObject, components: ..Component) {
    obj.component_set &~= (slice_to_bit_set(components))
}
has_desired_components :: proc(obj: ^GameObject, desired_components: bit_set[Component]) -> bool {
    return (obj.component_set & desired_components) == desired_components
}
// disable_component :: proc(obj: ^GameObject, components: ..Component) {
//     obj.components_disabled |= slice_to_bit_set(components)
// }
// enable_component :: proc(obj: ^GameObject, components: ..Component) {
//     obj.components_disabled &~= (slice_to_bit_set(components))
// }
