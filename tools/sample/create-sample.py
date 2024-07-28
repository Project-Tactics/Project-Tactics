import shutil
import os
import sys
import re
import fileinput

def to_camel_case(any_str):
    # Remove non-alphanumeric characters and split based on spaces, dashes, or underscores
    components = re.split(r'[^a-zA-Z0-9]', any_str)
    # Capitalize the first letter of each component except the first one
    camel_case_str = components[0].lower() + ''.join(x.capitalize() for x in components[1:])
    return camel_case_str

def add_line_to_file_before(file_path, line_to_add, line_to_find):
    with open(file_path, 'r') as file:
        file_contents = file.readlines()

    insert_index = None
    for i, line in enumerate(file_contents):
        if line.strip() == line_to_find:
            insert_index = i
            break

    if insert_index is not None:
        file_contents.insert(insert_index, line_to_add)

    with open(file_path, 'w') as file:
        file.writelines(file_contents)

def copy_folder(src_folder, dst_folder):
    shutil.copytree(src_folder, dst_folder, dirs_exist_ok=True)

def replace_strings_in_files(folder_path, replacements, extensions=['.txt', '.cpp', '.h', '.ini', '.json', '.prefab']):
    # Iterate over all files in the folder
    for root, dirs, files in os.walk(folder_path):
        for file_name in files:
            # Check if the file has the correct extension
            if not any(file_name.endswith(ext) for ext in extensions):
                continue

            file_path = os.path.join(root, file_name)
            
            # Read the file and replace the strings
            with fileinput.FileInput(file_path, inplace=True) as file:
                for line in file:
                    for old_string, new_string in replacements.items():
                        line = line.replace(old_string, new_string)
                    print(line, end='')

    print("String replacements completed.")

def rename_files(folder_path, rename_dict):
    # Iterate over the rename dictionary
    for old_name, new_name in rename_dict.items():
        old_path = os.path.join(folder_path, old_name)
        new_path = os.path.join(folder_path, new_name)
        
        # Check if the old file exists
        if os.path.exists(old_path):
            # Rename the file
            os.rename(old_path, new_path)
            print(f"Renamed {old_name} to {new_name}")
        else:
            print(f"File {old_name} does not exist in {folder_path}")


sample_name = input("Insert the Sample name: ")

sample_project_name = sample_name.strip().replace(" ", "")
data_folder_target_name = sample_name.strip().replace(" ", "_").lower() + "_data"
src_project_folder = "./project_template"
src_data_folder = "./data_template"
dst_project_folder = "../../src/Apps/Samples/States"
dst_data_folder = "../../resources/Samples/data/_demo" + sample_project_name
demoState = f'Demo{sample_project_name}State'
demoStateHeader = f'{demoState}.h'
demoStateCpp = f'{demoState}.cpp'
cmakefile = "../../src/Apps/Samples/CMakeLists.txt"
appSrc = "../../src/Apps/Samples/SamplesApplication.cpp"
rename_dict = {
    'State.cpp': demoStateCpp,
    'State.h': demoStateHeader,
}
replacements = {
    '##SAMPLE_NAME##': sample_name,
    '##CAMERA_NAME##': to_camel_case(sample_name),
}

if os.path.exists(dst_data_folder):
    print(f"Project or Data folders already exists. Can't create the app.")
    sys.exit(1)

copy_folder(src_project_folder, dst_project_folder)
copy_folder(src_data_folder, dst_data_folder)
rename_files(dst_project_folder, rename_dict)
replace_strings_in_files(dst_project_folder, replacements)
replace_strings_in_files(dst_data_folder, replacements)
add_line_to_file_before(cmakefile, "    States/" + demoStateCpp + "\n", "main.cpp")
add_line_to_file_before(cmakefile, "    States/" + demoStateHeader + "\n", "SamplesUtils.h")
add_line_to_file_before(appSrc, f"#include \"States/{demoStateHeader}\"\n", "// ADD INCLUDE SAMPLES HERE")
line = f"\t_addSampleFlow<{demoState}>(\"{to_camel_case(sample_project_name)}\", \"{sample_project_name}\");\n"
add_line_to_file_before(appSrc, line, "// ADD FLOW SAMPLES HERE")
