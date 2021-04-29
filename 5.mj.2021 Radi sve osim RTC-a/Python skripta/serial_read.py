import mysql.connector  #Import of mysql
import drivers
import serial
from datetime import datetime
import time
from time import sleep


db = mysql.connector.connect(host="localhost",user="root",passwd="mojmobjekul",db="iotweatherstation")
cursor = db.cursor()

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
            dt_string = now.strftime("%Y-%m-%d %H:%M:%S") #Vrijeme radi popraviti temperaturu i vlagu
            print(dt_string)

            #Pokusaj splitanja readlinea u vise stringova
            line1 = ser.readline().decode('utf-8').rstrip()
            #print(line1)
            str_int1 = str(line1)
            t1,h1 = str_int1[:2],str_int1[2:]
            print(t1)
            print(h1)
            #Splitanja readlinea sa Raspberry Pi-ja
            line2 = ser.readline().decode('utf-8').rstrip()
            str_int2 = str(line2)
            t2,h2 = str_int2[:2],str_int2[2:]
            print("RPI: T:" +t2+ " H:"+ h2)
            display.lcd_display_string('Arduino:', 1)
            display.lcd_display_string('T:'+ t1 + ' H:' + h1 , 2)
            sleep(5)
            display.lcd_clear()
            display.lcd_display_string('RPI:', 1)
            display.lcd_display_string('T:'+ t2 + ' H:' + h2 , 2)
            sleep(5)

            #SQL insert part
            arduinoInsert = ("INSERT INTO TempAndHumLog"
                          "(timestamp,temperature,humidity,sensor)"
                          "VALUES (%s,%s,%s,%s)"
                          )
            arduinoData = (dt_string,t1,h1,'Arduino')
            cursor.execute(arduinoInsert,arduinoData)
            db.commit()
            sleep(5)

            rpiInsert = ("INSERT INTO TempAndHumLog"
                          "(timestamp,temperature,humidity,sensor)"
                          "VALUES (%s,%s,%s,%s)"
                          )
            rpiData = (dt_string,t2,h2,'RPI')
            cursor.execute(rpiInsert,rpiData)
            db.commit()
            sleep(5)           
            
            
            

            
            


    
