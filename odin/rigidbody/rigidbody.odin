package rigidbody

vec2 :: [2]f32
mat3 :: matrix[3, 3]f32
Rigidbody :: struct {
    mass:              f32,
    moment_of_inertia: f32,
    velocity:          vec2,
    angular_velocity:  f32,
    force:             vec2,
    torque:            f32,
}

default_rigidbody :: proc() -> Rigidbody {
    return Rigidbody{mass = 1.0, moment_of_inertia = 1.0}
}

add_force_vec2 :: proc(rb: ^Rigidbody, f: vec2) {
    rb.force += f
}
add_force_xy :: proc(rb: ^Rigidbody, x, y: f32) {
    rb.force += vec2{x, y}
}
add_force :: proc {
    add_force_vec2,
    add_force_xy,
}

add_torque :: proc(rb: ^Rigidbody, t: f32) {
    rb.torque += t
}

add_force_at_point :: proc(rb: ^Rigidbody, f: vec2, p: vec2) {
    //TODO
}
