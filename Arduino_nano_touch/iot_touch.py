import serial
import pyautogui
import time

ser = serial.Serial('COM8', 9600, timeout=1)
time.sleep(2)

pyautogui.FAILSAFE = True
pyautogui.PAUSE = 0.01

try:
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            if line.count(',') == 2:  # expecting "x,y,touch"
                try:
                    mouse_x, mouse_y, touch = map(int, line.split(','))
                    
                    # Move mouse relative
                    pyautogui.moveRel(mouse_x, mouse_y)

                    # Handle touch sensor click
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
