#include <esp32ModbusRTU.h>
byte address=2; //server id - device address
esp32ModbusRTU modbus(&Serial2, 4); //4 RTSpin => DE/RS
//esp32ModbusRTU modbus(&Serial1, 4);  // use Serial1 and pin 4 as RTS

void Init_modbus(){
  Serial2.begin(4800);
  //Serial1.begin(4800, SERIAL_8N1, 16, 17);  // Modbus connection with RX: GPIO 16 => RX2, TX: GPIO 17 => TX2
  Serial.println("Init MODBUS");
  modbus.onData([](uint8_t serverAddress, esp32Modbus::FunctionCode fc, uint16_t address, uint8_t* data, size_t length) 
    {
      digitalWrite(2,HIGH);
      Serial.printf("received data: id: %u, fc %u\ndata: 0x", serverAddress, fc);
      for (uint8_t i = 0; i < length; ++i) {
        Serial.printf("%02x", data[i]);
      }
      Serial.print("\n");
    }
  );
  modbus.onError([](esp32Modbus::Error error) {
     Serial.printf("error: 0x%02x\n\n", static_cast<uint8_t>(error));
  });
  modbus.begin();
}

void setup() {
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(115200);
  Serial.print("GATEWAY init");
  delay(300);
  Init_modbus();
}


void loop(){ 
  modbus.readHoldingRegisters(address,0x00,1); // serverId, address + length
  delay(1000);
  digitalWrite(2,LOW);
}


