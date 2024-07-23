import shutil
import os
import sys
import fileinput

def update_cmake_project_file(cmake_file_path, folder_name):
    with open(cmake_file_path, 'r') as file:
        cmake_contents = file.readlines()

    # Add the new add_subdirectory command if it doesn't already exist
    add_subdirectory_line = f'add_subdirectory({folder_name})\n'
    if add_subdirectory_line not in cmake_contents:
        cmake_contents.append(add_subdirectory_line)

    # Write the updated contents back to the CMakeLists.txt file
    with open(cmake_file_path, 'w') as file:
        file.writelines(cmake_contents)

def copy_folder(src_folder, dst_folder):
    if not os.path.exists(dst_folder):
        shutil.copytree(src_folder, dst_folder)
        print(f"Folder copied to {dst_folder}")
    else:
        print(f"Destination folder {dst_folder} already exists")
        sys.exit(1)

def replace_strings_in_files(folder_path, replacements, extensions=['.txt', '.cpp', '.h', '.ini']):
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


app_name = input("Insert the App Name: ")

app_project_name = app_name.strip().replace(" ", "")
data_folder_target_name = app_name.strip().replace(" ", "_").lower() + "_data"
src_project_folder = "./app_project_template"
src_data_folder = "./app_data_template"
dst_project_folder = "../../src/Apps/" + app_project_name
dst_data_folder = "../../resources/" + app_project_name
rename_dict = {
    'TemplateApplication.cpp': f'{app_project_name}Application.cpp',
    'TemplateApplication.h': f'{app_project_name}Application.h',
}
replacements = {
    '##APP_NAME##': app_name,
    '##APP_PROJECT_NAME##': app_project_name,
    '##DATA_FOLDER_TARGET_NAME##': data_folder_target_name
}

if os.path.exists(dst_project_folder) or os.path.exists(dst_data_folder):
    print(f"Project or Data folders already exists. Can't create the app.")
    sys.exit(1)

copy_folder(src_project_folder, dst_project_folder)
copy_folder(src_data_folder, dst_data_folder)
rename_files(dst_project_folder, rename_dict)
replace_strings_in_files(dst_project_folder, replacements)
replace_strings_in_files(dst_data_folder, replacements)
update_cmake_project_file("../../src/Apps/CMakeLists.txt", app_project_name)
update_cmake_project_file("../../resources/CMakeLists.txt", app_project_name)
