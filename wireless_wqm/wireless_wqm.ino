// hao jin
// 2017.9.18
#include <SoftwareSerial.h> // for BlueTooth
#include <OneWire.h> // for DS18B20
#include <Wire.h>
#include <Adafruit_ADS1015.h>

// DS18B20 temperature sensor.
int DS18S20_Pin = 6; //DS18S20 Signal pin on digital 2
OneWire ds(DS18S20_Pin);  // on digital pin 2

// BlueTooth
// [BT] <-->  [Arduino]
// VCC  <-->  5V
// GND  <-->  GND
// TxD  <-->  pin D2
// RxD  <-->  pin D3  
#define RxD 2
#define TxD 3
SoftwareSerial Serial_BT(RxD,TxD); 

// ADS1115
// [ADS1115]  <-->    [Arduino]
// VDD        <-->    5V
// GND        <-->    GND
// SCL        <-->    pin A5
// SDA        <-->    pin A4
// ADDR       <-->    GND
Adafruit_ADS1115 adc1(0x48);
//float Voltage_RE = 0.0; // pin A0
//float Voltage_Cl = 0.0; // pin A2
//float Voltage_pH = 0.0; // pin A3
//pin A1 for Ag temperature sensor
// adc0: A0 pin, sensing the setting voltage at reference electrode
// adc1: A1 pin, for Ag temperature sensor
// adc2: A2 pin, for Cl
// adc3: A3 pin, for pH
int16_t adc1_diff_0_1;  
int16_t adc1_diff_2_3; 
float voltage_pH = 0.0;
float current_Cl = 0.0;
//int16_t adc3;

//flag to start send data
//int flag = 0;
 
void setup() 
{ 
    pinMode(RxD, INPUT); 
    pinMode(TxD, OUTPUT); 
//    Serial_BT.begin(9600);         // Bluetooth baud rate  
    Serial.begin(9600);            // USB terminal baud rate 
    adc1.begin();
//    adc1.setGain(GAIN_ONE); // set PGA gain to 1 (LSB=0.125 mV)
    adc1.setGain(GAIN_TWO); // set PGA gain to 2 (LSB=0.0625 mV)
} 
 
void loop() 
{ 
// === response to python's write command ===
//  if (Serial.available() > 0)
//    {
//       if (Serial.read() == '!'){
//        Serial.println('C');
//        delay(2000);
//        flag = 1;
//       } 
//    }
// === end ===

  // DS18B20 temperature data
  float temperature = getTemp();

  // ADS1115, 
  // read from the ADC, and obtain a sixteen bits integer as a result


//  adc0 = ads.readADC_SingleEnded(0);
//  adc2 = ads.readADC_SingleEnded(2);
//  adc3 = ads.readADC_SingleEnded(3);
//  Voltage_RE = (adc0 * 0.125); // the resolution of ADC is 0.125 mV
//  Voltage_Cl = (adc2 * 0.125);
//  Voltage_pH = (adc3 * 0.125);
adc1_diff_0_1 = adc1.readADC_Differential_0_1();
adc1_diff_2_3 = adc1.readADC_Differential_2_3();
voltage_pH = -adc1_diff_0_1 * 0.0625; // in mV
current_Cl = -adc1_diff_2_3 * 0.0625 / 0.5; // in nA, feedback resistor = 500k


  // sending data to USB and Bluetooth ports
  // data format:
//  if (flag == 1){
//    Serial_BT.print(" ");
//    Serial_BT.print(temperature); 
//    Serial_BT.print(" ");
//    Serial_BT.print(Voltage_RE, 4);
//    Serial_BT.print(" ");
//    Serial_BT.print(Voltage_Cl, 4); 
//    Serial_BT.print(" ");
//    Serial_BT.print(Voltage_pH, 4); 
//    Serial_BT.print(" ");
//    Serial_BT.print("\n");
    Serial.print(" ");
    Serial.print(temperature); 
    Serial.print(" ");
    Serial.print(voltage_pH, 4);
    Serial.print(" ");
    Serial.print(current_Cl, 4); 
    Serial.print(" ");
    Serial.print("\n");
    
    // set sampling rate to 1 Hz.
    delay(1000);
//    }
}

float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad

  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  
  ds.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  
  return TemperatureSum;
  
}
