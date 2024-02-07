package ecs
import "base:runtime"
import "core:container/queue"
import "core:fmt"
import "core:slice"
import "core:strings"
import "core:time"

//largely adapted from https://github.com/NoahR02/odin-ecs
//I added different means of tracking indices
//and an optimization similar to archetype ECS
//to aid the common access pattern for systems:
//each system cares about a particular set of components,
//and wants all the relevant components 
//of all entities that have all relevant components

Entity :: distinct uint

Context :: struct {
    entities:          Entities,
    components:        map[typeid]^runtime.Raw_Dynamic_Array,
    entity_indices:    map[Entity]map[typeid]uint,
    component_indices: map[typeid]map[Entity]uint,
    relevant_entities: map[string]Tracked_Entities, //maintain desired lists for faster querying
}

ECS_Error :: enum {
    NO_ERROR,
    ENTITY_DOES_NOT_HAVE_THIS_COMPONENT,
    ENTITY_DOES_NOT_MAP_TO_ANY_INDEX,
    ENTITY_ALREADY_HAS_THIS_COMPONENT,
    COMPONENT_NOT_REGISTERED,
    COMPONENT_IS_ALREADY_REGISTERED,
}

Tracked_Entities :: struct {
    components:     [dynamic]typeid,
    entity_indices: map[Entity]map[typeid]uint,
}

Entities :: struct {
    current_entity_id: uint,
    entities:          [dynamic]Entity_And_Some_Info,
    available_slots:   queue.Queue(uint),
}

Entity_And_Some_Info :: struct {
    entity:   Entity,
    is_valid: bool,
}



init_ecs :: proc() -> (ctx: Context) {
    create_entities :: proc(ctx: ^Context) {
        ctx.entities.entities = make([dynamic]Entity_And_Some_Info)
        queue.init(&ctx.entities.available_slots)
    }
    create_entities(&ctx)

    create_component_map :: proc(ctx: ^Context) {
        ctx.entity_indices = make(map[Entity]map[typeid]uint)
        ctx.component_indices = make(map[typeid]map[Entity]uint)
        ctx.relevant_entities = make(map[string]Tracked_Entities)
    }

    create_component_map(&ctx)

    return ctx
}

deinit_ecs :: proc(ctx: ^Context) {

    destroy_entities :: proc(ctx: ^Context) {
        delete(ctx.entities.entities)
        ctx.entities.current_entity_id = 0
        queue.destroy(&ctx.entities.available_slots)
    }
    destroy_entities(ctx)

    destroy_component_map :: proc(ctx: ^Context) {
        for key, value in ctx.components {
            free(value^.data)
            free(value)
        }

        delete(ctx.entity_indices)
        delete(ctx.component_indices)
    }
    destroy_component_map(ctx)
}

create_entity :: proc(ctx: ^Context) -> Entity {
    using ctx.entities

    if queue.len(available_slots) <= 0 {
        append_elem(&entities, Entity_And_Some_Info{Entity(current_entity_id), true})
        ctx.entity_indices[Entity(current_entity_id)] = make(map[typeid]uint)
        current_entity_id += 1
        return Entity(current_entity_id - 1)
    } else {
        index := queue.pop_front(&available_slots)
        entities[index] = Entity_And_Some_Info{Entity(index), true}
        return Entity(index)
    }

    return Entity(current_entity_id)
}

destroy_entity :: proc(ctx: ^Context, entity: Entity) {
    using ctx.entities

    for T, component in &ctx.entity_indices[entity] {
        remove_component_with_typeid(ctx, entity, T)
    }

    entities[uint(entity)] = {}
    queue.push_back(&available_slots, uint(entity))
}

@(private)
register_component :: proc(ctx: ^Context, $T: typeid) -> ECS_Error {
    is_type_a_key := T in ctx.components
    if is_type_a_key {
        return .COMPONENT_IS_ALREADY_REGISTERED
    } else {
        ctx.component_indices[T] = make(map[Entity]uint)
    }

    array := new([dynamic]T)
    ctx.components[T] = cast(^runtime.Raw_Dynamic_Array)array
    array^ = make_dynamic_array([dynamic]T)

    return .NO_ERROR
}

add_component :: proc(ctx: ^Context, entity: Entity, component: $T) -> (^T, ECS_Error) {
    register_component(ctx, T)
    if has_component(ctx, entity, T) {
        return nil, .ENTITY_ALREADY_HAS_THIS_COMPONENT
    }

    array := cast(^[dynamic]T)ctx.components[T]
    comp_map := &ctx.component_indices[T]
    // Add a new component to the component array.
    append_elem(array, component)

    // Map the entity to the new index, so we can lookup the component index later,
    comp_ind := &ctx.component_indices[T]
    comp_ind[entity] = len(array) - 1
    ent_ind := &ctx.entity_indices[entity]
    ent_ind[T] = len(array) - 1

    //relevant entity tracking
    for k, &v in ctx.relevant_entities {
        if entity in v.entity_indices {
            continue
        }
        if has_all_components(ctx, entity, v.components[:]) {
            relevant_components := make(map[typeid]uint)
            for component in v.components {
                relevant_components[component] = ctx.component_indices[component][entity]
            }
            v.entity_indices[entity] = relevant_components
        }
    }

    return &array[comp_map[entity]], .NO_ERROR
}

has_component :: proc(ctx: ^Context, entity: Entity, T: typeid) -> bool {
    return T in ctx.entity_indices[entity]
}

