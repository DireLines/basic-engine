package main
import glm "core:math/linalg/glsl"

GameObject :: struct {
	name:      string,
	transform: glm.mat3x3,
}
