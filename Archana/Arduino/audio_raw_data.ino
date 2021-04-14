#include <Audio.h>
#include <file.h>

AudioInputI2S            i2s1;
AudioRecordQueue         queue1;         //xy=281,63
AudioConnection          patchCord1(i2s1, 0, queue1, 0);


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  // Audio connections require memory, and the record queue
  // uses this memory to buffer incoming audio.
  AudioMemory(60);
}

void loop() {
  // put your main code here, to run repeatedly:

  queue1.begin();
  if (queue1.available() >= 1){
     byte buffer[256];  
  // Fetch 1 block from the audio library and copy
  // into a 256 byte buffer.
    memcpy(buffer, queue1.readBuffer(), 256);
    queue1.freeBuffer();
    Serial.println(".................Reading an audio packet : 256 byte array.......");
    for( size_t i = 0; i != 256; ++i){
      Serial.print(buffer[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
  

}
