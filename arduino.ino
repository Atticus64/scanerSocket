
#include <SPI.h>			// incluye libreria bus SPI
#include <MFRC522.h>			// incluye libreria especifica para MFRC522
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define RST_PIN  9			// constante para referenciar pin de reset
#define SS_PIN  10			// constante para referenciar pin de slave select
// #define SS_PIN 5
// #define RST_PIN 0
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

const int buzzerPin = 8;
const int botonPin = 2;

MFRC522 mfrc522(SS_PIN, RST_PIN);	// crea objeto mfrc522 enviando pines de slave select y reset
Adafruit_SSD1306 
display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);			// inicializa comunicacion por monitor serie a 9600 bps
  // Serial.begin(115200);
  SPI.begin();				// inicializa bus SPI
  mfrc522.PCD_Init();	// inicializa modulo lector
  pinMode(buzzerPin, OUTPUT);
  pinMode(botonPin, INPUT_PULLUP);

  Serial.println("\nIniciado correctamente...");
  
//   if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
//   Serial.println(F("SSD1306 allocation failed"));
//   return;
// }

  // Serial.println("Pantalla inicializada correctamente");

  // display.clearDisplay();
  // display.setTextSize(1);
  // display.setTextColor(SSD1306_WHITE);
  // display.setCursor(0, 0);
  // display.println("-Roman-");
  // display.display();

}

void loop() {
  //  Serial.println("si jala!");
   if (Serial.available()) {
    char comando = Serial.read();
     
    if (comando == 'r') {
      // if (detectarTarjeta()) {
        // leerBloque4();
      // }

  
      if (!mfrc522.PICC_IsNewCardPresent()) {
        return;
      }
      
      if (!mfrc522.PICC_ReadCardSerial()) {
        return;
      }	
     
      String uid = "";				// muestra texto UID:
      for (byte i = 0; i < mfrc522.uid.size; i++) {	// bucle recorre de a un byte por vez el UID
        uid += String(mfrc522.uid.uidByte[i], HEX);
      }
      Serial.print("UID:"); 
      Serial.println(uid);
      // String data = leerBloque4();
      // Serial.println(data);
     
  
      tone(buzzerPin, 1000);
      delay(100);
      noTone(buzzerPin);
    } else if (comando == 'w') {
      String data = Serial.readStringUntil(';').substring(1);
      MFRC522::PICC_Type tipo = mfrc522.PICC_GetType(mfrc522.uid.sak);
      String nombre_tipo = mfrc522.PICC_GetTypeName(tipo);
      Serial.println(nombre_tipo);
    
      // escribirBloque4(data);
      escribirPagina4(data);
    }
  }

  // if ( ! mfrc522.PICC_IsNewCardPresent())	// si no hay una tarjeta presente
  //   return;					// retorna al loop esperando por una tarjeta
  
  // if ( ! mfrc522.PICC_ReadCardSerial()) 	// si no puede obtener datos de la tarjeta
  //   return;					// retorna al loop esperando por otra tarjeta
    

  // char cmd = Serial.read();
  // // Serial.print(cmd);
  // // if (cmd == 'r') {

  //   Serial.print("UID:");
  //   String uid = "";				// muestra texto UID:
  //   for (byte i = 0; i < mfrc522.uid.size; i++) {	// bucle recorre de a un byte por vez el UID
  //     if (mfrc522.uid.uidByte[i] < 0x10){		// si el byte leido es menor a 0x10
  //       Serial.print(" 0");			// imprime espacio en blanco y numero cero
  //       }
  //       else{					// sino
  //       Serial.print(" ");			// imprime un espacio en blanco
  //       }
  //     uid += String(mfrc522.uid.uidByte[i], HEX);
  //     Serial.print(mfrc522.uid.uidByte[i], HEX); 	// imprime el byte del UID leido en hexadecimal  
  //   } 
  //   Serial.println();				// nueva linea
  //   Serial.println(uid);
  // // }


  // mfrc522.PICC_HaltA();                 	// detiene comunicacion con tarjeta
}


bool detectarTarjeta() {
  Serial.println("Esperando tarjeta...");
  while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
  }
  Serial.print("UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  return true;
}

String leerBloque4() {
  MFRC522::MIFARE_Key key;

  String data = "";
  for (int i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  byte buffer[18];
  byte size = sizeof(buffer);
  byte block = 4;
  if (mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)) != MFRC522::STATUS_OK) {
    // Serial.println("Error de autenticación.");
    // return;
  }

  MFRC522::StatusCode status = mfrc522.MIFARE_Read(block, buffer, &size);
  if (status == MFRC522::STATUS_OK) {
    for (int i = 0; i < 16; i++) {
      data += (char)buffer[i];
    }
  } else {
    Serial.println("Error al leer el bloque.");
    Serial.println(mfrc522.GetStatusCodeName(status));
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  return data;
}

// void escribirBloque4(String texto) {
//   Serial.println("Intentando escribir...");

//   if (!mfrc522.PICC_IsNewCardPresent()) {
//     Serial.println("No se detecta tarjeta al intentar escribir.");
//     return;
//   }

//   if (!mfrc522.PICC_ReadCardSerial()) {
//     Serial.println("Error al leer el serial de la tarjeta al intentar escribir.");
//     return;
//   }

//   Serial.print("UID de la tarjeta al intentar escribir: ");
//   for (byte i = 0; i < mfrc522.uid.size; i++) {
//     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
//     Serial.print(mfrc522.uid.uidByte[i], HEX);
//   }
//   Serial.println();

