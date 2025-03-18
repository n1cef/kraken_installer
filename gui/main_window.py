from PyQt5.QtWidgets import QMainWindow, QStackedWidget
from pages.welcome_page import WelcomePage
#from pages.partition_page import PartitionPage
#from pages.rsync_page import RsyncPage
#from pages.grub_page import GrubPage
#from pages.location_page import LocationPage
#from pages.keyboard_page import KeyboardPage
#from pages.user_page import UserPage
class InstallerWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.stacked_widget = QStackedWidget()
        self.pages = [
           WelcomePage(self),
            #PartitionPage(self),
            #RsyncPage(self),
            # GrubPag(self),
            #LocationPag(self),
            #KeyboardPage(self),
            #UserPage(self)
        ]
        for page in self.pages:
            self.stacked_widget.addWidget(page)
        self.setCentralWidget(self.stacked_widget)
        self.navigate_to(0)

    def navigate_to(self, index):
        self.stacked_widget.setCurrentIndex(index)