@(private)
remove_component_with_typeid :: proc(ctx: ^Context, entity: Entity, type_id: typeid) -> ECS_Error {
    using ctx.entities

    if !has_component(ctx, entity, type_id) {
        return .ENTITY_DOES_NOT_HAVE_THIS_COMPONENT
    }
    index := ctx.entity_indices[entity][type_id]

    array_len := ctx.components[type_id]^.len
    array := ctx.components[type_id]^.data

    delete_key(&ctx.entity_indices[entity], type_id)
    delete_key(&ctx.component_indices[type_id], entity)

    //relevant entity tracking
    for k, &v in ctx.relevant_entities {
        if entity in v.entity_indices {
            delete(v.entity_indices[entity])
            delete_key(&v.entity_indices, entity)
        }
    }

    return .NO_ERROR
}

remove_component :: proc(ctx: ^Context, entity: Entity, $T: typeid) -> ECS_Error {
    return remove_component_with_typeid(ctx, entity, T)
}

get_component :: proc(
    ctx: ^Context,
    entity: Entity,
    $T: typeid,
) -> (
    component: ^T,
    error: ECS_Error,
) {

    if !has_component(ctx, entity, T) {
        return nil, .ENTITY_DOES_NOT_HAVE_THIS_COMPONENT
    }

    array := cast(^[dynamic]T)ctx.components[T]
    index, is_entity_a_key := ctx.component_indices[T][entity]

    if !is_entity_a_key {
        return nil, .ENTITY_DOES_NOT_MAP_TO_ANY_INDEX
    }

    return &array[index], .NO_ERROR
}

get_component_list :: proc(ctx: ^Context, $T: typeid) -> ([]T, ECS_Error) {
    array := cast(^[dynamic]T)ctx.components[T]

    return array[:], .NO_ERROR
}

set_component :: proc(ctx: ^Context, entity: Entity, component: $T) -> ECS_Error {
    if !has_component(ctx, entity, T) {
        return .COMPONENT_NOT_REGISTERED
    }
    index, is_entity_a_key := ctx.component_map[T].entity_indices[entity]

    if !is_entity_a_key {
        return .ENTITY_DOES_NOT_MAP_TO_ANY_INDEX
    }
    array := cast(^[dynamic]T)ctx.component_map[T].data
    array[index] = component
    return .NO_ERROR
}


is_entity_valid :: proc(ctx: ^Context, entity: Entity) -> bool {
    if uint(entity) >= len(ctx.entities.entities) {
        return false
    }
    return ctx.entities.entities[uint(entity)].is_valid
}


get_relevant_components :: proc(
    ctx: ^Context,
    components: []typeid,
) -> (
    entity_indices: map[Entity]map[typeid]uint,
) {
    key := get_key_from_typeids(components)
    if key in ctx.relevant_entities {
        return ctx.relevant_entities[key].entity_indices
    }
    entity_indices = make(map[Entity]map[typeid]uint)
    if len(components) <= 0 {
        return entity_indices
    }
    if components[0] not_in ctx.component_indices {
        return entity_indices
    }
    for entity, _ in ctx.component_indices[components[0]] {

        has_all_needed_components := true
        for comp_type in components[1:] {
            if !has_component(ctx, entity, comp_type) {
                has_all_needed_components = false
                break
            }
        }

        if has_all_needed_components {
            relevant_components := make(map[typeid]uint)
            for component in components {
                relevant_components[component] = ctx.component_indices[component][entity]
            }
            entity_indices[entity] = relevant_components
        }

    }
    return
}
get_entities_with_components :: proc(
    ctx: ^Context,
    components: []typeid,
) -> (
    entities: [dynamic]Entity,
) {
    entity_indices := get_relevant_components(ctx, components)
    for k, _ in entity_indices {
        append(&entities, k)
    }
    return
}

has_all_components :: proc(ctx: ^Context, entity: Entity, components: []typeid) -> bool {
    for comp_type in components {
        if !has_component(ctx, entity, comp_type) {
            return false
        }
    }
    return true
}

//the slow but more straightforwardly correct way, for comparison
get_entities_with_components_prev :: proc(
    ctx: ^Context,
    components: []typeid,
) -> (
    entities: [dynamic]Entity,
) {
    entities = make([dynamic]Entity)
    if len(components) <= 0 {
        return entities
    }
    for entity, _ in ctx.component_indices[components[0]] {

        has_all_components := true
        for comp_type in components[1:] {
            if !has_component(ctx, entity, comp_type) {
                has_all_components = false
                break
            }
        }

        if has_all_components {
            append_elem(&entities, entity)
        }

    }

    return entities
}

builder: strings.Builder
get_key_from_typeids :: proc(components: []typeid) -> (key: string) {
    defer strings.builder_reset(&builder)
    component_u64s := make([]u64, len(components))
    for component, i in components {
        component_u64s[i] = transmute(u64)component
    }
    slice.sort(component_u64s)
    k := fmt.sbprint(&builder, component_u64s)
    return k
}
track_entities_with_components :: proc(ctx: ^Context, components: []typeid) {
    key := get_key_from_typeids(components)
    if key in ctx.relevant_entities {
        return
    }
    new_components := make([dynamic]typeid, len(components))
    for _, i in components {
        new_components[i] = components[i]
    }
    rel_ents := &ctx.relevant_entities
    rel_ents[key] = Tracked_Entities {
        entity_indices = get_relevant_components(ctx, components),
        components     = new_components,
    }
}
