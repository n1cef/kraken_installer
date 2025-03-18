import subprocess
import json
from PyQt5.QtCore import QObject, pyqtSignal

class DiskManager(QObject):
    disks_updated = pyqtSignal(list)
    
    @staticmethod
    def get_disk_size(disk):
        """Get disk size in gigabytes"""
        try:
            cmd = f"lsblk -b -dn -o SIZE /dev/{disk}"
            result = subprocess.run(cmd, shell=True, check=True, capture_output=True, text=True)
            return int(result.stdout.strip()) // (1024 ** 3)
        except subprocess.CalledProcessError as e:
            print(f"Error getting disk size: {e}")
            return 0

    @staticmethod
    def list_disks():
        """List all available disks with model information"""
        try:
            cmd = "lsblk -d -o NAME,SIZE,MODEL --json"
            result = subprocess.run(cmd, shell=True, check=True, capture_output=True, text=True)
            data = json.loads(result.stdout)
            return [{
                'name': disk['name'],
                'size': disk['size'],
                'model': disk.get('model', 'Unknown')
            } for disk in data['blockdevices']]
        except Exception as e:
            print(f"Error listing disks: {e}")
            return []

    @staticmethod
    def get_partitions(disk):
        """Get partitions for a specific disk"""
        try:
            cmd = f"lsblk -o NAME,SIZE,TYPE,MOUNTPOINT /dev/{disk} --json"
            result = subprocess.run(cmd, shell=True, check=True, capture_output=True, text=True)
            data = json.loads(result.stdout)
            return [{
                'name': part['name'],
                'size': part['size'],
                'type': part['type'],
                'mountpoint': part.get('mountpoint', '')
            } for part in data['blockdevices'][0].get('children', [])]
        except Exception as e:
            print(f"Error getting partitions: {e}")
            return []

    @staticmethod
    def create_auto_partitions(disk):
        """Create automatic partition scheme"""
        try:
            # Create partition table
            subprocess.run([
                'sudo', 'parted', f'/dev/{disk}', '--script', 'mklabel', 'gpt'
            ], check=True)

            # Create partitions
            subprocess.run([
                'sudo', 'parted', f'/dev/{disk}', '--script',
                'mkpart', 'primary', 'fat32', '1MiB', '501MiB'
            ], check=True)
            subprocess.run([
                'sudo', 'parted', f'/dev/{disk}', '--script', 'set', '1', 'esp', 'on'
            ], check=True)

            subprocess.run([
                'sudo', 'parted', f'/dev/{disk}', '--script',
                'mkpart', 'primary', 'ext4', '501MiB', '30.5GiB'
            ], check=True)

            subprocess.run([
                'sudo', 'parted', f'/dev/{disk}', '--script',
                'mkpart', 'primary', 'linux-swap', '30.5GiB', '34.5GiB'
            ], check=True)

            subprocess.run([
                'sudo', 'parted', f'/dev/{disk}', '--script',
                'mkpart', 'primary', 'ext4', '34.5GiB', '100%'
            ], check=True)

            # Format partitions
            subprocess.run(['sudo', 'mkfs.fat', '-F32', f'/dev/{disk}1'], check=True)
            subprocess.run(['sudo', 'mkfs.ext4', f'/dev/{disk}2'], check=True)
            subprocess.run(['sudo', 'mkswap', f'/dev/{disk}3'], check=True)
            subprocess.run(['sudo', 'mkfs.ext4', f'/dev/{disk}4'], check=True)
            subprocess.run(['sudo', 'swapon', f'/dev/{disk}3'], check=True)

            return True
        except subprocess.CalledProcessError as e:
            print(f"Partitioning failed: {e}")
            return False