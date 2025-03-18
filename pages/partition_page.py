from PyQt5.QtWidgets import (
    QWidget,
    QVBoxLayout,
    QHBoxLayout,
    QLabel,
    QComboBox,
    QTableWidget,
    QTableWidgetItem,
    QPushButton,
    QMessageBox
)
from PyQt5.QtCore import Qt
#from .brain.disk_operations import list_disks, get_partitions  # Assume these functions exist in your disk_operations.py
from brain.disk_operations import DiskManager
class PartitionPage(QWidget):
    def __init__(self, parent):
        super().__init__(parent)
        self.parent = parent
        self.selected_disk = None
        self.init_ui()
        self.populate_disks()

    def init_ui(self):
        layout = QVBoxLayout()
        
        # Disk selection
        disk_label = QLabel("Select Installation Disk:")
        disk_label.setStyleSheet("font-weight: bold; font-size: 14px;")
        self.disk_combo = QComboBox()
        self.disk_combo.setFixedHeight(30)
        
        # Partition table
        self.partition_table = QTableWidget(0, 4)
        self.partition_table.setHorizontalHeaderLabels(["Device", "Size", "Type", "Mount Point"])
        self.partition_table.horizontalHeader().setStretchLastSection(True)
        self.partition_table.verticalHeader().setVisible(False)
        
        # Buttons
        btn_layout = QHBoxLayout()
        back_btn = QPushButton("← Back")
        back_btn.setStyleSheet("min-width: 80px;")
        back_btn.clicked.connect(lambda: self.parent.navigate_to(0))
        
        next_btn = QPushButton("Next →")
        next_btn.setStyleSheet("min-width: 80px;")
        next_btn.clicked.connect(self.validate_selection)
        
        btn_layout.addWidget(back_btn)
        btn_layout.addWidget(next_btn)
        
        layout.addWidget(disk_label)
        layout.addWidget(self.disk_combo)
        layout.addWidget(self.partition_table)
        layout.addLayout(btn_layout)
        self.setLayout(layout)

    def populate_disks(self):
        try:
            self.disk_combo.clear()
            disks = DiskManager.list_disks()  # Implement this in disk_operations.py
            for disk in disks:
                self.disk_combo.addItem(f"{disk['name']} ({disk['size']})", disk['name'])
            self.disk_combo.currentIndexChanged.connect(self.update_partition_table)
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Failed to load disks: {str(e)}")

    def update_partition_table(self):
        try:
            self.partition_table.setRowCount(0)
            disk = self.disk_combo.currentData()
            partitions = get_partitions(disk)  # Implement this in disk_operations.py
            
            for idx, part in enumerate(partitions):
                self.partition_table.insertRow(idx)
                self.partition_table.setItem(idx, 0, QTableWidgetItem(part['device']))
                self.partition_table.setItem(idx, 1, QTableWidgetItem(part['size']))
                self.partition_table.setItem(idx, 2, QTableWidgetItem(part['type']))
                self.partition_table.setItem(idx, 3, QTableWidgetItem(part['mountpoint']))
                
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Failed to load partitions: {str(e)}")

    def validate_selection(self):
        if self.disk_combo.currentIndex() == -1:
            QMessageBox.warning(self, "Warning", "Please select a disk!")
            return
            
        if not self.has_root_partition():
            QMessageBox.warning(self, "Warning", "No root partition selected!")
            return
            
        self.parent.selected_disk = self.disk_combo.currentData()
        self.parent.navigate_to(2)  # Navigate to rsync page

    def has_root_partition(self):
        for row in range(self.partition_table.rowCount()):
            if self.partition_table.item(row, 3) and \
               self.partition_table.item(row, 3).text() == "/":
                return True
        return False