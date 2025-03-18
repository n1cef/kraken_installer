import subprocess
from PyQt5.QtCore import QObject, pyqtSignal
import subprocess
import json

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
    

    def list_disks():
        """Return list of disks in format: [{'name': 'sda', 'size': '500G'}, ...]"""
        cmd = "lsblk -d -o NAME,SIZE --json"
        result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
        disks = json.loads(result.stdout)['blockdevices']
        return [{'name': d['name'], 'size': d['size']} for d in disks]

    def get_partitions(disk):
        """Return partitions for specified disk"""
        cmd = f"lsblk -o NAME,SIZE,TYPE,MOUNTPOINT /dev/{disk} --json"
        result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
        data = json.loads(result.stdout)
        return [
            {
            'device': p['name'],
            'size': p['size'],
            'type': p['type'],
            'mountpoint': p.get('mountpoint', '')
            }
            for p in data['blockdevices'][0]['children']
    ]