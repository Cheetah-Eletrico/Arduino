#ifndef CHEETAH_H
#define CHEETAH_H
#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))
#define MSG_SIZE 10 //EM BYTES, CONSULTAR DOCUMENTACAO
#define N_SENSORES_MEDICAO 4
#define N_SENSORES_DISCRETO 0
#define CAN0_INT 2
#include "Arduino.h"
#include <mcp_can.h>
#include <Wire.h>
#include <SPI.h>
#include <HX711.h>
#include <config.h>

class CheetahSerial
{
  private:
    byte payload[MSG_SIZE];
    uint16_t digital[N_SENSORES_DISCRETO];
    uint16_t contA;
    uint16_t contD;
    bool subindo;
    unsigned int vel;
  public:
    CheetahSerial();
    void addToPayload(uint16_t value);
    void sendPayload();
    void addAnalogSensor(uint16_t value);
    void addDigitalSensor(uint16_t value);
};

class Acelerometro
{
  private:
   uint16_t variaveis[7];
   const int MPU=0x68;
  public:
   Acelerometro();
   uint16_t* leituraVariaveis();
};

class CheetahCAN : public MCP_CAN
{
  using MCP_CAN::MCP_CAN;
  private:
    byte payload[8];
    uint8_t cont8;
    long unsigned int rxId;
    unsigned char rxLen = 0;
    unsigned char rxBuf[8];
  public:
    void addToPayload8(byte value);
    uint8_t beginCAN();
    bool readMessage();
    uint8_t sendMessage(uint16_t id);
    uint16_t getMsgId();
    uint16_t getMsgLen();
    byte* getMsg();
};

class CelulaDeCarga : public HX711
{
  public:
    CelulaDeCarga::CelulaDeCarga();
    uint16_t testeCelula();
};

#endif
