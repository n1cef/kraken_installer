import subprocess
from PyQt5.QtCore import QObject, pyqtSignal

class DiskManager(QObject):
    disks_updated = pyqtSignal(list)
    
    def get_disks(self):
        try:
            output = subprocess.check_output(
                ["lsblk", "-d", "-o", "NAME,SIZE,MODEL"]
            ).decode().split('\n')[1:-1]
            self.disks_updated.emit(output)
        except subprocess.CalledProcessError as e:
            print(f"Error listing disks: {e}")
