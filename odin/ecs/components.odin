package ecs

//overloads for doing stuff to components (plural)

/* 
NOTE:
I know, code duplication...

There is no way to take in an array of types, and it will not be added to the language. 
The only reasonable solution is this...
*/
add_components_2 :: proc(ctx: ^Context, entity: Entity, a: $A, b: $B) -> (^A, ^B, [2]ECS_Error) {
    _a, err1 := add_component(ctx, entity, a)
    _b, err2 := add_component(ctx, entity, b)
    return _a, _b, {}
}

add_components_3 :: proc(
    ctx: ^Context,
    entity: Entity,
    a: $A,
    b: $B,
    c: $C,
) -> (
    ^A,
    ^B,
    ^C,
    [3]ECS_Error,
) {
    _a, err1 := add_component(ctx, entity, a)
    _b, err2 := add_component(ctx, entity, b)
    _c, err3 := add_component(ctx, entity, c)
    return _a, _b, _c, {}
}

add_components_4 :: proc(
    ctx: ^Context,
    entity: Entity,
    a: $A,
    b: $B,
    c: $C,
    d: $D,
) -> (
    ^A,
    ^B,
    ^C,
    ^D,
    [4]ECS_Error,
) {
    _a, err1 := add_component(ctx, entity, a)
    _b, err2 := add_component(ctx, entity, b)
    _c, err3 := add_component(ctx, entity, c)
    _d, err4 := add_component(ctx, entity, d)
    return _a, _b, _c, _d, {}
}

add_components_5 :: proc(
    ctx: ^Context,
    entity: Entity,
    a: $A,
    b: $B,
    c: $C,
    d: $D,
    e: $E,
) -> (
    ^A,
    ^B,
    ^C,
    ^D,
    ^E,
    [5]ECS_Error,
) {
    _a, err1 := add_component(ctx, entity, a)
    _b, err2 := add_component(ctx, entity, b)
    _c, err3 := add_component(ctx, entity, c)
    _d, err4 := add_component(ctx, entity, d)
    _e, err5 := add_component(ctx, entity, e)
    return _a, _b, _c, _d, _e, {}
}

add_components_6 :: proc(
    ctx: ^Context,
    entity: Entity,
    a: $A,
    b: $B,
    c: $C,
    d: $D,
    e: $E,
    f: $F,
) -> (
    ^A,
    ^B,
    ^C,
    ^D,
    ^E,
    ^F,
    [6]ECS_Error,
) {
    _a, err1 := add_component(ctx, entity, a)
    _b, err2 := add_component(ctx, entity, b)
    _c, err3 := add_component(ctx, entity, c)
    _d, err4 := add_component(ctx, entity, d)
    _e, err5 := add_component(ctx, entity, e)
    _f, err6 := add_component(ctx, entity, f)
    return _a, _b, _c, _d, _e, _f, {}
}

add_components_7 :: proc(
    ctx: ^Context,
    entity: Entity,
    a: $A,
    b: $B,
    c: $C,
    d: $D,
    e: $E,
    f: $F,
    g: $G,
) -> (
    ^A,
    ^B,
    ^C,
    ^D,
    ^E,
    ^F,
    ^G,
    [7]ECS_Error,
) {
    _a, err1 := add_component(ctx, entity, a)
    _b, err2 := add_component(ctx, entity, b)
    _c, err3 := add_component(ctx, entity, c)
    _d, err4 := add_component(ctx, entity, d)
    _e, err5 := add_component(ctx, entity, e)
    _f, err6 := add_component(ctx, entity, f)
    _g, err7 := add_component(ctx, entity, g)
    return _a, _b, _c, _d, _e, _f, _g, {}
}

add_components_8 :: proc(
    ctx: ^Context,
    entity: Entity,
    a: $A,
    b: $B,
    c: $C,
    d: $D,
    e: $E,
    f: $F,
    g: $G,
    h: $H,
) -> (
    ^A,
    ^B,
    ^C,
    ^D,
    ^E,
    ^F,
    ^G,
    ^H,
    [8]ECS_Error,
) {
    _a, err1 := add_component(ctx, entity, a)
    _b, err2 := add_component(ctx, entity, b)
    _c, err3 := add_component(ctx, entity, c)
    _d, err4 := add_component(ctx, entity, d)
    _e, err5 := add_component(ctx, entity, e)
    _f, err6 := add_component(ctx, entity, f)
    _g, err7 := add_component(ctx, entity, g)
    _h, err8 := add_component(ctx, entity, h)
    return _a, _b, _c, _d, _e, _f, _g, _h, {}
}

