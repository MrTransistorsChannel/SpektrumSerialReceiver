/*****************************************************************************************************************************************
 * Arduino Library for interfacing to Spektrum™ Serial Receivers.
 * Feature overview:
 *      * Reading 1024/2048 Spektrum™ serial protocols
 *      * Up to 12 RC channels + 8 X-plus channels available
 *      * Getting RX losses and bind type info
 *      * Binding Spektrum™ Serial Receivers in internal DSM2/DSMX 22ms/11ms modes
 *      * Automatic packet synchronisation
 * 
 * This source code is based on documentation obtained from:
 * https://github.com/SpektrumRC/SpektrumDocumentation/blob/master/Telemetry/Remote%20Receiver%20Interfacing.pdf
 * "Specification for Spektrum Remote Receiver Interfacing. Enabling Use of Spektrum Remotes in Third-Party Products", Rev G 2019 January 22
 * 
 * Created 20 Jun 2020
 * by Ilya Kobets (aka MrTransistor)
 * 
 * Modified 22 Jun 2020
 * by Ilya Kobets
 * 
 * Version 2.1
 *****************************************************************************************************************************************/

/**** Bind mode constants ****/
typedef enum Bind_mode{
    DSM2_22ms = 0x01,
    DSMX_22ms = 0xA2,
    DSM2_11ms = 0x12,
    DSMX_11ms = 0xB2
};
/*****************************/

/**** Main class ****/
class SpektrumSerialReceiver{
    public:
        SpektrumSerialReceiver(int bind_pin = 0);           // Constructor
        void begin(HardwareSerial& serial);                 // Initialization on HardwareSerial port. bind_pin variable should be set to receiver data pin for binding or could be left blank if not using bind functionality
        void begin(Stream& stream);                         // Initialization on Stream object
        void update();                                      // Getting and parsing data frames
        uint16_t getChannel(uint8_t chnum);                 // Returns value for specified channel
        uint8_t getRXLosses();                              // Returns number of signal losses calculated by Receiver
        Bind_mode getBindMode();                            // Returns bind mode ID, can be compared with bind mode constants to get current mode
        void bindRX(Bind_mode mode);                        // Enabling specified bind mode on receiver (uses Bind_mode constants
                                                            // bindRX() SHOULD BE USED BEFORE begin() METHOD WITHIN 200ms AFTER RECEIVER POWER-UP!!!!
    private:
        Stream* _stream;

        int _pin;

        static const uint8_t MASK_1024_CHANID  = 0xFC;      // Channel ID and servo position masks for 10 & 11 bit protocols
        static const uint8_t MASK_2048_CHANID  = 0x78;
        static const uint8_t MASK_XPLUS_CHANID = 0x03;
        static const uint8_t MASK_1024_SXPOS   = 0x03;
        static const uint8_t MASK_2048_SXPOS   = 0x07;
        static const uint8_t MASK_XPLUS_SXPOS  = 0x01;

        uint8_t packetData[14];
        uint16_t channelData[20];
        uint8_t rxlosses;
        uint8_t bindMode;

        void send_bind_pulses(uint8_t num_pulses);
};
/********************/
