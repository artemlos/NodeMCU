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

void sendData( Data *data);

void setup()
{
    Serial.begin(9600);   // Debugging only
    //Serial.println("setup");
    pinMode(13, OUTPUT);

    // Initialise the IO and ISR
    //vw_set_ptt_inverted(true); // Required for DR3100
    vw_set_tx_pin(2);
    vw_setup(2000);  // Bits per sec
}


int counter = 0;

void loop()
{
  digitalWrite(13, true); // Flash a light to show transmitting
    
  Data to_send;
  to_send.type = Integer;
  uint32_t a = 123;//encodeSensorValue(22, 123);
  
  to_send.value.i = a; // temperature sensor is 1, so 445 could mean 44.5 degrees.
  Serial.println(a,HEX);
  sendData(&to_send);
    
  digitalWrite(13, false);
  delay(100);
  
}

//store data and sensor type in the int (that we later send to the other device)
uint32_t encodeSensorValue (short data, uint8_t sensor) {
  uint32_t result = sensor;
  result = result << 24;
  return result | data;
}

//get the sensor value and the sensor type from the other device.
uint8_t decodeSensorValue (uint32_t encodedValue, short* value) {
  *value = encodedValue & 0xFFFF;
  return (encodedValue >> 24) & 0xFF;
}

// used by the transmitter
uint8_t*  convertData ( Data *data) {

  uint8_t buf[VW_MAX_MESSAGE_LEN];
  
  if (data->type == Boolean) {
    // copy a bool
    buf[0] = 0;
    buf[1] = data->value.b;
  } else {
    //copy a 32 bit int
    uint32_t value = data->value.i;
    buf[0] = 1;
  
    int j;
    for(j = 1; j < 5; j++) {
      buf[j] = value & 0xFF;
      value = value >> 8;
    }
  }

  return buf;
}

void sendData ( Data *data) {

  if (data->type == Boolean) {
    // a bool takes two bytes to send
    vw_send(convertData(data), 2);
  } else {
    // an integer takes five bytes to send
    vw_send(convertData(data), 5);
  }
      
  vw_wait_tx(); // Wait until the operation is complete
}