//   MFRC522::MIFARE_Key key;
//   for (int i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

//   byte dataBlock[16] = {0};
//   byte block = 4;

//   for (int i = 0; i < texto.length() && i < 16; i++) {
//     dataBlock[i] = texto[i];
//   }

//   // if (mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)) != MFRC522::STATUS_OK) {
//   //   // Serial.println("Fallo autenticación.");
//   //   // mfrc522.PICC_HaltA();
//   //   // mfrc522.PCD_StopCrypto1();
//   //   // return;
//   // }

//   if (mfrc522.MIFARE_Write(block, dataBlock, 16) == MFRC522::STATUS_OK) {
//     Serial.println("OK");
//   } else {
//     Serial.println("Error al escribir.");
//   }

//   mfrc522.PICC_HaltA();
//   mfrc522.PCD_StopCrypto1();
// }

bool autenticarBloque(byte block) {
  // Prepara la clave - clave por defecto FF FF FF FF FF FF
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  
  // Muestra información de depuración
  Serial.print("Intentando autenticar bloque ");
  Serial.println(block);
    
  // Muestra información de la tarjeta para depuración
  Serial.print("UID de tarjeta: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  
  // Intenta autenticar con clave A
  Serial.println("Autenticando con clave A...");
  MFRC522::StatusCode status = mfrc522.PCD_Authenticate(
    MFRC522::PICC_CMD_MF_AUTH_KEY_A, 
    block, 
    &key, 
    &(mfrc522.uid)
  );
  
  // Manejo detallado de errores
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Fallo en autenticación. Código de error: ");
    Serial.print(status);
    Serial.print(" (");
    Serial.print(mfrc522.GetStatusCodeName(status));
    Serial.println(")");
    
    if (status == MFRC522::STATUS_TIMEOUT) {
      Serial.println("  - La comunicación con la tarjeta falló por timeout");
      Serial.println("  - Asegúrate de que la tarjeta esté bien posicionada");
    }
    else if (status == MFRC522::STATUS_ERROR) {
      Serial.println("  - Error de comunicación general");
      Serial.println("  - Verifica conexiones físicas del módulo RC522");
    }
    else if (status == MFRC522::STATUS_COLLISION) {
      Serial.println("  - Se detectó colisión, posiblemente hay más de una tarjeta");
    }
    else if (status == MFRC522::STATUS_MIFARE_NACK) {
      Serial.println("  - La tarjeta rechazó el comando o la clave es incorrecta");
      Serial.println("  - Verifica que estés usando la clave correcta para este sector");
      Serial.println("  - Si la tarjeta fue escrita previamente, puede tener otra clave");
    }
    

    
    return false;
  }
 


  Serial.println("Autenticación exitosa");
  return true;
}

// void escribirBloque4(String texto) {
//   while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
//     delay(50);
//   }

//   MFRC522::MIFARE_Key key;
//   for (int i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

//   byte dataBlock[16] = {0};
//   byte block = 4;

//   if (!autenticarBloque(block)) {
//     return false;  // La función de autenticación ya maneja los errores
//   }

//   for (int i = 0; i < texto.length() && i < 16; i++) {
//     dataBlock[i] = texto[i];
//   }

//   delay(50); 

//   if (mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)) != MFRC522::STATUS_OK) {
//     Serial.println("Fallo autenticación.");
//     return;
//   }

//   if (mfrc522.MIFARE_Write(block, dataBlock, 16) == MFRC522::STATUS_OK) {
//     Serial.println("OK");
//   } else {
//     Serial.println("Error al escribir.");
//   }

//   mfrc522.PICC_HaltA();
//   mfrc522.PCD_StopCrypto1();
// }

bool escribirBloque4(String texto) {
  Serial.println("Intentando escribir...");
  while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
  }
  byte block = 4;
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

 
  // if (!autenticarBloque(block)) {
  //   Serial.println("Falló la autenticación, no se puede escribir.");
  //   return false;
  // }

  byte dataBlock[16] = {0};
  for (int i = 0; i < texto.length() && i < 16; i++) {
    dataBlock[i] = texto[i];
  }
  // for (int i = 0; i < texto.length() && i < 16; i++) {
  //   dataBlock[i] = texto[i];
  // }

  // delay(50);
  if (mfrc522.MIFARE_Ultralight_Write(block, dataBlock, 16) == MFRC522::STATUS_OK) {
    Serial.println("Escritura OK");
    return true;
  } else {
    Serial.println("Error al escribir en el bloque.");
    // mfrc522.PICC_HaltA();
    // mfrc522.PCD_StopCrypto1();
    return false;
  }
}

bool escribirPagina4(String texto) {
  Serial.println("Intentando escribir a página 4...");

  // Esperar tarjeta
  while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
  }

  // Detener cualquier cifrado activo (por si quedó activo por otro tipo de tarjeta)
  mfrc522.PCD_StopCrypto1();

  // Página 4: primeras de usuario en NTAG213
  byte pagina = 4;
  byte dataBlock[4] = {0};

  // Copiar máximo 4 caracteres al bloque
  for (int i = 0; i < texto.length() && i < 4; i++) {
    dataBlock[i] = texto[i];
  }

  // Usar MIFARE_Ultralight_Write (solo para 4 bytes)
  MFRC522::StatusCode status = mfrc522.MIFARE_Ultralight_Write(pagina, dataBlock, 4);

  if (status == MFRC522::STATUS_OK) {
    Serial.println("✅ Escritura OK en página 4");
    return true;
  } else {
    Serial.print("❌ Error al escribir en página 4: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
}
