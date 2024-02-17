package transform

import "core:fmt"
import "core:math"
import glm "core:math/linalg/glsl"
vec2 :: [2]f32
mat3 :: matrix[3, 3]f32
Transform :: struct {
    position: vec2,
    scale:    vec2,
    pivot:    vec2,
    rotation: f32,
}

default_transform :: proc() -> Transform {
    return Transform{scale = {1, 1}}
}

unpivot :: proc(using t: ^Transform) -> (result: mat3) {
    return translate_vec2(-pivot)
}
identity :: proc() -> (result: mat3) {
    result[0, 0] = 1
    result[1, 1] = 1
    result[2, 2] = 1
    return
}
translate_vec2 :: proc(v: vec2) -> (result: mat3) {
    return translate_xy(v.x, v.y)
}
translate_xy :: proc(x, y: f32) -> (result: mat3) {
    result = identity()
    result[0, 2] = x
    result[1, 2] = y
    return
}
translate :: proc {
    translate_xy,
    translate_vec2,
}

rotate :: proc(r: f32) -> (result: mat3) {
    using math
    result = identity()
    result[0, 0] = cos(r)
    result[0, 1] = -sin(r)
    result[1, 0] = sin(r)
    result[1, 1] = cos(r)
    return
}

scale_vec2 :: proc(v: vec2) -> (result: mat3) {
    return scale_xy(v.x, v.y)
}
scale_xy :: proc(x, y: f32) -> (result: mat3) {
    result = identity()
    result[0, 0] = x
    result[1, 1] = y
    return
}
scale :: proc {
    scale_xy,
    scale_vec2,
}

apply :: proc(using t: ^Transform) -> mat3 {
    using glm
    return translate(position) * rotate(radians_f32(rotation)) * scale_vec2(scale)

}
reverse :: proc(using t: ^Transform) -> (result: mat3) {
    using glm
    return scale_vec2(1 / scale) * rotate(radians_f32(-rotation)) * translate(-position)
}
