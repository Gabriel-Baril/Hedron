import re
import os
import subprocess
from pathlib import Path

# Specify the solution file and the target folder name
solution_file = "D:/_DEV/_HEDRON/module/hdn.solution.hdef/hdef.sln"

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
            projects_to_build.append((project_name, project_path)) # Buil;d every project in the solution to make sur everything is up to date


# Build each project using MSBuild
for project in projects_to_build:
    project_name = project[0]
    project_path = project[1]

    print(f"Building project: {project_path} ({project_name})")
    project_folder_path = os.path.dirname(project_path)
    flatbuffer_schemas_path = os.path.join(project_folder_path, "schemas")
    src_dir_csharp = os.path.join(project_folder_path, "src/generated")

    if(os.path.exists(flatbuffer_schemas_path)):
        result = subprocess.run([
            "generate_flatbuffers.bat",
            flatbuffer_schemas_path,
            f"src/hdef", # out cpp path (hdn.data)
            src_dir_csharp
        ],
        check=True, 
        capture_output=True,
        text=True,
        stdin=subprocess.DEVNULL)

    # executable_path = Path(os.path.join("D:/_DEV/_HEDRON/module/hdn.solution.hdef/", f"{project_folder_path}/out/bin/win64-release/{project_name}.exe")).resolve(strict=False)

    try:
        subprocess.run(
            [
                "msbuild", 
                project_path, 
                "/t:Build", 
                "/p:Configuration=Release"
            ],
            check=True,
            shell=True,
            stdin=subprocess.DEVNULL
        )
    except subprocess.CalledProcessError as e:
        print(f"Error building project {project_path}: {e}")