
import serial
from serial.tools import list_ports
import time
import threading
import queue
import sys

# ------------Variables/Objects-----------------

# Loops will run across all threads when running is set.
running = threading.Event()
running.set()



# -------------Functions-------------

def main_print(msg, prompt=">"):
    '''
    Avoids jank when printing from the main thread while the input thread is running. 
    Should be used in place of print() anywhere the input thread is running.
    '''
    def print_thread_func():
        sys.stdout.write('\r')  # move cursor to beginning of line
        sys.stdout.write(' ' * 80)  # clear line
        sys.stdout.write('\r')  # move back again
        sys.stdout.write(msg + '\n') # Print message
        sys.stdout.write(prompt)  # re-print the prompt
        sys.stdout.flush()
    print_thread = threading.Thread(target=print_thread_func)
    print_thread.start()

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
    '''
    Sends a command string to the Arduino and prints any response.
    '''
    arduino.write(f'{cmd}\n'.encode())
    time.sleep(0.05)
    if arduino.in_waiting:
        response = arduino.readline().decode().strip()
        print(response)

def wait(duration):
    '''
    Creates a timer on a seperate thread to wait the proper time before the next stage.
    '''
    main_print(f'Waiting {duration} seconds before next stage...')
    def timer_thread_func():
        time.sleep(duration)
        main_print("Ready for next stage!")
    timer_thread = threading.Thread(target=timer_thread_func, daemon=True)
    timer_thread.start()

# ----------Commands--------------------- 

def cmd_test_arduino():
    arduino.write(b'test\n')
    time.sleep(0.05) # Wait for arduino to receive command
    in_value = input("Enter a number: ") # Taking input from user 
    out_value = write_read(in_value) 
    print(out_value) # printing the value  

def cmd_test_timer():
    wait(5)

def cmd_stop():
    '''
    Stops everything safely
    '''
    # Later: Add what we want emergency stop to do here (e.g. depressurize)
    try: 
        arduino.close() 
    except: 
        pass
    main_print("Program stopped. Press Enter to exit...")
    running.clear() # Break all loops across threads
    return # Once this function returns, the main loop ends therefore main thread ends.
    # Input thread hangs over (that's why there's an extra input before it exits), annoying but works
    
'''Open and close valves'''
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
    "test arduino": cmd_test_arduino,
    "test timer": cmd_test_timer,
    "automated": cmd_automated,
    "stop": cmd_stop,
    "open vacuum": cmd_open_vacuum,
    "close vacuum": cmd_close_vacuum,
    "open pressure": cmd_open_pressure,
    "close pressure": cmd_close_pressure,
    "open release": cmd_open_release,
    "close release": cmd_close_release,
}


# ---------Setup-------------

# Find the serial port with the arduino
port = find_arduino()
if port:
    # Define the arduino
    arduino = serial.Serial(port=port, baudrate=115200, timeout=1) # Same baudrate, timeout as in .ino
    # time.sleep(2)
    print("Found Arduino on", port)
else:
    print("Arduino not found.")
    cmd_stop()

# Handle inputs on a seperate thread
command_queue = queue.Queue()
def input_loop():
    '''
    Waits for commands and adds them to the queue for the main thread to execute
    '''
    while running.is_set(): 
        command = input('>')
        command_queue.put(command) # Queue is such a weirdly spelled word
        
input_thread = threading.Thread(target=input_loop)
input_thread.start()

# -----------Main Thread Loop----------------

while running.is_set():
    command = command_queue.get()
    cmd_func = commands.get(command)
    if cmd_func:
        cmd_func()
    else:
        print("Unknown command.")
