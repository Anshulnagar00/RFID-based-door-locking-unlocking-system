# RFID-based-door-locking-unlocking-system

Rfid based iot door unlocking system using firebase realtime database and nodeMCU

**Demonstration link -** 
https://drive.google.com/folderview?id=14oUnM18M65QeM8jWbs2tOGUJ0tqh3Dg7

**Components required -** 
1.  MFRC522 RFID Module
2.  LCD Display with I2C Module
3.  NodeMCU
4.  Servo Motors
5.  Mechanical Arrangemet for Door 
  
**Connections -**
* I2C:RST - NodeMCU:D1
* I2C:SDA - NodeMCU:D2
* I2C:SCK - NodeMCU:D5
* I2C:MISO - NodeMCU:D6
* I2C:MOSI - NodeMCU:D7
* NodeMCU:D3 - (Invalid entry signal) Red LED
* NodeMCU:D4 - (Door open Signal) Green LED
