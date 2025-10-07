
import serial
from serial.tools import list_ports
import time

# ----------Functions-------------

def find_arduino():
    '''
    Finds the serial port that the arduino is plugged into.
    '''
    ports = list_ports.comports()
    for p in ports:
        if "Arduino" in p.description or "USB-SERIAL" in p.description:
            return p.device
    return None

def write_read(serial_test_var):
    '''
    A test function for the test command
    ''' 
    arduino.write(f'{serial_test_var}\n'.encode()) # Encoded string into utf-8 bytes
    time.sleep(0.05) 
    data = arduino.readline() 
    data_string = data.decode().strip()
    return data_string 

def send_command(cmd):
    """
    Sends a command string to the Arduino and prints any response.
    """
    arduino.write(f'{cmd}\n'.encode())
    time.sleep(0.05)
    if arduino.in_waiting:
        response = arduino.readline().decode().strip()
        print(response)

# ----------Commands--------------------- 

def cmd_test():
    arduino.write(b'test\n')
    time.sleep(0.05) # Wait for arduino to receive command
    in_value = input("Enter a number: ") # Taking input from user 
    out_value = write_read(in_value) 
    print(out_value) # printing the value     
def cmd_stop():
    exit()
def cmd_automated():
    send_command('automated')
def cmd_open_vacuum():
    send_command("open vacuum")
def cmd_close_vacuum():
    send_command("close vacuum")
def cmd_open_pressure():
    send_command("open pressure")
def cmd_close_pressure():
    send_command("close pressure")
def cmd_open_release():
    send_command("open release")
def cmd_close_release():
    send_command("close release")

# Command name dictionary

commands = {
    "test": cmd_test,
    "automated": cmd_automated,
    "stop": cmd_stop,
    "open vacuum": cmd_open_vacuum,
    "close vacuum": cmd_close_vacuum,
    "open pressure": cmd_open_pressure,
    "close pressure": cmd_close_pressure,
    "open release": cmd_open_release,
    "close release": cmd_close_release,
}

# ---------MAIN CODE-------------

# Find the serial port with the arduino
port = find_arduino()
if port:
    # Define the arduino
    arduino = serial.Serial(port=port, baudrate=115200, timeout=1) # Same baudrate, timeout as in .ino
    # time.sleep(2)
    print("Found Arduino on", port)
else:
    print("Arduino not found.")
    input("Press Enter to exit...")
    cmd_stop()

# Loop
while True: 
    command = input('>')
    cmd_func = commands.get(command)
    if cmd_func:
        cmd_func()
    else:
        print("Unknown command.")


