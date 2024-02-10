package sprite

import "vendor:raylib"

Sprite :: struct {
    file:  string,
    z:     f32, // rendering order
    color: raylib.Color,
    image: ^raylib.Texture2D,
}

default_sprite :: proc() -> Sprite {
    return Sprite{file = "default.png", color = raylib.WHITE}
}
