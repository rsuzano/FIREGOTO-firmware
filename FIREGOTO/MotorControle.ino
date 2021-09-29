/*
    FireGoTo - an Arduino Motorized Telescope Project for Dobsonian Mounts
    https://firegoto.com.br
    Copyright (C) 2021  Rangel Perez Sardinha / Marcos Lorensini originally created by Reginaldo Nazar

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

*/
void IniciaMotores()
{
  //Iniciar as variaveis do motor de passo

  pinMode(MotorAZ_Ativa, OUTPUT);
  pinMode(MotorALT_Ativa, OUTPUT);
  pinMode(MotorALT_Direcao, OUTPUT);
  pinMode(MotorALT_Passo, OUTPUT);
  pinMode(MotorAZ_Direcao, OUTPUT);
  pinMode(MotorAZ_Passo, OUTPUT);

#ifdef DRV8825
  pinMode(MotorALT_Sleep, OUTPUT);
  pinMode(MotorALT_Reset, OUTPUT);
  pinMode(MotorALT_M2, OUTPUT);
  pinMode(MotorALT_M1, OUTPUT);
  pinMode(MotorALT_M0, OUTPUT);
  pinMode(MotorAZ_Sleep, OUTPUT);
  pinMode(MotorAZ_Reset, OUTPUT);
  pinMode(MotorAZ_M2, OUTPUT);
  pinMode(MotorAZ_M1, OUTPUT);
  pinMode(MotorAZ_M0, OUTPUT);


  //Aciona os pinos por padrão
  digitalWrite(MotorALT_Sleep, HIGH);
  digitalWrite(MotorALT_Reset, HIGH);
  digitalWrite(MotorALT_M2, HIGH);
  digitalWrite(MotorALT_M1, HIGH);
  digitalWrite(MotorALT_M0, HIGH);

  digitalWrite(MotorAZ_Sleep, HIGH);
  digitalWrite(MotorAZ_Reset, HIGH);
  digitalWrite(MotorAZ_M2, HIGH);
  digitalWrite(MotorAZ_M1, HIGH );
  digitalWrite(MotorAZ_M0, HIGH);
#endif

  digitalWrite(MotorALT_Direcao, LOW);
  digitalWrite(MotorALT_Passo, LOW);
  digitalWrite(MotorAZ_Direcao, LOW);
  digitalWrite(MotorAZ_Passo, LOW);
  digitalWrite(MotorALT_Ativa, LOW);
  digitalWrite(MotorAZ_Ativa, LOW);
  AltMotor.setMaxSpeed((dReducao / 10)*MinTimer);
  AzMotor.setMaxSpeed((dReducao / 10)*MinTimer);
  setaccelmsteps(dReducao);
  Serial.print("setaccelmsteps(dReducao);"); Serial.println(dReducao);
  startTimer(MinTimer);
}

void SentidodosMotores()
{
  if (SentidoDEC == 1)
  {
    AzMotor.setPinsInverted(true);
  }
  else
  {
    AzMotor.setPinsInverted(false);

  }
  if (SentidoRA == 1)
  {
    AltMotor.setPinsInverted(true);

  }
  else
  {
    AltMotor.setPinsInverted(false);

  }


}
void runmotorAlt ()

{
  //Serial.println("runmotorAlt");
  //cli();
  if (setupflag == 0)
  {
    AltMotor.run();
  }
  else
  {
    AltMotor.runSpeed();
  }
  //sei();
}

void runmotorAz ()

{
  //cli();
  //Serial.println("runmotorAz");
  if (setupflag == 0)
  {
    AzMotor.run();
  }
  else
  {
    AzMotor.runSpeed();
  }
  // sei();
}

void setaccel()
{
  Serial.print("setaccel()=>"); Serial.println(dReducao);
  setaccelmsteps(dReducao);
}
void setMaxSpeeed() {
  Serial.print("setMaxSpeed"); Serial.println(dReducao);
  setSpeed((dReducao / 10)*MinTimer);
}
void setSpeed(int maxSpeed) {
  AltMotor.setMaxSpeed(maxSpeed);
  AzMotor.setMaxSpeed(maxSpeed);
}
void setaccelmsteps(int msteps) {
  printAccelAndSpeed();
  Serial.print("setaccelmsteps "); Serial.println(msteps);
  AzMotor.setAcceleration(msteps * MinTimer);
  AltMotor.setAcceleration(msteps * MinTimer);
  printAccelAndSpeed();
}
void setaccel(int Accel)
{
  printAccelAndSpeed();
  AltMotor.setAcceleration(Accel);
  AzMotor.setAcceleration(Accel);
  printAccelAndSpeed();
}
void printAccelAndSpeed() {
  char str[200];
  sprintf(str, "AltMotor.getMaxSpeed=>%d AltMotor.getAcceleration=>%d AzMotor.getMaxSpeed=>%d AzMotor.getAcceleration=>%d",
          AltMotor.maxSpeed(),
          AltMotor.acceleration(),
          AzMotor.maxSpeed(),
          AzMotor.acceleration());
  Serial.println(str);
}
void addbackslash()
{
  if (AzMotor.distanceToGo() >= 0)
  {
    dirAz = 1;
  } else {
    dirAz = 0;
  }
  if (AltMotor.distanceToGo() >= 0)
  {
    dirAlt = 1;
  } else {
    dirAlt = 0;
  }
  if (AtivaBack == 1)
  {
    AtivaBack = 0;
    if (dirAlt != dirAltant)
    {
      if (dirAlt == 0)
      {
        Serial.println("alt:0");
        AltMotor.setCurrentPosition(AltMotor.currentPosition() + RAbacklash);
      } else
      {
        AltMotor.setCurrentPosition(AltMotor.currentPosition() - RAbacklash);
        Serial.println("alt:1");

      }
    }
    if (dirAz != dirAzant)
    {
      if (dirAz == 0)
      {
        AzMotor.setCurrentPosition( AzMotor.currentPosition() + DECbacklash);
        Serial.println("az: 0 ");

      } else
      {
        AzMotor.setCurrentPosition( AzMotor.currentPosition() - DECbacklash);
        Serial.println("az: 1");

      }
    }
  }
}


