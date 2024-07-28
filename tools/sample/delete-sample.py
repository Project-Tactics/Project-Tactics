import os
import sys
import shutil
import re

def to_camel_case(any_str):
    # Remove non-alphanumeric characters and split based on spaces, dashes, or underscores
    components = re.split(r'[^a-zA-Z0-9]', any_str)
    # Capitalize the first letter of each component except the first one
    camel_case_str = components[0].lower() + ''.join(x.capitalize() for x in components[1:])
    return camel_case_str

def remove_line_from_file(file_path, line_to_remove):
    # Read all lines from the file
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Filter out the line we want to remove
    lines = [line for line in lines if line.strip() != line_to_remove]

    # Write the modified lines back to the file
    with open(file_path, 'w') as file:
        file.writelines(lines)

sample_name = input("Insert the Sample to remove: ")

# prompt a are you sure message waiting for a yes or no
answer = input(f"Are you sure you want to remove {sample_name} (yes/no)? ")
if answer != "yes" and answer != "y":
    print("Removal canceled.")
    sys.exit(1)

sample_project_name = sample_name.strip().replace(" ", "")
dst_project_folder = "../../src/Apps/Samples/States/"
dst_data_folder = "../../resources/Samples/data/_demo" + sample_project_name
demoState = f'Demo{sample_project_name}State'
demoStateCpp = f'{demoState}.cpp'
demoStateHeader = f'{demoState}.h'
cmakefile = "../../src/Apps/Samples/CMakeLists.txt"
appSrc = "../../src/Apps/Samples/SamplesApplication.cpp"

if not os.path.exists(dst_data_folder):
    print(f"Project or Data folders do not exist. Can't remove the app.")
    sys.exit(1)

# Remove the folders
shutil.rmtree(dst_data_folder)

# Remove the files
os.remove(dst_project_folder + demoStateCpp)
os.remove(dst_project_folder + demoStateHeader)

remove_line_from_file(cmakefile, "States/" + demoStateCpp)
remove_line_from_file(cmakefile, "States/" + demoStateHeader)
remove_line_from_file(appSrc, f'#include \"States/{demoStateHeader}\"')
line = f"_addSampleFlow<{demoState}>(\"{to_camel_case(sample_project_name)}\", \"{sample_project_name}\");"
remove_line_from_file(appSrc, line)