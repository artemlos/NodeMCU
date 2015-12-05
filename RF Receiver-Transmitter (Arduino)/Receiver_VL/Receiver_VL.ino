/**
 * @author Artem Los
 */

#include <VirtualWire.h>

enum DataType {
  Integer,
  Boolean
};


union DataValue {
  uint32_t i;
  uint8_t b;
};


struct DataStruct {
  DataType type;
  DataValue value;
};

typedef struct DataStruct Data;


void setup()
{
  Serial.begin(9600);   // Debugging only
  Serial.println("setup receier");
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);      // % volt source  
  // Initialise the IO and ISR
  //vw_set_ptt_inverted(true); // Required for DR3100
  vw_set_rx_pin(2);
  vw_setup(2000);    // Bits per sec

  vw_rx_start();       // Start the receiver PLL running

}

void loop()
{
  receiveMessage();
}


uint8_t decodeSensorValue (uint32_t encodedValue, short* value) {
  *value = encodedValue & 0xFFFF;
  return (encodedValue >> 24) & 0xFF;
}

void receiveMessage () {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) // Non-blockingSerial.println(data.value.i, HEX);
  {

    Serial.print("Got: ");
  int i;
  for (i = 0; i < buflen; i++)
  {
      Serial.print(buf[i], HEX);
      Serial.print(' ');
  }
  Serial.println("");
    //Data data = convertData(buf);

    
    //short* sensor_value;
    //uint8_t sensor = decodeSensorValue(data.value.i, sensor_value);
    
    //Serial.println(sensor);
    /*Serial.print(" ");
    Serial.print(sensor);
    Serial.println("");*/

    /*int k;
    for(k=0; k < sensor; k++) {
      digitalWrite(13, true);
      delay(500);
      digitalWrite(13, false);
      delay(1000);
    }
    delay(5000);*/
  }
}

// used by the receiver
Data convertData (uint8_t *buf) {
  Data data;

  
  if (buf[0] == 0) {
    // paste a bool
    data.type = Boolean;
    data.value.b = buf[1];
    data.value.i = 0;
  } else if (buf[0] == 1) {
    //paste a 32 bit int
    data.value.i = 0;
    int j;
    for(j = 4; j > 0; j--) {
      data.value.i = data.value.i << 8;
      data.value.i |= buf[j];
    }
    data.value.b= 0;
  }

  return data;
}
