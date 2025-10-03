
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
    print("Found Arduino on", port)
else:
    print("Arduino not found.")

# Define the arduino
arduino = serial.Serial(port=port, baudrate=115200, timeout=.1) # Same baudrate, timeout as in .ino


# TEST FUNCTION

def write_read(serial_test_var): 
    arduino.write(bytes(f'{serial_test_var}\n', 'utf-8')) # Added newline so arduino reads it
    time.sleep(0.05) 
    data = arduino.readline() 
    data_string = data.decode('utf-8').strip()  # Decode bytes to string and strip newline/carriage return
    return data_string 

while True: 
	in_value = input("Enter a number: ") # Taking input from user 
	out_value = write_read(in_value) 
	print(out_value) # printing the value 

