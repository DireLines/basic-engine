package rigidbody

import glm "core:math/linalg/glsl"

Rigidbody :: struct {
    mass:              f32,
    moment_of_inertia: f32,
    velocity:          glm.vec2,
    angular_velocity:  f32,
    force:             glm.vec2,
    torque:            f32,
}

default_rigidbody :: proc() -> Rigidbody {
    return Rigidbody{mass = 1.0, moment_of_inertia = 1.0}
}

add_force_vec2 :: proc(rb: ^Rigidbody, f: glm.vec2) {
    rb.force += f
}
add_force_xy :: proc(rb: ^Rigidbody, x, y: f32) {
    rb.force += glm.vec2{x, y}
}
add_force :: proc {
    add_force_vec2,
    add_force_xy,
}

add_torque :: proc(rb: ^Rigidbody, t: f32) {
    rb.torque += t
}

add_force_at_point :: proc(rb: ^Rigidbody, f: glm.vec2, p: glm.vec2) {
    //TODO
}
