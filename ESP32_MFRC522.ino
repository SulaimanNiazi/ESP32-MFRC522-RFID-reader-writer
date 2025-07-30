#include <SPI.h>
#include <MFRC522.h>

#define SCK_PIN 18
#define MISO_PIN 19
#define MOSI_PIN 23
#define RST_PIN 2
#define Indicator_PIN 3
#define SS_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);                             // Create MFRC522 instance

void setup() {
  Serial.begin(115200);
  while(!Serial);                                             // Do nothing if there is no UART connection

  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);             // Init SPI bus
  pinMode(Indicator_PIN, OUTPUT);
  digitalWrite(Indicator_PIN, LOW);
  mfrc522.PCD_Init();                                         // Init MFRC522 card

  Serial.write('\n');
}

void loop(){
  Serial.print("**Select Mode**\n1. Change UID Mode\n2. Read Block Mode\n3. Write Block Mode\nChoice: ");
  
  byte buffer[20] = "", len = 0, choice = readUART()[0];
  MFRC522::StatusCode status;
  String string = "";
  MFRC522::MIFARE_Key key;
  
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;         // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.

  switch(choice){
    case '1':
      Serial.println("Change UID Mode Selected\nEnter new UID value: ");
      string = readUART();
      
      while(1){
        awaitCardDetection();

      }
      break;
    case '2':
      Serial.println("Read Block Mode Selected");
      while(1){
        Serial.print("Enter Block Number: ");
        choice = readUART().toInt();
        if(3 < choice && choice < 64){
          Serial.print("Block ");
          Serial.print(choice);
          Serial.println(" Selected.");
          break;
        }else{
          Serial.println("Enter a block value in the range of 4 to 63.");
        }
      }
      while(1){
        Serial.print("Enter the number of characters to read: ");
        len = readUART().toInt();
        if(0 < len && len < 17){
          Serial.print(len);
          Serial.println(" characters will be read.");
          break;
        }else{
          Serial.println("Enter a value in the range of 0 to 16.");
        }
      }
      Serial.println("Bring a card near to read.");
      
      while(1){
        awaitCardDetection();
        
        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, choice, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Authentication failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
        }else{
          byte temp = 20;
          status = mfrc522.MIFARE_Read(choice, buffer, &temp);
          
          if (status != MFRC522::STATUS_OK) {
            Serial.print(F("Reading failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
          }else{
            for (uint8_t i = 0; i < len; i++){
              Serial.write(buffer[i]);
            }
            Serial.println(" ");
          }
        }
      }
      break;
    case '3':
      Serial.println("Write Block Mode Selected.");
      while(1){
        Serial.print("Enter Block Number: ");
        choice = readUART().toInt();
        if(3 < choice && choice < 64){
          Serial.print("Block ");
          Serial.print(choice);
          Serial.println(" Selected.");
          break;
        }else{
          Serial.println("Enter a block value in the range of 4 to 63.");
        }
      }
      Serial.println("Enter block data:");
      string = readUART();
      string.getBytes(buffer, 16);
      for (int i = string.length(); i < 16; i++) buffer[i] = ' '; // pad with spaces
      Serial.println("Block data saved.\nBring a card near to write.");
      
      while(1){
        awaitCardDetection();
        
        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, choice, &key, &(mfrc522.uid));
        
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("PCD_Authenticate() failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
        }else{
          status = mfrc522.MIFARE_Write(choice, buffer, 16);
          
          if (status != MFRC522::STATUS_OK) {
            Serial.print(F("MIFARE_Write() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
          }
          else Serial.println(F("MIFARE_Write() success: "));
        } 
      }
      break;
    default:
      Serial.print("Please Enter a valid choice\nYou entered: ");
      switch(choice){
        case '\n':
          Serial.print("\\n");
          break;
        case '\r':
          Serial.print("\\r");
          break;
        case '\t':
          Serial.print("\\t");
          break;
        default:
          Serial.write(choice);
      }
      Serial.println();
  }
}

String readUART() {
  String input = "";
  while(1){
    if (Serial.available() > 0) {                         // Wait for the UART recieve buffer to get a byte
      byte inByte = Serial.read();                        // Read the byte from UART
      if ((inByte == 10)||(inByte == 0x0D)) {                                 // Line Feed (LF)
        Serial.println();
        break;
      }
      if (inByte == 8) {                                  // Backspace (BS)
        if (input.length() > 0) {
          input.remove(input.length() - 1);               // Remove last character
          Serial.print("\b \b");                          // Move the cursor back, print a space, and move it back again
        }
      } else {
        input += (char)inByte;                            // Append the received character to the input string
        Serial.print((char)inByte);                       // Print the character
      }
    }
  }  
  return input;                                           // Return the collected input
}

void awaitCardDetection(){
  mfrc522.PICC_HaltA();                                   // Halt the previous card, ending the communication session
  mfrc522.PCD_StopCrypto1();                              // Stop the encryption on the reader, resetting its state
  digitalWrite(Indicator_PIN, LOW);
  
  while(1){
    if (mfrc522.PICC_IsNewCardPresent()){            // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle. (Skipped)
      if (mfrc522.PICC_ReadCardSerial()){                 // Select one of the cards
        Serial.println(F("**Card Detected:**"));
        mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); // dump some identification details about the card
        MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
        if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
          Serial.println("This project only works with MIFARE Classic cards and ISO 14443A tags.");
        }else{
          digitalWrite(Indicator_PIN, HIGH);
          return;
        }
      }
    }
  }
}