/*
 *   FireGoTo - an Arduino Motorized Telescope Project for Dobsonian Mounts
    Copyright (C) 2020  Rangel Perez Sardinha / Marcos Lorensini

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */
void RotinadeSetup() //:HSETUPON#
{
  SerialPrint("\n ######################################################################### \n");
  if (setupflag == 0)
  {
    SerialPrint(" \n Rotina inicial de Setup \n ");
    digitalWrite(MotorALT_M2, LOW);
    digitalWrite(MotorALT_M1, LOW);
    digitalWrite(MotorALT_M0, HIGH);
    digitalWrite(MotorAZ_M2, LOW);
    digitalWrite(MotorAZ_M1, LOW);
    digitalWrite(MotorAZ_M0, HIGH);
  }
  setupflag = 2;

  SerialPrint(" \n O valor atual timer e: ");
  SerialPrint(String(MinTimer - 200));
  SerialPrint(" (:HST00000# -> Quanto menor mais rapido ate o limite do motor ambos motores) \n");
  delay(1000);

  SerialPrint(" \n O Sentido do motor RA/Alt e: ");
  if (SentidoRA == 0)
  {
    SerialPrint("Horario (:HSSRA0# -> Horario // :HSSRA1# -> AntiHorario)\n");
  }
  else
  {
    SerialPrint("Anti-Horario (:HSSRA0# -> Horario // :HSSRA1# -> AntiHorario)\n");
  }
  delay(1000);

  SerialPrint(" \n Acionamento do motor RA/ALT em velocidade maxima \n ");
  AzMotor.setSpeed(0);
  AltMotor.setSpeed(MinTimer * MinTimer);
  delay(6000);
  AltMotor.setSpeed(0);


  SerialPrint(" \n O Sentido do motor DEC/AZ e: ");
  if (SentidoDEC == 0)
  {
    SerialPrint("Horario (:HSSDEC0# -> Horario // :HSSDEC1# -> AntiHorario)\n");
  }
  else
  {
    SerialPrint("Anti-Horario (:HSSDEC0# -> Horario // :HSSDEC1# -> AntiHorario)\n ");
  }
  delay(1000);

  SerialPrint(" \n Acionamento do motor DEC/AZ em velocidade maxima \n ");
  AltMotor.setSpeed(0);
  AzMotor.setSpeed(MinTimer * MinTimer);
  delay(6000);
  AzMotor.setSpeed(0);

  AzMotor.setSpeed(0);
  AltMotor.setSpeed(0);
  SerialPrint(" \n :HSRD000# -> Valor de Micropassos: ");
  SerialPrint(String(Reducao));
  SerialPrint(" \n Total da Relacao de engrenagens de RA/ALT: ");
  SerialPrint(String(MaxPassoAlt));
  SerialPrint(" (:HSAL00000000# -> (reducao * numero passos * micropasso)\n");
  SerialPrint(" \n Total da Relacao de engrenagens de DEC/AZ: ");
  SerialPrint(String(MaxPassoAz));
  SerialPrint(" (:HSAZ00000000# -> (reducao * numero passos * micropasso) \n");

  SerialPrint(" \n Para sair :HSETUPOFF# ");
  setupflag = 1;
}

void RotinadeSetupOff() //:HSETUPOFF#
{
  setupflag = 0;
}
