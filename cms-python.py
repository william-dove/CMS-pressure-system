
import serial
from serial.tools import list_ports
import time

def find_arduino():
    '''
    Finds the serial port that the arduino is plugged into.
    '''
    ports = list_ports.comports()
    for p in ports:
        if "Arduino" in p.description or "USB-SERIAL" in p.description:
            return p.device
    return None

# Find the serial port with the arduino
port = find_arduino()
if port:
    # Define the arduino
    arduino = serial.Serial(port=port, baudrate=115200, timeout=1) # Same baudrate, timeout as in .ino
    time.sleep(2)
    print("Found Arduino on", port)
else:
    print("Arduino not found.")
    exit()

# TEST FUNCTION

def write_read(serial_test_var): 
    arduino.write(f'{serial_test_var}\n'.encode()) # Encoded string into utf-8 bytes
    time.sleep(0.05) 
    data = arduino.readline() 
    data_string = data.decode('utf-8').strip()  # Decode bytes to string and strip newline/carriage return
    return data_string 

while True: 

    command = input()

    if command == "test":
        arduino.write(b'test\n')
        time.sleep(0.05)
        in_value = input("Enter a number: ") # Taking input from user 
        out_value = write_read(in_value) 
        print(out_value) # printing the value 

