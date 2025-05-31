from PySide6.QtWidgets import (
    QWidget, QVBoxLayout, QLabel, QTabWidget, QProgressBar, QFormLayout, QLineEdit, QPushButton
)

class MainUI(QWidget):
    def __init__(self):
        super().__init__()

        layout = QVBoxLayout()
        self.setLayout(layout)

        tabs = QTabWidget()

        form_tab = QWidget()
        form_layout = QFormLayout()
        self.name_input = QLineEdit()
        self.email_input = QLineEdit()
        submit_btn = QPushButton("Submit")
        submit_btn.clicked.connect(self.submit_form)

        form_layout.addRow("Name:", self.name_input)
        form_layout.addRow("Email:", self.email_input)
        form_layout.addRow(submit_btn)
        form_tab.setLayout(form_layout)

        progress_tab = QWidget()
        progress_layout = QVBoxLayout()
        self.progress_bar = QProgressBar()
        self.progress_bar.setValue(40)
        progress_layout.addWidget(QLabel("Progress:"))
        progress_layout.addWidget(self.progress_bar)
        progress_tab.setLayout(progress_layout)

        tabs.addTab(form_tab, "Form")
        tabs.addTab(progress_tab, "Progress")

        layout.addWidget(tabs)

    def submit_form(self):
        print(f"Name: {self.name_input.text()}")
        print(f"Email: {self.email_input.text()}")
        