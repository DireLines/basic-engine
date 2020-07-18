# codegen.py
# searches for .object files, which are yaml specifications of GameObject instances,
# and turns them into C++ implementations in a file called GameObjects.h
import subprocess
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

# filename('/path/to/cool-file.txt') = 'cool-file'
def filename(filepath):
    return os.path.splitext(os.path.basename(obj_filename))[0]

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

# construct a C++ declare/initialize statement
def declare(var_type, var_name):
    return f'{var_type}* {var_name} = new {var_type}();\n'

def add_component(owner_name, var_name):
    return f'{owner_name}->addComponent({var_name});\n'

def component_code(component, owner_name):
    result = ""
    spaces = "    "
    component_type = component
    if(type(component) == dict):
        component_type = list(component.keys())[0]
    if component_type not in variable_name_generator:
        variable_name_generator[component_type] = NameGenerator(component_type)
    component_name = variable_name_generator[component_type].next()
    result += spaces + declare(component_type,component_name)
    if(type(component) == dict):
        fields = component[component_type]
        if fields is not None:
            for field in fields:
                result += spaces + assign(component_name, field, fields[field])
    result += spaces + add_component(owner_name, component_name)
    return result

header = """#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H
// AUTOGENERATED GameObject declarations from .object files
using namespace std;

#include "GameObject.h"
"""

components_needed = set()
classes_declared = set()

constructor_include = """#include "GameObjects.h"
// AUTOGENERATED GameObject constructors from .object files
""" 
classes = ""
constructors = ""
obj_filenames = subprocess.run('find . -name *.object'.split(), capture_output=True).stdout.decode('ascii').rstrip().split('\n')
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
        constructor_code = ""
        components = yaml.safe_load(obj_file.read())
        for component in components:
            component_type = component
            if(type(component) == dict):
                component_type = list(component.keys())[0]
            components_needed.add(component_type)
            constructor_code += component_code(component, 'this')
            #TODO: handle child GameObjects by putting this into a recursive function which changes owner
        constructor_code = """
Example::Example(){
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

