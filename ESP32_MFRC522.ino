#include <SPI.h>
#include <MFRC522.h>

#define SCK_PIN 4
#define MISO_PIN 5
#define MOSI_PIN 6
#define RST_PIN 7
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);                             // Create MFRC522 instance

void setup() {
  Serial.begin(9600);
  while(!Serial);
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);             // Init SPI bus
  mfrc522.PCD_Init();                                         // Init MFRC522 card
  Serial.write('\n');
}

void loop(){
  MFRC522::MIFARE_Key key;                                    // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  MFRC522::StatusCode status;
  String string = "";

  Serial.print("**Select Mode**\n1. Change UID Mode\n2. Read Block Mode\n3. Write Block Mode\nChoice: ");
  byte buffer[34] = "", len = 0, block_size = 20, choice = readUART()[0];
  
  switch(choice){
    case '1':
      Serial.println("Change UID Mode Selected");
      while(1){
        awaitCardDetection();
      }
      break;
    case '2':
      Serial.println("Read Block Mode Selected");
      Serial.print("Enter Block Number: ");
      choice = readUART()[0] - '0';
      Serial.print("Block ");
      Serial.print(choice);
      Serial.print(" Selected.\nEnter the number of characters to read: ");
      string = readUART();
      len = string.toInt();
      Serial.print(len);
      Serial.println(" characters will be read.\nBring a card near to read.");
      while(1){
        awaitCardDetection();
        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, choice, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Authentication failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
        }else{
          status = mfrc522.MIFARE_Read(choice, buffer, &block_size);
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
      Serial.println("Write Block Mode Selected");
      Serial.print("Enter Block Number: ");
      choice = readUART()[0] - '0';
      Serial.print("Block ");
      Serial.print(choice);
      Serial.println(" Selected.");
      Serial.println("Enter block data:");
      string = readUART();
      string.getBytes(buffer, block_size);
      for (int i = string.length(); i < block_size; i++) buffer[i] = ' '; // pad with spaces
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
      if (inByte == 10) {                                 // Line Feed (LF)
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
  while(1){
    if (mfrc522.PICC_IsNewCardPresent()){                 // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
      if (mfrc522.PICC_ReadCardSerial()){                 // Select one of the cards
        Serial.println(F("**Card Detected:**"));
        mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); // dump some identification details about the card
        return;
      }
    }
  }
}