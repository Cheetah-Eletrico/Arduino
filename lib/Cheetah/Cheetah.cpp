#include "Cheetah.h"

CheetahSerial::CheetahSerial()
{
  this->contA = 1;
  this->contD = 0;
  this->vel = 30;
  memset(this->payload , 0 , sizeof(this->payload));
  memset(this->digital , 0 , sizeof(this->digital));
}

void CheetahSerial::addAnalogSensor(uint16_t value)
{
  this->payload[contA] =  highByte(value);
  this->payload[contA+1] = lowByte(value);
  this->contA += 2;
}

void CheetahSerial::addDigitalSensor(uint16_t value)
{
  this->digital[contD] = value;
  this->contD++;
}

void CheetahSerial::sendPayload()
{
  int temp[N_SENSORES_DISCRETO/8];
  for(int i = 0 ; i < N_SENSORES_DISCRETO/8 ; i++)
  {
    int aux = 0;
    int bin = 1;
    for(int j = i*8 + 7 ; j >= i*8 ; j--)
    {
      aux += this->digital[j] * bin;
      bin *=2;
    }
    temp[i] = aux;
  }

  int cont = 0;
  for (int i = 1 + (N_SENSORES_MEDICAO * 2) ; i < MSG_SIZE-1 ; i++)
  {
    this->payload[i] = temp[cont];
    cont++;
  }
  //this->payload[5] = 255;

  this->contA = 1;
  this->contD = 0;

  this->payload[0] = 255;
  this->payload[MSG_SIZE-1] = 254;
  Serial.write(this->payload, MSG_SIZE);
}



Acelerometro::Acelerometro()
{
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);

  //Inicializa o MPU-6050
  Wire.write(0);
  Wire.endTransmission(true);

  memset(variaveis, 0, sizeof(variaveis));
}

uint16_t* Acelerometro::leituraVariaveis()
{
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  //Solicita os dados do sensor
  Wire.requestFrom(MPU,14,true);

  for(int i = 0; i < 7; i++)
  {
    variaveis[i] = Wire.read()<<8|Wire.read();
  }

  return variaveis;
}

uint8_t CheetahCAN::beginCAN()
{
  int init_status = begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ);
  setMode(MCP_NORMAL);
  if(init_status == CAN_OK)
    return CAN_OK;
  else
    return init_status;
  pinMode(CAN0_INT, INPUT);

  this->cont8 = 0;
  memset(this->payload , 0 , sizeof(this->payload));
}

void CheetahCAN::addToPayload8(byte value)
{
  this->payload[this->cont8] = value;
  this->cont8++;
}

uint8_t CheetahCAN::sendMessage(uint16_t id)
{
  byte sndStat = sendMsgBuf(id, 0, this->cont8, this->payload);
  this->cont8 = 0;
  if(sndStat == CAN_OK)
    return 0;

  Serial.println(sndStat);
  return 1;
}

bool CheetahCAN::readMessage()
{
  if(!digitalRead(CAN0_INT))
  {
    readMsgBuf(&rxId, &rxLen, rxBuf);
    return true;
  }
  return false;
}

byte* CheetahCAN::getMsg()
{
  return rxBuf;
}

uint16_t CheetahCAN::getMsgId()
{
  return rxId;
}

uint16_t CheetahCAN::getMsgLen()
{
  return rxLen;
}

CelulaDeCarga::CelulaDeCarga()
{
  begin(CELULACARGA_DOUT_PIN , CELULACARGA_SCK_PIN);
}

uint16_t CelulaDeCarga::testeCelula()
{
  Serial.println("Inicializando a celula");
  Serial.print("Leitura bruta do ADC");
  Serial.println(read());
  Serial.print("Media de 20 leituras");
  Serial.println(read_average(20));
  Serial.print("Coloque um objeto de peso conhecido e anote o valor obtido a seguir");
  set_scale(CELULACARGA_DIV);
  tare();
  Serial.println(get_value(20));
  Serial.println("Ajuste o parametro em config.h para obter uma medida precisa");
}
