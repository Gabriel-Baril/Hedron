import os
import subprocess
import argparse

def find_files_with_extensions(base_path, allowed_folders, allowed_extensions):
    out_files = []
    for root, _, files in os.walk(base_path):
        # Check if the current directory is in the allowed folders
        if any(allowed_folder in os.path.relpath(root, base_path) for allowed_folder in allowed_folders):
            for file in files:
                for extension in allowed_extensions:
                    if file.endswith(extension):
                        out_files.append(f".\\{os.path.join(os.path.relpath(root, base_path), file)}")
                        break
    return out_files

def find_sharpmake_files(base_path, allowed_folders):
    """
    Recursively find all .sharpmake.cs files in the specified folders.

    :param base_path: The root directory to search.
    :param allowed_folders: A list of folder names to include in the search.
    :return: A list of full paths to .sharpmake.cs files in the allowed folders.
    """
    return find_files_with_extensions(base_path, allowed_folders, [".sharpmake.cs"])

def run_sharpmake(sharpmake_exe, sharpmake_files):
    """
    Run Sharpmake.Application.exe with the collected .sharpmake.cs files.

    :param sharpmake_exe: Path to Sharpmake.Application.exe.
    :param sharpmake_files: List of .sharpmake.cs file paths.
    """
    print(f"---------- RUNNING SHARPMAKE ----------")
    if not sharpmake_files:
        print("No .sharpmake.cs files found in the specified folders.")
        return

    # Build the /sources argument
    sharpmake_files_str = ",".join(f"'{file}'" for file in sharpmake_files)
    args = f"/verbose /sources({sharpmake_files_str.replace("\\","/")})" # Replacing backward slashes with forward one since the sharpmake parser require it

    # Construct the command
    command = f'{sharpmake_exe} {args}'

    # Execute the command
    try:
        subprocess.run(command)
    except subprocess.CalledProcessError as e:
        print(f"Error running Sharpmake: {e.stderr}")

def clean_generated_files(base_path, allowed_folders, extensions):
    files_to_remove = find_files_with_extensions(base_path, allowed_folders, extensions)
    print(f"---------- CLEANING GENERATED FILES ----------")
    print(f"Found {len(files_to_remove)} Files to Clean")
    for file in files_to_remove:
        try:
            os.remove(file)
            print(f"    CLEANED '{file}'")
        except PermissionError:
            print(f"    [PermissionError] Cannot clean '{file}'")

def main():
    # Define the base path and Sharpmake executable path
    base_path = os.getcwd()  # Current directory or specify a path
    parser = argparse.ArgumentParser(description="A script to generate & clean module within /Hedron")
    parser.add_argument("--sharppath", type=str, help="The sharpmake path to use during generation")
    parser.add_argument("--clean", action="store_true", help="Clean sharpmake generated files")

    args = parser.parse_args()
    print(f"---------- ARGUMENTS ----------")
    sharpmake_exe = args.sharppath
    clean_files = args.clean
    print(f"[ARG] sharppath: {sharpmake_exe}")
    print(f"[ARG] clean: {clean_files}")

    # Allowed folders
    allowed_folders = ["Module"]


    print(f"---------- SHARPMAKE FILE SEARCH ----------")
    print(f"Searching for .sharpmake.cs files in {allowed_folders} folders under {base_path}")
    
    # Find all .sharpmake.cs files in allowed folders
    sharpmake_files = find_sharpmake_files(base_path, allowed_folders)
    sharpmake_files.append('.\\Main.sharpmake.cs') # The main sharpmake file

    if sharpmake_files:
        print(f"Found {len(sharpmake_files)} .sharpmake.cs files:")
        for file in sharpmake_files:
            print(f"    FOUND {file}")
    else:
        print("No .sharpmake.cs files found in the specified folders.")

    # Run Sharpmake with the found files
    if clean_files:
        extensions = [ ".sln", ".vcxproj", ".vcxproj.filters", ".vcxproj.user", ".csproj"]
        clean_generated_files(base_path, allowed_folders, extensions)
    
    run_sharpmake(sharpmake_exe, sharpmake_files)

if __name__ == "__main__":
    main()
