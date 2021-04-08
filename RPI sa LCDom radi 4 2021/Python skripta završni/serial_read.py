import drivers
import serial
from datetime import datetime
import time
from time import sleep


if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyUSB0',9600,timeout=10)
    ser.flush()

    display = drivers.Lcd() # Using lcd display
    now = datetime.now()
    # dd/mm/YY H:M:S
    dt_string = now.strftime("%d/%m/%Y %H:%M:%S")

    while True:                     
        display.lcd_clear()

        if ser.in_waiting > 0:
            #Getting current time
            now = datetime.now()
            # dd/mm/YY H:M:S
            dt_string = now.strftime("%d/%m/%Y %H:%M:%S")
            print(dt_string)
            line1 = ser.readline().decode('utf-8').rstrip()
            print(line1)
            line2 = ser.readline().decode('utf-8').rstrip()
            print(line2)
            display.lcd_display_string(line1, 1)
            display.lcd_display_string(line2, 2)
            sleep(5)
            print("\n") 
            
            


    
