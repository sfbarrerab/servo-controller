import sys
import time
import serial
from serial.tools import list_ports

from PyQt5.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QLabel,
    QComboBox, QPushButton, QSlider, QHBoxLayout,
    QMessageBox, QRadioButton, QButtonGroup
)
from PyQt5.QtCore import Qt, QThread, pyqtSignal

WAITING_TIME = 5
MAX_SERVOS = 4


# ================= SERIAL THREAD ===================
class SerialWorker(QThread):
    response_received = pyqtSignal(str)
    error_signal = pyqtSignal(str)

    def __init__(self, port, command):
        super().__init__()
        self.port = port
        self.command = command

    def run(self):
        try:
            ser = serial.Serial(self.port, 9600, timeout=1)
            time.sleep(2)
            ser.reset_input_buffer()

            ser.write(self.command.encode())
            ser.flush()

            start_time = time.time()
            while time.time() - start_time < WAITING_TIME:
                if ser.in_waiting > 0:
                    response = ser.readline().decode(errors="ignore").strip()
                    ser.close()
                    self.response_received.emit(response)
                    return

            ser.close()
            self.error_signal.emit("Timeout: No response from Arduino.")

        except Exception as e:
            self.error_signal.emit(str(e))


# ================= COM SELECTOR ===================
class ComPortSelector(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Select COM Port")
        self.setGeometry(300, 300, 300, 150)

        layout = QVBoxLayout()
        self.combo = QComboBox()
        ports = list_ports.comports()
        if not ports:
            QMessageBox.critical(self, "Error", "No COM ports found.")
            sys.exit()

        for port in ports:
            self.combo.addItem(port.device)

        layout.addWidget(QLabel("Select COM Port"))
        layout.addWidget(self.combo)

        btn = QPushButton("Connect")
        btn.clicked.connect(self.open_servo_window)
        layout.addWidget(btn)

        self.setLayout(layout)

    def open_servo_window(self):
        port = self.combo.currentText()
        if not port:
            QMessageBox.warning(self, "Error", "No port selected.")
            return
        self.servo_window = ServoControlWindow(port)
        self.servo_window.show()
        self.close()


# ================ SERVO CONTROL ==================
class ServoControlWindow(QWidget):
    def __init__(self, port):
        super().__init__()
        self.port = port
        self.worker = None

        self.setWindowTitle("Servo Controller")
        self.setGeometry(300, 300, 400, 350)

        layout = QVBoxLayout()

        # === Servo selection (exclusive) ===
        layout.addWidget(QLabel("Select Servo to Modify"))

        self.servo_group = QButtonGroup()
        self.servo_group.setExclusive(True)
        self.radio_buttons = []
        for i in range(MAX_SERVOS):
            rb = QRadioButton(f"Servo {i}")
            if i == 0:
                rb.setChecked(True)
            self.radio_buttons.append(rb)
            self.servo_group.addButton(rb, i)
            layout.addWidget(rb)

        # === Editable sliders ===
        layout.addWidget(QLabel("Editable Values"))

        self.init_label = QLabel("Initial Position (New): 0")
        layout.addWidget(self.init_label)
        self.init_slider = QSlider(Qt.Horizontal)
        self.init_slider.setRange(0, 180)
        self.init_slider.valueChanged.connect(
            lambda v: self.init_label.setText(f"Initial Position (New): {v}")
        )
        layout.addWidget(self.init_slider)

        self.final_label = QLabel("Final Position (New): 0")
        layout.addWidget(self.final_label)
        self.final_slider = QSlider(Qt.Horizontal)
        self.final_slider.setRange(0, 180)
        self.final_slider.valueChanged.connect(
            lambda v: self.final_label.setText(f"Final Position (New): {v}")
        )
        layout.addWidget(self.final_slider)

        # === Stored values ===
        layout.addWidget(QLabel("Stored Values in Arduino"))
        self.stored_init_label = QLabel("Stored Initial Position: --")
        layout.addWidget(self.stored_init_label)
        self.stored_final_label = QLabel("Stored Final Position: --")
        layout.addWidget(self.stored_final_label)

        # === Buttons ===
        btn_layout = QHBoxLayout()
        upload_btn = QPushButton("Upload")
        upload_btn.clicked.connect(self.upload_values)
        btn_layout.addWidget(upload_btn)

        verify_btn = QPushButton("Verify")
        verify_btn.clicked.connect(self.verify_values)
        btn_layout.addWidget(verify_btn)

        layout.addLayout(btn_layout)
        self.setLayout(layout)

    # === Upload ===
    def upload_values(self):
        servoId = self.servo_group.checkedId()
        init_val = self.init_slider.value()
        final_val = self.final_slider.value()
        command = f"SET,{servoId},{init_val},{final_val}\n"
        self.start_worker(command)

    # === Verify ===
    def verify_values(self):
        servoId = self.servo_group.checkedId()
        command = f"GET,{servoId}\n"
        self.start_worker(command)

    # === Worker thread ===
    def start_worker(self, command):
        self.worker = SerialWorker(self.port, command)
        self.worker.response_received.connect(self.handle_response)
        self.worker.error_signal.connect(self.handle_error)
        self.worker.start()

    # === Response handling ===
    def handle_response(self, response):
        if response.strip() == "OK":
            QMessageBox.information(self, "Success", "Values stored successfully.")
            return

        try:
            parts = list(map(int, response.strip().split(",")))
            if len(parts) == 3:
                servoId, init_val, final_val = parts
                self.stored_init_label.setText(f"Stored Initial Position: {init_val}")
                self.stored_final_label.setText(f"Stored Final Position: {final_val}")
                QMessageBox.information(self, "Success", f"Values verified for Servo {servoId}.")
            else:
                QMessageBox.warning(self, "Invalid Response", response)
        except:
            QMessageBox.warning(self, "Invalid Response", response)

    # === Error handling ===
    def handle_error(self, message):
        QMessageBox.critical(self, "Communication Error", message)


# ================= MAIN ==========================
if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = ComPortSelector()
    window.show()
    sys.exit(app.exec_())