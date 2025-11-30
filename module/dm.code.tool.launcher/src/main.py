import sys
from PySide6.QtWidgets import QApplication, QMainWindow, QMenuBar
from PySide6.QtGui import QAction
from ui_main import MainUI

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Hedron Launcher")
        self.resize(600, 400)

        self.main_ui = MainUI()
        self.setCentralWidget(self.main_ui)

        self.setup_menu()

    def setup_menu(self):
        menubar = self.menuBar()

        file_menu = menubar.addMenu("File")
        exit_action = QAction("Exit", self)
        exit_action.triggered.connect(self.close)
        file_menu.addAction(exit_action)

        help_menu = menubar.addMenu("Help")
        about_action = QAction("About", self)
        about_action.triggered.connect(self.show_about)
        help_menu.addAction(about_action)
        
    def show_about(self):
        print("This is a simple launcher to orchestrate builds in the hedron ecosystem")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())