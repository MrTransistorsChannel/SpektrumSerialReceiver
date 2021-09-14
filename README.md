# SpektrumSerialReceiver Arduino Library
## Arduino Library for interfacing to Spektrum™ Serial Receivers

![IMAGE](https://www.spektrumrc.com/ProdInfo/LargeImages/SPM9645.png)
![IMAGE](https://www.spektrumrc.com/ProdInfo/SPM/450/SPM9745-450.jpg)
![IMAGE](https://www.spektrumrc.com/ProdInfo/SPM/450/SPM4647-450.jpg)
![IMAGE](https://www.spektrumrc.com/ProdInfo/SPM/450/SPM4648-450.jpg)

<a id="Notes"></a>
## Important notes
* ~~Library should start before turning on your transmitter, otherwise receiver power-up process should be controlled by Arduino pin~~ (_**Fixed after v2.0-rc1**_)
* Binding procedure should be executed before `begin()` method **within 200ms after receiver power-up**. If using Arduino with default bootloader, you should control receiver power-up process from Arduino pin, otherwise bind process will be impossible *(except for Arduino Micro/Leonardo with ATmega32U4)*
  
## Usage
This library provides user-friendly constants and data type for bind modes:
```c++
/**** Bind mode constants ****/
typedef enum Bind_mode{
    DSM2_22ms = 0x01,
    DSMX_22ms = 0xA2,
    DSM2_11ms = 0x12,
    DSMX_11ms = 0xB2
};
/*****************************/
```
### Methods list

```c++
void begin(HardwareSerial& serial, int bind_pin=0); // Initialization on HardwareSerial port. bind_pin variable should be set to receiver data pin for binding or could be left blank if not using bind functionality
void begin(Stream& stream, int bind_pin);           // Initialization on Stream object
void update();                                      // Getting and parsing data frames
uint16_t getChannel(uint8_t chnum);                 // Returns value for specified channel
uint8_t getRXLosses();                              // Returns number of signal losses calculated by Receiver
Bind_mode getBindMode();                            // Returns bind mode ID, can be compared with bind mode constants to get current mode
void bindRX(Bind_mode mode);                        // Enabling specified bind mode on receiver (uses Bind_mode constants
```
## Task list
- [x] Getting R/C channels data
- [x] Getting RX losses
- [x] Getting current bind (protocol) type
- [x] Auto-detection of channel resolution
- [x] Binding in internal DSM2/DSMX 22ms/11ms mode
- [x] Automatic synchronisation
- [x] Getting X-Plus™ channels data
- [ ] Spektrum™ VTX setup feature