void syncro()
{
  AltMotor.setCurrentPosition((int)ALTmountAlvo);
  AzMotor.setCurrentPosition((int)AZmountAlvo);
  CalcPosicaoPasso();
}

void SetPosition()
{
  AltMotor.moveTo((int)ALTmountAlvo);
  AzMotor.moveTo((int)AZmountAlvo);
}


int paramotors()
{
  AZmountAlvo = AZmount;
  ALTmountAlvo = ALTmount;
  return (1);
}


void BaixaResolucao ()
{
  if ( MaxPassoAz == dMaxPassoAz)
  {
    MaxPassoAz = dMaxPassoAz / dReducao;
    MaxPassoAlt = dMaxPassoAlt / dReducao;
#ifdef DRV8825
    digitalWrite(MotorALT_M2, LOW);
    digitalWrite(MotorALT_M1, LOW);
    digitalWrite(MotorALT_M0, LOW);
    digitalWrite(MotorAZ_M2, LOW);
    digitalWrite(MotorAZ_M1, LOW );
    digitalWrite(MotorAZ_M0, LOW);
#endif
#ifdef TMC2209
    Serial.print("baixaResolucao "); Serial.println(dReducao);
    driverAz.microsteps(0);
    driverAlt.microsteps(0);
#endif
    AltMotor.setCurrentPosition((int)AltMotor.currentPosition() / dReducao);
    AzMotor.setCurrentPosition((int)AzMotor.currentPosition() / dReducao);
    setaccelmsteps(dReducao);
    Serial.print("baixaResolucao.setaceelmsteps "); Serial.println(dReducao);
    CalculaResolucao();
    CalcPosicaoPasso();
    ledStateG = HIGH;
  }
}

void AltaResolucao ()
{
  if ( MaxPassoAz != dMaxPassoAz)
  {
    MaxPassoAz = dMaxPassoAz;
    MaxPassoAlt = dMaxPassoAlt;
#ifdef DRV8825
    digitalWrite(MotorALT_M2, AltaM2);
    digitalWrite(MotorALT_M1, AltaM1);
    digitalWrite(MotorALT_M0, AltaM0);
    digitalWrite(MotorAZ_M2, AltaM2);
    digitalWrite(MotorAZ_M1, AltaM1);
    digitalWrite(MotorAZ_M0, AltaM0);
#endif
#ifdef TMC2209
    Serial.print("altaResolucao"); Serial.println(dReducao);
    driverAz.microsteps(dReducao);
    driverAlt.microsteps(dReducao);
#endif


    AltMotor.setCurrentPosition((int)AltMotor.currentPosition() * dReducao);
    AzMotor.setCurrentPosition((int)AzMotor.currentPosition() * dReducao);
    CalculaResolucao();
    CalcPosicaoPasso();
    Serial.print("AltaResolucao.setaccelmteps"); Serial.println(dReducao);
    setaccelmsteps(dReducao);
    ledStateG = LOW;
  }
}




void CalculaResolucao()
{
  ResolucaoeixoAltGrausDecimal = 360.0 / MaxPassoAlt ;
  ResolucaoeixoAzGrausDecimal = 360.0 / MaxPassoAz ;
  ResolucaoeixoAltPassoGrau = (MaxPassoAlt  / 360.0);
  ResolucaoeixoAzPassoGrau = (MaxPassoAz  / 360.0);
}


void CalcPosicaoPasso()
{
  ALTmount = AltMotor.currentPosition();
  AZmount = AzMotor.currentPosition();
  eixoAltGrausDecimal = ResolucaoeixoAltGrausDecimal * ALTmount;
  eixoAzGrausDecimal = ResolucaoeixoAzGrausDecimal * AZmount;
}

void protegemount()
{
  if ((eixoAltGrausDecimal < 1 ) || (eixoAltGrausDecimal > 90))
  {
    paramotors();
    ativaacom = 0;
  }


  if ((eixoAzGrausDecimal < 1) || (eixoAzGrausDecimal > 360))
  {
    paramotors();
    ativaacom = 0;
  }
}
boolean isMotorAltRunning() {
  return AltMotor.isRunning();
}
boolean isMotorAzRunning() {
  return AzMotor.isRunning();
}
