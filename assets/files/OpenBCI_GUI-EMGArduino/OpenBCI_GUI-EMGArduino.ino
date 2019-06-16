////////////////////////////////////////////////////////////////////////
//            OpenBCI_GUI to Arduino via Serial: EMG                  //
//                                                                    //
//   Tested 6/16/2019 using iMac, Genuine Arduino, OpenBCI_GUI 4.1.3  //
////////////////////////////////////////////////////////////////////////

#define NUM_CHAN 16
#define BAUD_RATE 115200

const byte BufferSize = 96;
char Buffer[BufferSize+1];
boolean newData = false;
float emgData[NUM_CHAN];

void setup() {
    Serial.begin(BAUD_RATE);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("<Arduino is ready>");
}

void loop() {
    // Data Format 16CH
    // 0.999,0.001,0.002,0.003,0.004,0.005,0.006,0.007,0.008,0.009,0.010,0.011,0.012,0.013,0.014,0.015\n
    receiveMoreThan64Chars();
    if (newData) {
      parseData(",", Buffer);
      showData();
      newData = false;
    }
}

void receiveMoreThan64Chars() {
  if(Serial.available() > 0){

    // Clear Buffer content
    memset(Buffer, 0, BufferSize+1);
    
    while(Serial.available() > 0){
      // "readBytes" terminates if the determined length has been read, or it
      // times out. It fills "Buffer" with 1 to 90 bytes of data. To change the
      // timeout use: Serial.setTimeout() in setup(). Default timeout is 1000ms.
      Serial.readBytes(Buffer, BufferSize);
    }

    // Print out buffer contents
    //Serial.println(Buffer);

    // You can use Serial.peek() to check if more than 90 chars
    // were in the serial buffer and if Buffer has truncated data.
    // This should never happen because you know what the max length of
    // the incoming data is and you have adequately sized your input buffer.
    if(Serial.peek() != -1){
      Serial.println("90 byte Buffer Overflowed. ");
    }
    Buffer[sizeof(Buffer)] = '\0'; //terminate the string
    newData = true;
  }
}
  
void parseData(char * delimiter, char* str) {
    char * pch;
    pch = strtok (str,delimiter);
    int i = 0;
    while (pch != NULL) {
        emgData[i] = atof(pch);    
        pch = strtok (NULL, ",");
        i++;
    }   
}

void showData() {
  for(int i = 0; i < NUM_CHAN; i++) {
    Serial.println(emgData[i], 3);
  }
  if (emgData[0] > 0.5) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

