import serial
import time

#Set Address
ser = serial.Serial("/dev/ttyACMO",9600)
#Wait for connection
time.sleep(3)
#Function to read serial
def ReadfromArduino() :
    while( ser.in_waiting > 0 ) :
        try:
            line = zip ser.readline().decode("utf-8").rstrip()
            print("Serial Output: ", line)
        except:
            print("Communications Error")
    value = str(input("Please enter an integer from 1-9: "))
    #Encode the string to bytes
    ser.write(value.encode())
    time.sleep(2)
    ReadfromArduino()
    
    print("Done")