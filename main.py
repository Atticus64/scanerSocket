import serial
import time
import requests

# Configura el puerto serial

def write_card(data, ser):
    payload = f'w:{data}'
    ser.write(payload.encode('utf-8'))
    print(f"Enviando al Arduino: {payload}")

    time.sleep(2)  # Espera un poco para que el Arduino procese la solicitud
    if ser.in_waiting > 0:
        line = ser.readline().decode().strip()
        print(line)
        if line.startswith("OK"):
            print("Escritura completada.")
        else:
            print("Error al escribir en la tarjeta.")
    
def wait_read():
    #puerto = 'COM6'  
    puerto = 'COM5'  
    baud_rate = 9600

    # Conecta con el Arduino
    ser = serial.Serial(puerto, baud_rate, timeout=3)
    readed = False
    body = {}
    while not readed:
        print("Esperando una tarjeta...")
        (uid, data) = read_card(ser)

        body = {
            'id': uid,
            'data': data if data else None
        }    
        if uid:
            #print(f"UID de la tarjeta: {uid}")
            print(body)
            readed = True
            #requests.post('http://localhost:5000/data', json=body)
        else:
            print("No se detectó ninguna tarjeta.")
        time.sleep(1)  # Vuelve a intentar después de un segundo

    return body
     
       
def read_card(ser):
    # Envia el comando 'r' al Arduino para leer la tarjeta
    line = ""

    #ser.write(b'r')
    while not line.startswith("UID:"):
        ser.write(b'r')
        time.sleep(0.1)  

        if ser.in_waiting > 0:
            line = ser.readline().decode().strip()
            time.sleep(0.5)
            uid = ""
            if line.startswith("UID:"):
            #    print("UID detectado:", line[4:])
                buffer = line[4:]
                uid = buffer.split(";")[0]
                #data = buffer.split(";")[1]
                data = None
                return uid, data
