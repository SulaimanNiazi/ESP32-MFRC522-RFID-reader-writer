void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.write('\n');
}

void loop(){
  Serial.print("**Select Mode**\n1. Change UID Mode\n2. Read Block Mode\n3. Write Block Mode\nChoice: ");
  byte choice = readUART()[0];
  switch(choice){
    case '1':
      Serial.println("Change UID Mode Selected");
      while(1){

      }
      break;
    case '2':
      Serial.println("Read Block Mode Selected");
      while(1){

      }
      break;
    case '3':
      Serial.println("Write Block Mode Selected");
      while(1){

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