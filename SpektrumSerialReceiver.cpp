/*****************************************************************************************************************************************
 * Arduino Library for interfacing to Spektrum™ Serial Receivers.
 * Feature overview:
 *      * Reading 1024/2048 Spektrum™ serial protocols
 *      * Up to 20 RC channels available
 *      * Getting RX losses and bind type info
 *      * Binding Spektrum™ Serial Receivers in internal DSM2/DSMX 22ms/11ms modes
 * 
 * This source code is based on documentation obtained from:
 * https://www.spektrumrc.com/ProdInfo/Files/Remote%20Receiver%20Interfacing%20Rev%20A.pdf
 * "Specification for Spektrum Remote Receiver Interfacing. Enabling Use of Spektrum Remotes in Third-Party Products", Rev A 2016 April 12
 * 
 * Created 20 Jun 2020
 * by Ilya Kobets (aka MrTransistor)
 * 
 * Version 1.0
 *****************************************************************************************************************************************/
#include <Arduino.h>
#include "SpektrumSerialReceiver.h"

/**** Constructor ****/
SpektrumSerialReceiver::SpektrumSerialReceiver(int bind_pin=0){
    _pin = bind_pin;
}
/*********************/

/**** Initialisation on HardwareSerial ****/
void SpektrumSerialReceiver::begin(HardwareSerial& serial){
    serial.begin(115200);
    begin((Stream&)serial);
}
/******************************************/

/**** Initialisation on Stream object ****/
void SpektrumSerialReceiver::begin(Stream& stream){
    _stream = &stream;
}
/*****************************************/

/**** Updating channels ****/
void SpektrumSerialReceiver::update(){
    while (_stream->available()) {
        delay(1);
        rxlosses = _stream->read();
        bindMode = _stream->read();
        delay(1);
        for(int i = 0; i < 14; i++){
            if(!_stream->available()) return;
            packetData[i] = _stream->read();
        }
    }
    for (int i = 0; i < 7; i++) {
        if(bindMode == DSM2_22ms){
            channelData[((packetData[i * 2] & MASK_1024_CHANID) >> 2)] = ((packetData[i * 2] & MASK_1024_SXPOS) << 8) + packetData[i * 2 + 1];
        }
        else{
            channelData[((packetData[i * 2] & MASK_2048_CHANID) >> 3)] = ((packetData[i * 2] & MASK_2048_SXPOS) << 8) + packetData[i * 2 + 1];
        }
    }
}
/***************************/

/**** Returns specified channel value ****/
uint16_t SpektrumSerialReceiver::getChannel(uint8_t chnum){
    return channelData[chnum];
}
/*****************************************/

/**** Returns num of signal losses ****/
uint8_t SpektrumSerialReceiver::getRXLosses(){
    return rxlosses;
}
/*************************************/

/**** Returns bind mode ****/
Bind_mode SpektrumSerialReceiver::getBindMode(){
    return (Bind_mode)bindMode;
}
/***************************/

/**** Binding procedure ****/
void SpektrumSerialReceiver::bindRX(Bind_mode mode){
    pinMode(_pin, OUTPUT);
    switch(mode){
        case DSM2_22ms:
            send_bind_pulses(3);
            break;
        case DSM2_11ms:
            send_bind_pulses(5);
            break;
        case DSMX_22ms:
            send_bind_pulses(7);
            break;
        case DSMX_11ms:
            send_bind_pulses(9);
            break;
    }
    pinMode(_pin, OUTPUT);

}
/***************************/

/**** Private function for sending bind pulses ****/
void SpektrumSerialReceiver::send_bind_pulses(uint8_t num_pulses){
    for (int i = 0; i < num_pulses; i++){
        digitalWrite(_pin, 0);
        delayMicroseconds(120);
        digitalWrite(_pin, 1);
        delayMicroseconds(120);
    }
}
/**************************************************/