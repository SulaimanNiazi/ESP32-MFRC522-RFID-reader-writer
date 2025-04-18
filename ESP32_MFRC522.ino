#include <SPI.h>
#include <MFRC522.h>

#define SCK_PIN 4
#define MISO_PIN 5
#define MOSI_PIN 6
#define RST_PIN 7
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);                   // Create MFRC522 instance

void setup() {
  Serial.begin(9600);
  while(!Serial);
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);   // Init SPI bus
  mfrc522.PCD_Init();                               // Init MFRC522 card
  Serial.write('\n');
}

void loop(){
  Serial.print("**Select Mode**\n1. Change UID Mode\n2. Read Block Mode\n3. Write Block Mode\nChoice: ");
  byte buffer[34] = "", choice = readUART()[0];
  String string = "";
  switch(choice){
    case '1':
      Serial.println("Change UID Mode Selected");
      while(1){
        awaitCardDetection();
        mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card
      }
      break;
    case '2':
      Serial.println("Read Block Mode Selected");
      Serial.print("Enter Block Number: ");
      choice = readUART()[0];
      Serial.print("Block ");
      Serial.write(choice);
      Serial.println(" Selected.\nBring a card near to read.");
      while(1){
        awaitCardDetection();
        mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card
      }
      break;
    case '3':
      Serial.println("Write Block Mode Selected");
      Serial.print("Enter Block Number: ");
      choice = readUART()[0];
      Serial.print("Block ");
      Serial.write(choice);
      Serial.println(" Selected.");
      Serial.println("Enter block data:");
      string = readUART();
      string.getBytes(buffer, 20);
      for (int i = string.length(); i < 20; i++) buffer[i] = ' ';         // pad with spaces
      Serial.println("Block data saved.\nBring a card near to write.");
      while(1){
        awaitCardDetection();
        mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));               // dump some details about the card
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
  String input = ""; // String to store input
  while (true) {
    if (Serial.available() > 0) {
      byte inByte = Serial.read();  // Read the byte from UART
      // If the byte is a Line Feed (Enter key), stop reading
      if (inByte == 10) {  // Line Feed (LF)
        Serial.println();
        break;
      }
      // If the byte is Backspace, remove the last character if there is any
      if (inByte == 8) {  // Backspace (BS)
        if (input.length() > 0) {
          input.remove(input.length() - 1);  // Remove last character
          Serial.print("\b \b"); // Move the cursor back, print a space, and move it back again
        }
      } else {
        // Append the received character to the input string
        input += (char)inByte;
        Serial.print((char)inByte); // Print the character
      }
    }
  }  
  return input; // Return the collected input
}

void awaitCardDetection(){
  while(1){
    while(1){
      if ( ! mfrc522.PICC_IsNewCardPresent())break;     // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
      if ( ! mfrc522.PICC_ReadCardSerial())break;       // Select one of the cards
      Serial.println(F("**Card Detected:**"));
      return;
    }
  }
}