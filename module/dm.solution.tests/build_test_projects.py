import re
import os
import subprocess
from pathlib import Path

def write_project_executable(filepath, project_executable_list):
    with open(filepath, 'w', encoding='utf-8') as file:
        for project_executable in project_executable_list:
            file.write(str(project_executable) + '\n')  # Add a newline character after each line

# Specify the solution file and the target folder name
solution_file = "D:/CLOUD/OneDrive/DEV/DREAM/module/dm.solution.tests/tests.sln"
folder_name = "test"

# Regex pattern to match projects in the solution file
pattern = re.compile(r'Project\("\{[A-F0-9\-]+\}"\) = "(.+?)", "(.+?)", "(.+?)"')

# List to store projects in the specified folder
projects_to_build = []


# Read the solution file line by line
with open(solution_file, "r", encoding="utf-8") as file:
    for line in file:
        match = pattern.search(line)
        if match:
            project_name, project_path, project_guid = match.groups()
            # Check if the project path contains the specified folder
            if folder_name in project_path:
                projects_to_build.append((project_name, project_path))

# Build each project using MSBuild
project_executable_list = []
for project in projects_to_build:
    project_name = project[0]
    project_path = project[1]

    print(f"Building project: {project_path} ({project_name})")
    project_folder_path = os.path.dirname(project_path)
    executable_path = Path(os.path.join("D:/CLOUD/OneDrive/DEV/DREAM/module/dm.solution.tests/", f"{project_folder_path}/out/bin/win64-release/{project_name}.exe")).resolve(strict=False)

    try:
        subprocess.run(
            [
                "msbuild",
                project_path,
                "/t:Build",
                "/p:Configuration=Release",
                "/p:Platform=x64"
            ],
            check=True,
            shell=True,
        )
        project_executable_list.append(executable_path)
    except subprocess.CalledProcessError as e:
        print(f"Error building project {project_path}: {e}")

write_project_executable("D:/CLOUD/OneDrive/DEV/DREAM/module/dm.solution.tests/test_executable_list.txt", project_executable_list)
