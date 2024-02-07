package main

import "core:fmt"
import glm "core:math/linalg/glsl"
import "core:time"
import "rigidbody"
import "transform"

import gl "vendor:OpenGL"
import SDL "vendor:sdl2"

System :: struct {
    components_needed: [dynamic]typeid,
    start:             proc(system: ^System, game: ^Game),
    update:            proc(system: ^System, game: ^Game),
    needObject:        proc(system: ^System, game: ^Game, obj: ^GameObject) -> bool,
    addObject:         proc(system: ^System, game: ^Game, obj: ^GameObject),
    removeObject:      proc(system: ^System, game: ^Game, obj: ^GameObject),
    name:              string,
}

program: u32
uniforms: gl.Uniforms
indices :: []u16{0, 1, 2, 2, 3, 0}
test_system :: proc() -> ^System {
    return new_clone(System {
        start = proc(system: ^System, game: ^Game) {
            print("start")
        },
        update = proc(system: ^System, game: ^Game) {
            print("update")
        },
        needObject = proc(system: ^System, game: ^Game, obj: ^GameObject) -> bool {
            return true
        },
        addObject = proc(system: ^System, game: ^Game, obj: ^GameObject) {
            print("object added:", obj.name)
        },
        removeObject = proc(system: ^System, game: ^Game, obj: ^GameObject) {
            print("object removed:", obj.name)
        },
        name = "system",
    })
}

