from PyQt5.QtWidgets import QWidget, QLabel, QVBoxLayout, QPushButton
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QMovie

class WelcomePage(QWidget):
    def __init__(self, parent):
        super().__init__(parent)
        self.parent = parent
        self.init_ui()

    def init_ui(self):
        layout = QVBoxLayout()
        
        # Animation
        self.animation_label = QLabel()
        movie = QMovie(":../resources/animation.gif")
        self.animation_label.setMovie(movie)
        movie.start()
        
        # Welcome text
        welcome_label = QLabel("Welcome to Kraken OS Installer")
        welcome_label.setAlignment(Qt.AlignCenter)
        
        # Next button
        next_btn = QPushButton("Next →")
        next_btn.clicked.connect(lambda: self.parent.navigate_to(1))
        
        layout.addWidget(self.animation_label)
        layout.addWidget(welcome_label)
        layout.addWidget(next_btn)
        self.setLayout(layout)
