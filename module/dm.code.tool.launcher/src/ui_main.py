from PySide6.QtWidgets import (
    QWidget, QVBoxLayout, QLabel, QTabWidget, QProgressBar, QFormLayout, QLineEdit, QPushButton
)

import subprocess
import os
import shutil

class MainUI(QWidget):
    def __init__(self):
        super().__init__()

        layout = QVBoxLayout()
        self.setLayout(layout)

        tabs = QTabWidget()

        form_tab = QWidget()
        form_layout = QFormLayout()
        self.build_name_input = QLineEdit()
        self.scene_prefab_path_input = QLineEdit()
        submit_btn = QPushButton("Submit")
        submit_btn.clicked.connect(self.submit_form)

        form_layout.addRow("Build Name:", self.build_name_input) # dreamlike_20250613_scene_01_v1
        form_layout.addRow("Scene Prefab Path:", self.scene_prefab_path_input) # prefabs/example_prefab.xml
        form_layout.addRow(submit_btn)
        form_tab.setLayout(form_layout)

        tabs.addTab(form_tab, "Config")

        layout.addWidget(tabs)

    def compile_pipeline(self):
        pipeline_root_path = "D:/CLOUD/OneDrive/DEV/DREAM/module/dm.tool.pipeline" # TODO: Make it based on .ini
        project_path = f"{pipeline_root_path}/pipeline.vcxproj"
        try:
            subprocess.run(
                [
                    "msbuild",
                    project_path,
                    "/t:Build",
                    "/p:Configuration=Debug",
                    "/p:Platform=x64",
                    "/v:q",
                    "/nologo"
                ],
                check=True,
                shell=True
            )
        except subprocess.CalledProcessError as e:
            print(f"Error building project {project_path}: {e}")
            return False
        return True

    def build(self):
        pipeline_exe_path = "D:/CLOUD/OneDrive/DEV/DREAM/module/dm.tool.pipeline/out/bin/win64-debug/pipeline.exe"
        scene_path = self.scene_prefab_path_input.text()
        try:
            subprocess.run(
                [
                    pipeline_exe_path,
                    "--scene",
                    scene_path
                ],
                check=True,
                shell=True
            )
        except subprocess.CalledProcessError as e:
            print(f"Error invoke build process: {e}")
            return False
        return True

    def compile_game(self):
        root_path = "D:/CLOUD/OneDrive/DEV/DREAM/module/dm.game.dreamlike" # TODO: Make it based on .ini
        project_path = f"{root_path}/dreamlike.vcxproj"
        try:
            subprocess.run(
                [
                    "msbuild",
                    project_path,
                    "/t:Build",
                    "/p:Configuration=Debug",
                    "/p:Platform=x64",
                    "/v:q",
                    "/nologo"
                ],
                check=True,
                shell=True
            )
        except subprocess.CalledProcessError as e:
            print(f"Error building project {project_path}: {e}")
            return False
        return True

    def copy_folder_to_package(self, game_root_folder, build_output_path, folder_name):
        folder_to_package = os.path.join(game_root_folder,folder_name)
        shutil.copytree(folder_to_package, os.path.join(build_output_path, folder_name), dirs_exist_ok=True)

    def package_build(self):
        build_repo_folder = "D:/CLOUD/OneDrive/DEV/DREAM_BUILD"
        build_output_path = os.path.join(build_repo_folder, self.build_name_input.text())
        os.makedirs(build_output_path, exist_ok=True)

        game_root_folder = "D:/CLOUD/OneDrive/DEV/DREAM/module/dm.game.dreamlike"
        # Copy required code modules
        game_code_module_folder = os.path.join(game_root_folder,"out/bin/win64-debug")
        shutil.copytree(game_code_module_folder, build_output_path, dirs_exist_ok=True)

        # Copy data consumed by the exe
        self.copy_folder_to_package(game_root_folder, build_output_path, "shaders")
        self.copy_folder_to_package(game_root_folder, build_output_path, "objects")
        self.copy_folder_to_package(game_root_folder, build_output_path, "models")

    def submit_form(self):
        if not self.compile_pipeline():
            print("Failed to compile pipeline")
            return
        else:
            print("Pipeline compiled")

        if not self.build():
            print("Failed to complete the build")
            return
        else:
            print("Build finished")

        if not self.compile_game():
            print("Failed to compile game")
            return
        else:
            print("Game compiled")

        self.package_build()
        print("Build packaged")

