import os
import sys
import shutil

def remove_add_subdirectory_from_cmakefile(file_path, app_name):
    with open(file_path, "r") as file:
        lines = file.readlines()

    with open(file_path, "w") as file:
        for line in lines:
            if line.strip("\n") != f"add_subdirectory({app_name})":
                file.write(line)

app_name = input("Insert the App Name to remove: ")

# prompt a are you sure message waiting for a yes or no
answer = input(f"Are you sure you want to remove {app_name} (yes/no)? ")
if answer != "yes" and answer != "y":
    print("Removal canceled.")
    sys.exit(1)

app_project_name = app_name.strip().replace(" ", "")
dst_project_folder = "../../src/Apps/" + app_project_name
dst_data_folder = "../../resources/" + app_project_name

if not os.path.exists(dst_project_folder) or not os.path.exists(dst_data_folder):
    print(f"Project or Data folders do not exist. Can't remove the app.")
    sys.exit(1)

# Remove the folders
shutil.rmtree(dst_project_folder)
shutil.rmtree(dst_data_folder)

remove_add_subdirectory_from_cmakefile("../../src/Apps/CMakeLists.txt", app_project_name)
remove_add_subdirectory_from_cmakefile("../../resources/CMakeLists.txt", app_project_name)