/* Código fonte para teste de célula de carga e strain gauge
 * Não esquecer de configurar os pinos no config.h
 * Plataforma: ATmega328p
 * Utiliza a biblioteca HX711 de bogde: https://github.com/bogde/HX711
 * Autor: Caio Tácito
 * Data: 04/05/2021
*/
#include "Cheetah.h"

CelulaDeCarga celula;
void setup()
{
  Serial.begin(115200);
}

void loop()
{
  celula.testeCelula();
  delay(500);
}