add_components :: proc {
    add_component,
    add_components_2,
    add_components_3,
    add_components_4,
    add_components_5,
    add_components_6,
    add_components_7,
    add_components_8,
}


get_components_2 :: proc(ctx: ^Context, entity: Entity, $A, $B: typeid) -> (^A, ^B, [2]ECS_Error) {
    a, err1 := get_component(ctx, entity, A)
    b, err2 := get_component(ctx, entity, B)
    return a, b, {}
}

get_components_3 :: proc(
    ctx: ^Context,
    entity: Entity,
    $A, $B, $C: typeid,
) -> (
    ^A,
    ^B,
    ^C,
    [3]ECS_Error,
) {
    a, err1 := get_component(ctx, entity, A)
    b, err2 := get_component(ctx, entity, B)
    c, err3 := get_component(ctx, entity, C)
    return a, b, c, {}
}

get_components_4 :: proc(
    ctx: ^Context,
    entity: Entity,
    $A, $B, $C, $D: typeid,
) -> (
    ^A,
    ^B,
    ^C,
    ^D,
    [4]ECS_Error,
) {
    a, err1 := get_component(ctx, entity, A)
    b, err2 := get_component(ctx, entity, B)
    c, err3 := get_component(ctx, entity, C)
    d, err4 := get_component(ctx, entity, D)
    return a, b, c, d, {}
}

get_components_5 :: proc(
    ctx: ^Context,
    entity: Entity,
    $A, $B, $C, $D, $E: typeid,
) -> (
    ^A,
    ^B,
    ^C,
    ^D,
    ^E,
    [5]ECS_Error,
) {
    a, err1 := get_component(ctx, entity, A)
    b, err2 := get_component(ctx, entity, B)
    c, err3 := get_component(ctx, entity, C)
    d, err4 := get_component(ctx, entity, D)
    e, err5 := get_component(ctx, entity, E)
    return a, b, c, d, e, {}
}

get_components :: proc {
    get_component,
    get_components_2,
    get_components_3,
    get_components_4,
    get_components_5,
}

get_component_slice_from_entities :: proc(
    ctx: ^Context,
    entities: []Entity,
    $T: typeid,
    allocator := context.allocator,
) -> []^T {
    context.user_ptr = ctx
    get_t_proc :: proc(h: Entity) -> ^T {
        e, err := get_component(cast(^Context)context.user_ptr, h, T) or_else nil
        return e
    }
    return slice.mapper(entities, get_t_proc)
}


remove_components_2 :: proc(
    ctx: ^Context,
    entity: Entity,
    $A, $B: typeid,
) -> (
    ^A,
    ^B,
    [2]ECS_Error,
) {
    a, err1 := remove_component(ctx, entity, A)
    b, err2 := remove_component(ctx, entity, B)
    return a, b, {}
}

remove_components_3 :: proc(
    ctx: ^Context,
    entity: Entity,
    $A, $B, $C: typeid,
) -> (
    ^A,
    ^B,
    ^C,
    [3]ECS_Error,
) {
    a, err1 := remove_component(ctx, entity, A)
    b, err2 := remove_component(ctx, entity, B)
    c, err3 := remove_component(ctx, entity, C)
    return a, b, c, {}
}

remove_components_4 :: proc(
    ctx: ^Context,
    entity: Entity,
    $A, $B, $C, $D: typeid,
) -> (
    ^A,
    ^B,
    ^C,
    ^D,
    [4]ECS_Error,
) {
    a, err1 := remove_component(ctx, entity, A)
    b, err2 := remove_component(ctx, entity, B)
    c, err3 := remove_component(ctx, entity, C)
    d, err4 := remove_component(ctx, entity, D)
    return a, b, c, d, {}
}

remove_components_5 :: proc(
    ctx: ^Context,
    entity: Entity,
    $A, $B, $C, $D, $E: typeid,
) -> (
    ^A,
    ^B,
    ^C,
    ^D,
    ^E,
    [5]ECS_Error,
) {
    a, err1 := remove_component(ctx, entity, A)
    b, err2 := remove_component(ctx, entity, B)
    c, err3 := remove_component(ctx, entity, C)
    d, err4 := remove_component(ctx, entity, D)
    e, err5 := remove_component(ctx, entity, E)
    return a, b, c, d, e, {}
}

remove_components :: proc {
    remove_component,
    remove_components_2,
    remove_components_3,
    remove_components_4,
    remove_components_5,
}
