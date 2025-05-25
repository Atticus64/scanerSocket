
#from py122u import nfc

#reader = nfc.Reader()
#reader.connect()
#reader.connect()
#reader.print_data(reader.get_uid())
#reader.info()

import time
from py122u import nfc, error


def read_16(r, position, number):
    return r.read_binary_blocks(position, number)

def read(r, position, number):
    result = []
    while number >= 16:
        result.append(read_16(r, position, 16))
        number -= 16
        position += 1
    return result


def write(r, position, number, data):
    while number >= 16:
        write_16(r, position, 16, data)
        number -= 16
        position += 1


def write_16(r, position, number, data):
    r.update_binary_blocks(position, number, data)



def readCard() -> str:
    print("Listo para leer tarjetas NFC.")
    reader = nfc.Reader()

    while True:
        print("Esperando tarjeta NFC...")
        try:
            reader.connect()
            
            uid = reader.get_uid()

            #word = "0000000006"
            #data = list(word.encode('ascii'))
            #reader.load_authentication_data(0x01, [0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF])
            #reader.authentication(0x00, 0x61, 0x01)

            #elem = len(data)
            # agrega el padding de 16 de los faltantes
            #data += [0x00 for i in range(16 - elem)]
            
            #write(reader, 0x02, 0x10,[0x00 for i in range(16)])


            #print(read(reader, 0x06, 0x10))
            #info = read(reader, 0x06, 0x20)
            #todos = [n for bloque in info for n in bloque]
            #ascii_str = ''.join(chr(n) if 32 <= n <= 126 else '.' for n in todos)
            #word = ''.join(filter(str.isdigit, ascii_str))

            #print(f"Datos leídos: {ascii_str}")
            print(f"Tarjeta detectada: UID = {uid}")
            uid = ''.join(f'{b:02x}' for b in uid)
            return uid

        except (error.NoCommunication):
            time.sleep(0.5)

