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
    return os.path.splitext(os.path.basename(obj_filename))[0]

def all_files_of_types(filetypes, path=''):
    result = []
    for filetype in filetypes:
        result.extend(Path(path).rglob('*.'+filetype))
    return result

# if adding a new public variable to GameObject.h,
# also add it to this list, or .object file parser
# will assume it's the name of some component
game_object_fields = ["enabled", "name"]

# these are set automatically, so ignore scripts attempting to change them
ignore_changes = ["type","gameObject"]

components_needed = set()
classes_declared = set()

#the script header contains some useful stuff 
#which GameObjects should always know about
components_needed.add("Script")

# construct a C++ assignment statement
# assign('steve','favorite_color','green') = 'steve->favorite_color = green;'
def assign(owner,field,value):
    #some special cases for literals
    if(type(value) == str):
        value = '"' + value + '"'
    if(type(value) == bool):
        value = str(value).lower()
    if(type(value) == dict and 'code' in value):
        value = value['code']
    return f'{owner}->{field} = {str(value)};\n'

# add a component and get a pointer to it
def declare(var_type, var_name, owner_name):
    return f'{var_type}* {var_name} = {owner_name}->addComponent<{var_type}>();\n'

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

header = """#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H
// AUTOGENERATED GameObject declarations from .object files

#include "GameObject.h"
"""

constructor_include = """#include "GameObjects.h"
#include "Random.h"
// AUTOGENERATED GameObject constructors from .object files
""" 
classes = ""
constructors = ""
obj_filenames = all_files_of_types(['object','scene'])
for obj_filename in sorted(obj_filenames):
    with open(obj_filename, 'r') as obj_file:
        class_name = filename(obj_filename)
        classes_declared.add(class_name)
        class_decl = """
class Example: public GameObject {
public:
    Example();
};
        """.replace("Example",class_name)
        classes += class_decl.rstrip() + '\n'
        constructor_code = constructor_body(yaml.safe_load(obj_file.read()), 'this', child=False)
        constructor_code = """
Example::Example() {
    type = "Example";
//initialization
}
        """.replace("Example",class_name).replace("//initialization",constructor_code.rstrip())
        constructors += constructor_code.rstrip() + '\n'

component_includes = ""
for component in sorted(components_needed - classes_declared):
    component_includes += f'#include "{component}.h"\n'

footer = "\n#endif\n"

header_code = header + component_includes + classes + footer
constructor_code = constructor_include + constructors
output_directory = './src/generated/'
if not os.path.exists(output_directory):
    os.makedirs(output_directory)
header_filepath = output_directory + 'GameObjects.h'
constructor_filepath = output_directory + 'GameObjects.cpp'
if(os.path.exists(header_filepath) and open(header_filepath,'r').read() == header_code):
    if(os.path.exists(constructor_filepath) and open(constructor_filepath,'r').read() == constructor_code):
        exit() #if generated code was unchanged, don't make the compiler think it was
header_file = open(header_filepath,'w')
header_file.write(header_code)
constructor_file = open(constructor_filepath, 'w')
constructor_file.write(constructor_code)

