import glob
import os
import argparse
import subprocess
import sys

def find_files_with_extensions(directory, extensions):
    files = []    
    for extension in extensions.split(','):
        search_pattern = os.path.join(directory, '**', f'{extension}')
        for file in glob.glob(search_pattern, recursive=True):
            files.append(file)        
    print(f'Found {len(files)} files')
    return files

def format_files_with_clang(files, format):
    clang_format_path = os.path.join(os.path.dirname(__file__), 'clang-format.exe')
    command = [clang_format_path, '--Werror', '-i'] 
    if not format:
        command.append('--dry-run')

    print("Run clang-format...")
    print(" ".join(command))
    command += files
    result = subprocess.run(command, stdout=subprocess.PIPE, text=True)
    print(result.stdout)
    print(f"{'Found some errors!' if result.returncode != 0 else 'All Good!'}")
    return result.returncode

parser = argparse.ArgumentParser(description='Format files with clang-format.')
parser.add_argument('--dir', type=str, default="../src/Apps",
                    help='directory path to search for files')
parser.add_argument('--format', action="store_true", default=False,
                    help='when defined format the file with clang-format, otherwise just check the formatting rules.')

args = parser.parse_args()

extensions = '*.c,*.h,*.cpp,*.hpp'
files = find_files_with_extensions(args.dir, extensions)
result = format_files_with_clang(files, args.format)
if result != 0:
    sys.exit(result)
