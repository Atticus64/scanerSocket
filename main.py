import serial
import time
import requests

# Configura el puerto serial
puerto = 'COM5'  
baud_rate = 9600

# Conecta con el Arduino
ser = serial.Serial(puerto, baud_rate, timeout=1)

def read_card():
    # Envia el comando 'r' al Arduino para leer la tarjeta
    ser.write(b'r')
    time.sleep(2)  # Espera un poco para que el Arduino procese la solicitud

    # Lee la respuesta del Arduino
    if ser.in_waiting > 0:
        line = ser.readline().decode().strip()
        uid = ""
        data = ""
        if line.startswith("UID:"):
        #    print("UID detectado:", line[4:])
            uid = line[4:]
        elif line.startswith("DATA:"):
        #    print("Datos del bloque:", line[5:])
            data = line[5:]

        return uid, data
    else:
        return None, None

def write_card(data):
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
    

# Ejecuta la función de lectura
while True:
    print("Esperando una tarjeta...")
    (uid, data) = read_card()

    body = {
        'id': uid,
        'card': True
    }    
    if uid:
        #print(f"UID de la tarjeta: {uid}")
        print(body)

        requests.post('http://localhost:5000/data', json=body)
        
        if data:
            print(f"Datos de la tarjeta: {data}")
        #else:
            #print("No se encontraron datos en la tarjeta.")
            #write_card(info)
            #write_card("b:00001")
        #print("Escribiendo en la tarjeta...")
        #print("Escritura completada.")
    else:
        print("No se detectó ninguna tarjeta.")
    time.sleep(1)  # Vuelve a intentar después de un segundo
