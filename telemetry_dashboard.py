import serial
import time
import sys

# ==========================================
# CONFIGURATION
# ==========================================
# Change this to your Bluetooth module's COM port (e.g., 'COM5' on Windows or '/dev/tty.HC-05' on Mac/Linux)
BLUETOOTH_PORT = 'COM3' 
BAUD_RATE = 9600

def connect_to_rover():
    print(f"[*] Attempting to connect to rover on {BLUETOOTH_PORT}...")
    try:
        rover_serial = serial.Serial(BLUETOOTH_PORT, BAUD_RATE, timeout=1)
        time.sleep(2) # Wait for connection to stabilize
        print("[+] Connection Established! Receiving Telemetry...\n")
        return rover_serial
    except Exception as e:
        print(f"[-] Connection Failed: {e}")
        print("    Ensure your HC-05 is paired and the COM port is correct.")
        sys.exit()

def start_dashboard(rover_serial):
    try:
        while True:
            if rover_serial.in_waiting > 0:
                raw_data = rover_serial.readline().decode('utf-8').strip()
                
                # Check if it's our formatted telemetry string
                if raw_data.startswith("DATA"):
                    parts = raw_data.split(',')
                    
                    if len(parts) == 5:
                        _, distance, left_ir, right_ir, action = parts
                        
                        # Clear terminal line and print live dashboard
                        sys.stdout.write(f"\r[ROVER STATE] | Distance: {distance}cm | L-IR: {left_ir} | R-IR: {right_ir} | Action: {action.ljust(18)}")
                        sys.stdout.flush()
                        
    except KeyboardInterrupt:
        print("\n\n[*] Dashboard Terminated by User.")
        rover_serial.close()

if __name__ == "__main__":
    serial_connection = connect_to_rover()
    start_dashboard(serial_connection)
  
