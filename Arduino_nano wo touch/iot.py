import serial
import pyautogui
import time

# Configure serial port (replace 'COM3' with your Arduino's port)
ser = serial.Serial('COM8', 9600, timeout=1)
time.sleep(2)  # Wait for serial connection to establish

# Set pyautogui settings
pyautogui.FAILSAFE = True  # Move mouse to upper-left corner to abort
pyautogui.PAUSE = 0.01    # Small delay between actions

try:
    while True:
        if ser.in_waiting > 0:
            # Read and parse serial data
            line = ser.readline().decode('utf-8').strip()
            # Check if the line matches the expected format (e.g., "x,y")
            if ',' in line and line.count(',') == 1:
                try:
                    mouse_x, mouse_y = map(int, line.split(','))
                    
                    # Move mouse relative to current position
                    pyautogui.moveRel(mouse_x, mouse_y)
                    
                except ValueError:
                    print(f"Invalid data received: {line}")
            else:
                print(f"Ignored non-numeric data: {line}")
except KeyboardInterrupt:
    print("Program terminated")
finally:
    ser.close()