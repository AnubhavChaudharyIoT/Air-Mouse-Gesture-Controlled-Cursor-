import time

import pyautogui
import serial

# --- IMPORTANT: Change 'COM5' to your Arduino's serial port ---
ser = serial.Serial("COM5", 9600, timeout=1)
time.sleep(2)

pyautogui.FAILSAFE = False
pyautogui.PAUSE = 0.01

try:
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode(encoding="utf-8").strip()
            if line.count(",") == 2:  # expecting "x,y,touch"
                try:
                    mouse_x, mouse_y, touch = map(int, line.split(","))

                    # Move mouse relative. The signs are adjusted in the Arduino code.
                    pyautogui.moveRel(mouse_x, mouse_y)

                    if touch == 1:
                        pyautogui.click()
                except ValueError:
                    print(f"Invalid data: {line}")
            else:
                print(f"Ignored: {line}")

except KeyboardInterrupt:
    print("Program terminated")
finally:
    ser.close()