physics_system :: proc() -> ^System {
    return new_clone(System {
        components_needed = {transform.Transform, rigidbody.Rigidbody},
        start = proc(system: ^System, game: ^Game) {
        },
        update = proc(system: ^System, game: ^Game) {
        },
        needObject = proc(system: ^System, game: ^Game, obj: ^GameObject) -> bool {
            return true
        },
        addObject = proc(system: ^System, game: ^Game, obj: ^GameObject) {

        },
        removeObject = proc(system: ^System, game: ^Game, obj: ^GameObject) {

        },
        name = "PhysicsSystem",
    })
}
collision_system :: proc() -> ^System {
    return new_clone(System {
        // components_needed = {transform.Transform,collider.Collider},
        start = proc(system: ^System, game: ^Game) {
        },
        update = proc(system: ^System, game: ^Game) {

        },
        needObject = proc(system: ^System, game: ^Game, obj: ^GameObject) -> bool {
            return true
        },
        addObject = proc(system: ^System, game: ^Game, obj: ^GameObject) {

        },
        removeObject = proc(system: ^System, game: ^Game, obj: ^GameObject) {

        },
        name = "CollisionSystem",
    })
}
script_runner :: proc() -> ^System {
    return new_clone(System {
        // components_needed = {Script},
        start = proc(system: ^System, game: ^Game) {
        },
        update = proc(system: ^System, game: ^Game) {
        },
        needObject = proc(system: ^System, game: ^Game, obj: ^GameObject) -> bool {
            return true
        },
        addObject = proc(system: ^System, game: ^Game, obj: ^GameObject) {

        },
        removeObject = proc(system: ^System, game: ^Game, obj: ^GameObject) {

        },
        name = "ScriptRunner",
    })
}
renderer :: proc() -> ^System {
    renderer_update :: proc(system: ^System, using game: ^Game) {
        // timer := timer()
        // duration := time.tick_since(game.start_tick)
        // t := f32(time.duration_seconds(duration))

        // event: SDL.Event
        // for SDL.PollEvent(&event) != false {
        //     #partial switch event.type {
        //     case .KEYDOWN:
        //         #partial switch event.key.keysym.sym {
        //         case .ESCAPE:
        //             return
        //         case:
        //             print(event.key.keysym.sym, "pressed")
        //         }
        //     case .KEYUP:
        //     // print(event.key.keysym.sym,"released")
        //     case .QUIT:
        //       return
        //     case:

        //     }
        // }

        // pos := glm.vec3{glm.cos(t * 2), glm.sin(t * 2), 0}

        // pos *= 0.3

        // model = model * glm.mat4Rotate({0, 0, 1}, t)
        // view := glm.mat4LookAt({0, -1, +1}, {0, 0, 0}, {0, 0, 1})
        // proj := glm.mat4Perspective(45, 1.3, 0.1, 100.0)
        // u_transform := proj * view * model

        // gl.UniformMatrix4fv(uniforms["u_transform"].location, 1, false, &u_transform[0, 0])
        // // gl.Uniform1f(gl.GetUniformLocation(program,"time"),t)
        // // gl.Uniform2f(gl.GetUniformLocation(program,"u_resolution"),window_width,window_height)
        // gl.Viewport(0, 0, window_width, window_height)
        // gl.ClearColor((39.0 / 255), (40.0 / 255), (34.0 / 255), 1.0)
        // gl.Clear(gl.COLOR_BUFFER_BIT)

        // gl.DrawElements(gl.TRIANGLES, i32(len(indices)), gl.UNSIGNED_SHORT, nil)

        // SDL.GL_SwapWindow(window)
        // timer->time("sdl clear")
        // timer->time("sort objects by z")
        // timer->time("draw objects")
        // timer->time("sdl render")
    }
    return new_clone(
        System {
            // components_needed = {transform.Transform,Sprite},
            start = proc(system: ^System, using game: ^Game) {
                gl_context := SDL.GL_CreateContext(window)
                SDL.GL_MakeCurrent(window, gl_context)
                // load the OpenGL procedures once an OpenGL context has been established
                gl.load_up_to(3, 3, SDL.gl_set_proc_address)

                // useful utility procedures that are part of vendor:OpenGl
                program, program_ok := gl.load_shaders_source(vertex_source, fragment_source)
                if !program_ok {
                    fmt.eprintln("Failed to create GLSL program")
                    return
                }
                defer gl.DeleteProgram(program)

                gl.UseProgram(program)

                uniforms := gl.get_uniforms_from_program(program)
                defer delete(uniforms)

                vao: u32
                gl.GenVertexArrays(1, &vao);defer gl.DeleteVertexArrays(1, &vao)

                // initialization of OpenGL buffers
                vbo, ebo: u32
                gl.GenBuffers(1, &vbo);defer gl.DeleteBuffers(1, &vbo)
                gl.GenBuffers(1, &ebo);defer gl.DeleteBuffers(1, &ebo)

                Vertex :: struct {
                    position: glm.vec3,
                    color:    glm.vec4,
                }

                vertices := []Vertex {
                    {{-0.5, +0.5, 0}, {1.0, 0.0, 0.0, 0.75}},
                    {{-0.5, -0.5, 0}, {1.0, 1.0, 0.0, 0.75}},
                    {{+0.5, -0.5, 0}, {0.0, 1.0, 0.0, 0.75}},
                    {{+0.5, +0.5, 0}, {0.0, 0.0, 1.0, 0.75}},
                }

                indices := []u16{0, 1, 2, 2, 3, 0}

                gl.BindBuffer(gl.ARRAY_BUFFER, vbo)
                gl.BufferData(
                    gl.ARRAY_BUFFER,
                    len(vertices) * size_of(vertices[0]),
                    raw_data(vertices),
                    gl.STATIC_DRAW,
                )
                gl.EnableVertexAttribArray(0)
                gl.EnableVertexAttribArray(1)
                gl.VertexAttribPointer(
                    0,
                    3,
                    gl.FLOAT,
                    false,
                    size_of(Vertex),
                    offset_of(Vertex, position),
                )
                gl.VertexAttribPointer(
                    1,
                    4,
                    gl.FLOAT,
                    false,
                    size_of(Vertex),
                    offset_of(Vertex, color),
                )

                gl.BindBuffer(gl.ELEMENT_ARRAY_BUFFER, ebo)
                gl.BufferData(
                    gl.ELEMENT_ARRAY_BUFFER,
                    len(indices) * size_of(indices[0]),
                    raw_data(indices),
                    gl.STATIC_DRAW,
                )
            },
            update = renderer_update,
            needObject = proc(system: ^System, game: ^Game, obj: ^GameObject) -> bool {
                return true
            },
            addObject = proc(system: ^System, game: ^Game, obj: ^GameObject) {

            },
            removeObject = proc(system: ^System, game: ^Game, obj: ^GameObject) {

            },
            name = "Renderer",
        },
    )
}

vertex_source :: `
#version 120
attribute vec3 a_position;
attribute vec4 a_color;
varying vec4 v_color;
uniform mat4 u_transform;
void main() {	
	gl_Position = u_transform * vec4(a_position, 1.0);
	v_color = a_color;
}
`

fragment_source :: `
#version 120
varying vec4 v_color;

void main() {
    gl_FragColor = v_color;
}
`
