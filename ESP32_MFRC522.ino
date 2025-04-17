void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.write('\n');
}

void loop() {
  byte choice;
  Serial.println("**Select Mode**\n1. Change UID Mode\n2. Read Block Mode\n3. Write Block Mode");
  Serial.flush();
  while(!Serial.available());
  choice = Serial.read();
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