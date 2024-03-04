# codegen.py
# searches for .object files, which are yaml specifications of GameObject instances,
# and turns them into C++ implementations in a file called GameObjects.h
from pathlib import Path
import os
try:
    import yaml
except ImportError:
    print("you are missing pyyaml")
    print("pip3 install pyyaml --user")
    exit()

#helpers
class NameGenerator:
    def __init__(self,variable_name):
        self.id = 0
        self.variable_name = variable_name
    def next(self):
        self.id += 1
        return self.variable_name + str(self.id)
variable_name_generator = {}

def variable_name(var_type):
    if var_type not in variable_name_generator:
        variable_name_generator[var_type] = NameGenerator(var_type)
    return variable_name_generator[var_type].next()

# filename('/path/to/cool-file.txt') = 'cool-file'
def filename(filepath):
    return os.path.splitext(os.path.basename(filepath))[0]

def all_files_of_types(filetypes, path=''):
    result = []
    for filetype in filetypes:
        result.extend(Path(path).rglob('*.'+filetype))
    return result

components_needed = set()
object_types_declared = set()

# construct an odin assignment statement
# assign(['steve','favorite_color'],'green') = 'steve.favorite_color = green'
def assign(base_path,value):
    #some special cases for literals
    if(type(value) == str):
        value = '"' + value + '"'
    if(type(value) == bool):
        value = str(value).lower()
    if(type(value) == dict and 'code' in value):
        value = value['code']
    base_path = '.'.join(base_path)
    return f'{base_path} = {str(value)}\n'

# add a component and get a pointer to it
def declare(var_type, var_name, owner_name):
    return f'{var_name} : {var_type} = add_component({owner_name},{var_type});\n'

# get a pointer to an existing component
def modify(var_type, var_name, owner_name):
    return f'{var_type}* {var_name} = {owner_name}->getComponent<{var_type}>();\n'

def component_code(component, owner_name, modify_comps=True):
    result = ""
    spaces = "    "
    component_type = component
    if(type(component) == dict):
        component_type = list(component.keys())[0]
    if component_type in ignore_changes:
        return result
    if component_type in game_object_fields:
        result += spaces + assign(owner_name, component_type, component[component_type])
        return result
    components_needed.add(component_type)
    component_name = variable_name(component_type)
    statement = ""
    if(modify_comps):
        statement = modify(component_type,component_name, owner_name)
    else:
        statement = declare(component_type,component_name, owner_name)
    result += spaces + statement
    if(type(component) == dict):
        fields = component[component_type]
        if fields is not None:
            for field in fields:
                result += spaces + assign(component_name, field, fields[field])
    return result

def constructor_body(components, owner_name, child = True):
    result = ""
    spaces = "    "
    if components is None:
        return result
    for component in components:
        component_type = component
        if(type(component) == dict):
            component_type = list(component.keys())[0]
            if(type(component[component_type]) == list):#another GameObject
                new_obj_name = variable_name(component_type)
                if(child):
                    result += spaces + modify(component_type, new_obj_name, owner_name)
                else:
                    result += spaces + declare(component_type, new_obj_name, owner_name)
                result += constructor_body(component[component_type],new_obj_name)
            else:
                result += component_code(component, owner_name, modify_comps=child)
        else:
            result += component_code(component, owner_name, modify_comps=child)
    return result
basic_components = ["transform","rigidbody","collider","sprite","animation"]
#for one component, get component bits to set
def get_component_set(component):
    components = []
    if component.lower() in basic_components:
        components.append(component.capitalize())
    else:
        components.append('Script')
    return set(components)
#for all components, get component bits to set
def get_total_component_set(component_names):
    component_set = set()
    for component in component_names:
        component_set = component_set.union(get_component_set(component))
    return component_set
def set_to_enum_literal(component_set):
    result = "{ "
    variants = []
    for component in component_set:
        variants.append("." + component)
    result += ", ".join(variants)
    result += " }"
    return result
#is component in main package, or else is it in its own package?
def in_main_package(component_name):
    if component_name.lower() in basic_components:
        return False
    return True
#get code for initializing the components to defaults in the struct literal
def init_components(components):
    component_names = []
    for component in components:
        if type(component) == str:
            component_names.append(component)
        if type(component) == dict:
            component_names.extend(component.keys())
    result = ""
    component_set = set_to_enum_literal(get_total_component_set(component_names))
    result += f"component_set = {component_set},\n"
    for component in component_names:
        pkg_dot = "" if in_main_package(component) else (component + ".")
        result += f"        {component} = {pkg_dot}default_{component}(),\n"
    return result

#get code for setting fields 
def set_fields(components,owner_name):
    result = ""
    for component in components:
        if type(component) != dict:
            continue
        assert(len(component.keys()) == 1)
        component_name = list(component.keys())[0]
        values = component[component_name]
        for k in values:
            v = values[k]
            result += "    " + assign([owner_name,component_name,k],v)
    return result
        
constructors_header = """package main
import "core:math/rand"
// AUTOGENERATED object constructor procs from .object files
""" 
object_types = ""
constructors = ""
obj_filenames = all_files_of_types(['object','scene'])
for obj_filename in sorted(obj_filenames):
    with open(obj_filename, 'r') as obj_file:
        object_type = filename(obj_filename)
        object_types_declared.add(object_type)
        yaml_contents = yaml.safe_load(obj_file.read())
        init_components_code = init_components(yaml_contents)
        set_fields_code = set_fields(yaml_contents, 'obj')
        constructor_code = """
example :: proc() -> GameObject {
    obj := GameObject {
        //init-components
    }
//set-fields
    return obj
}
        """.replace("example",object_type)\
            .replace("//init-components",init_components_code.rstrip())\
            .replace('//set-fields',set_fields_code.rstrip())
        constructors += constructor_code.rstrip() + '\n'

constructor_code = constructors_header + constructors
output_directory = '.'
if not os.path.exists(output_directory):
    os.makedirs(output_directory)
constructor_filepath = output_directory + '/' + 'generated_objects.odin'
if(os.path.exists(constructor_filepath) and open(constructor_filepath,'r').read() == constructor_code):
    exit() #if generated code was unchanged, don't make the compiler think it was (for incremental compilation)
constructor_file = open(constructor_filepath, 'w')
constructor_file.write(constructor_code)
# print(constructor_code)

