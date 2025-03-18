import subprocess
from PyQt5.QtWidgets import (
    QWidget, QVBoxLayout, QHBoxLayout, QLabel, QComboBox, QTableWidget,
    QTableWidgetItem, QPushButton, QMessageBox, QRadioButton, QButtonGroup,
    QGroupBox
)
from PyQt5.QtCore import Qt, pyqtSignal
from brain.disk_operations import DiskManager

class PartitionPage(QWidget):
    partition_scheme_changed = pyqtSignal(str)  # 'auto' or 'manual'

    def __init__(self, parent):
        super().__init__(parent)
        self.parent = parent
        self.selected_disk = None
        self.init_ui()
        self.populate_disks()

    def init_ui(self):
        layout = QVBoxLayout()

        # Partition scheme selection
        self.scheme_group = QGroupBox("Partitioning Scheme")
        scheme_layout = QVBoxLayout()
        
        self.auto_radio = QRadioButton("Automatic Partitioning (Recommended)")
        self.manual_radio = QRadioButton("Manual Partitioning (Advanced)")
        self.auto_radio.setChecked(True)
        
        self.btn_group = QButtonGroup()
        self.btn_group.addButton(self.auto_radio)
        self.btn_group.addButton(self.manual_radio)
        self.btn_group.buttonToggled.connect(self.on_scheme_changed)
        
        scheme_layout.addWidget(self.auto_radio)
        scheme_layout.addWidget(self.manual_radio)
        self.scheme_group.setLayout(scheme_layout)
        layout.addWidget(self.scheme_group)

        # Automatic partitioning UI
        self.auto_ui = QGroupBox("Automatic Partition Scheme")
        auto_layout = QVBoxLayout()
        self.size_warning = QLabel()
        self.size_warning.setStyleSheet("color: red;")
        self.partition_preview = QTableWidget(0, 3)
        self.partition_preview.setHorizontalHeaderLabels(["Mount Point", "Size", "Type"])
        auto_layout.addWidget(self.size_warning)
        auto_layout.addWidget(self.partition_preview)
        self.auto_ui.setLayout(auto_layout)
        layout.addWidget(self.auto_ui)

        # Manual partitioning UI
        self.manual_ui = QGroupBox("Manual Partitioning Tools")
        manual_layout = QHBoxLayout()
        self.cfdisk_btn = QPushButton("Launch cfdisk")
        self.parted_btn = QPushButton("Launch parted")
        self.refresh_btn = QPushButton("Refresh Partitions")
        manual_layout.addWidget(self.cfdisk_btn)
        manual_layout.addWidget(self.parted_btn)
        manual_layout.addWidget(self.refresh_btn)
        self.manual_ui.setLayout(manual_layout)
        self.manual_ui.hide()
        layout.addWidget(self.manual_ui)

        # Disk selection
        disk_label = QLabel("Select Installation Disk:")
        self.disk_combo = QComboBox()
        self.disk_combo.currentIndexChanged.connect(self.on_disk_changed)
        
        # Buttons
        btn_layout = QHBoxLayout()
        back_btn = QPushButton("← Back")
        back_btn.clicked.connect(lambda: self.parent.navigate_to(0))
        next_btn = QPushButton("Next →")
        next_btn.clicked.connect(self.validate_selection)
        
        layout.addWidget(disk_label)
        layout.addWidget(self.disk_combo)
        layout.addLayout(btn_layout)
        self.setLayout(layout)

        # Connect manual tools
        self.cfdisk_btn.clicked.connect(self.launch_cfdisk)
        self.parted_btn.clicked.connect(self.launch_parted)
        self.refresh_btn.clicked.connect(self.populate_disks)

    def populate_disks(self):
        try:
            self.disk_combo.clear()
            disks = DiskManager.list_disks()
            for disk in disks:
                self.disk_combo.addItem(
                    f"{disk['name']} ({disk['size']} - {disk['model']})",
                    disk['name']
                )
            self.disk_combo.currentIndexChanged.connect(self.on_disk_changed)
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Failed to load disks: {str(e)}")

    def on_scheme_changed(self):
        if self.auto_radio.isChecked():
            self.auto_ui.show()
            self.manual_ui.hide()
            self.partition_scheme_changed.emit('auto')
        else:
            self.auto_ui.hide()
            self.manual_ui.show()
            self.partition_scheme_changed.emit('manual')

    def on_disk_changed(self):
        disk = self.disk_combo.currentData()
        if not disk:
            return
            
        if self.auto_radio.isChecked():
            self.update_auto_partition_preview(disk)

    def update_auto_partition_preview(self, disk):
        disk_size = DiskManager.get_disk_size(disk)
        self.partition_preview.setRowCount(0)
        
        if disk_size < 50:
            self.size_warning.setText("Warning: Disk size under 50GB! Recommended minimum is 50GB.")
        else:
            self.size_warning.clear()

        # Add partitions
        partitions = [
            ("/boot/efi", "500M", "EFI System"),
            ("/", "30G", "Linux filesystem"),
            ("swap", "4G", "Linux swap"),
            ("/home", "Remaining", "Linux filesystem")
        ]
        
        for idx, (mount, size, ptype) in enumerate(partitions):
            self.partition_preview.insertRow(idx)
            self.partition_preview.setItem(idx, 0, QTableWidgetItem(mount))
            self.partition_preview.setItem(idx, 1, QTableWidgetItem(size))
            self.partition_preview.setItem(idx, 2, QTableWidgetItem(ptype))

    def launch_cfdisk(self):
        disk = self.disk_combo.currentData()
        if disk:
            subprocess.Popen(["sudo", "cfdisk", f"/dev/{disk}"])
            
    def launch_parted(self):
        disk = self.disk_combo.currentData()
        if disk:
            subprocess.Popen(["sudo", "parted", f"/dev/{disk}"])

    def validate_selection(self):
        disk = self.disk_combo.currentData()
        if not disk:
            QMessageBox.warning(self, "Error", "Please select a disk!")
            return
            
        if self.auto_radio.isChecked():
            if not self.validate_auto_partition(disk):
                return
            # Create partitions if auto selected
            DiskManager.create_auto_partitions(disk)
        else:
            if not self.validate_manual_partition(disk):
                return
                
        self.parent.selected_disk = disk
        self.parent.navigate_to(2)

    def validate_auto_partition(self, disk):
        disk_size = DiskManager.get_disk_size(disk)
        if disk_size < 50:
            reply = QMessageBox.question(
                self, "Warning",
                "Disk size is below 50GB. Continue anyway?",
                QMessageBox.Yes | QMessageBox.No
            )
            if reply != QMessageBox.Yes:
                return False
        return True

    def validate_manual_partition(self, disk):
        partitions = DiskManager.get_partitions(disk)
        has_root = any(p['mountpoint'] == '/' for p in partitions)
        has_boot = any(p['mountpoint'] == '/boot/efi' for p in partitions)
        
        if not has_root:
            QMessageBox.warning(self, "Error", "No root partition (/) found!")
            return False
        if not has_boot:
            QMessageBox.warning(self, "Error", "No EFI system partition found!")
            return False
        return True