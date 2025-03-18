from PyQt5.QtWidgets import QMessageBox

def show_error(message, parent=None):
    QMessageBox.critical(parent, "Error", message)

def run_command(cmd, sudo=False):
    try:
        if sudo:
            cmd = ["sudo"] + cmd
        result = subprocess.run(
            cmd,
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT
        )
        return result.stdout.decode()
    except subprocess.CalledProcessError as e:
        show_error(f"Command failed: {e}\nOutput: {e.output.decode()}")
        return None
