package transform

import "core:fmt"
import glm "core:math/linalg/glsl"

Transform :: struct {
    position: glm.vec2,
    scale:    glm.vec2,
    pivot:    glm.vec2,
    rotation: f32,
}

default_transform :: proc() -> Transform {
    return Transform{scale = {1, 1}}
}

unpivot :: proc(using t: ^Transform) -> (result: glm.mat3) {
    return translate_vec2(-pivot)
}
identity :: proc() -> glm.mat3 {
    return glm.identity(glm.mat3)
}
translate_vec2 :: proc(v: glm.vec2) -> (result: glm.mat3) {
    return translate_xy(v.x, v.y)
}
translate_xy :: proc(x, y: f32) -> (result: glm.mat3) {
    result = identity()
    result[0][2] = x
    result[1][2] = y
    return
}
translate :: proc {
    translate_xy,
    translate_vec2,
}

rotate :: proc(r: f32) -> (result: glm.mat3) {
    result = identity()
    result[0][0] = glm.cos(r)
    result[0][1] = -glm.sin(r)
    result[1][0] = glm.sin(r)
    result[1][1] = glm.cos(r)
    return
}

scale_vec2 :: proc(v: glm.vec2) -> (result: glm.mat3) {
    return scale_xy(v.x, v.y)
}
scale_xy :: proc(x, y: f32) -> (result: glm.mat3) {
    result = identity()
    result[0][0] = x
    result[1][1] = y
    return
}
scale :: proc {
    scale_xy,
    scale_vec2,
}

apply :: proc(using t: ^Transform) -> glm.mat3 {
    trans := translate(position)
    rot := rotate(glm.radians_f32(rotation))
    scl := scale_vec2(scale)
    fmt.println("translate", trans, "rotate", rot, "scale", scl)
    return translate(position) * rotate(glm.radians_f32(rotation)) * scale_vec2(scale)

}
reverse :: proc(using t: ^Transform) -> (result: glm.mat3) {
    return scale_vec2(1 / scale) * rotate(glm.radians_f32(-rotation)) * translate(-position)
}
