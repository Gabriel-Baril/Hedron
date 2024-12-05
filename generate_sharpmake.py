import os
import subprocess
import sys

def find_sharpmake_files(base_path, allowed_folders):
    """
    Recursively find all .sharpmake.cs files in the specified folders.

    :param base_path: The root directory to search.
    :param allowed_folders: A list of folder names to include in the search.
    :return: A list of full paths to .sharpmake.cs files in the allowed folders.
    """
    sharpmake_files = []
    for root, _, files in os.walk(base_path):
        # Check if the current directory is in the allowed folders
        if any(allowed_folder in os.path.relpath(root, base_path) for allowed_folder in allowed_folders):
            for file in files:
                if file.endswith(".sharpmake.cs"):
                    sharpmake_files.append(f".\\{os.path.join(os.path.relpath(root, base_path), file)}")
    return sharpmake_files

def run_sharpmake(sharpmake_exe, sharpmake_files):
    """
    Run Sharpmake.Application.exe with the collected .sharpmake.cs files.

    :param sharpmake_exe: Path to Sharpmake.Application.exe.
    :param sharpmake_files: List of .sharpmake.cs file paths.
    """
    if not sharpmake_files:
        print("No .sharpmake.cs files found in the specified folders.")
        return

    # Build the /sources argument
    sharpmake_files_str = ",".join(f"'{file}'" for file in sharpmake_files)
    args = f"/force /verbose /sources({sharpmake_files_str.replace("\\","/")})" # Replacing backward slashes with forward one since the sharpmake parser require it

    # Construct the command
    command = f'{sharpmake_exe} {args}'

    print(f"Running: {command}")

    # Execute the command
    try:
        result = subprocess.run(command, shell=True, check=True, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        print(result.stdout)
    except subprocess.CalledProcessError as e:
        print(f"Error running Sharpmake: {e.stderr}")

def main():
    # Define the base path and Sharpmake executable path
    base_path = os.getcwd()  # Current directory or specify a path
    sharpmake_exe = sys.argv[1]

    # Allowed folders
    allowed_folders = ["External", "Module", "Plugin", "Solution", "Playground"]

    print(f"Searching for .sharpmake.cs files in {allowed_folders} folders under {base_path}...")
    
    # Find all .sharpmake.cs files in allowed folders
    sharpmake_files = find_sharpmake_files(base_path, allowed_folders)
    sharpmake_files.append('./Main.sharpmake.cs') # The main sharpmake file

    if sharpmake_files:
        print(f"Found {len(sharpmake_files)} .sharpmake.cs files:")
        for file in sharpmake_files:
            print(f"  - {file}")
    else:
        print("No .sharpmake.cs files found in the specified folders.")

    # Run Sharpmake with the found files
    run_sharpmake(sharpmake_exe, sharpmake_files)

if __name__ == "__main__":
    main()
