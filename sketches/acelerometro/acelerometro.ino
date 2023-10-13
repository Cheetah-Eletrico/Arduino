#include<Wire.h>
#include"Cheetah.h"

CheetahSerial serial(24);
Acelerometro sensor;

void setup()
{
  Serial.begin(9600);
 
}
void loop()
{

  uint16_t *a = sensor.leituraVariaveis();

  String vet[7] = {"Aceleração X: ","Aceleração Y: ","Aceleração Z: ","Temperatura: ","Giroscópio X: ","Giroscópio Y: ","Giroscópio Z: "};
  
  Serial.print("|");
  
  for(int i = 0; i < 7; i++)
  {
    Serial.print(vet[i]);
    Serial.print(a[i]);
    Serial.print("|");
    }
    
  Serial.println();
  
  delay(100);
}