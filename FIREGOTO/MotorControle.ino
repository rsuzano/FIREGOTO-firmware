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
void IniciaMotores()
{
  //Iniciar as variaveis do motor de passo
  pinMode(MotorALT_Direcao, OUTPUT);
  pinMode(MotorALT_Passo, OUTPUT);
  pinMode(MotorALT_Sleep, OUTPUT);
  pinMode(MotorALT_Reset, OUTPUT);
  pinMode(MotorALT_M2, OUTPUT);
  pinMode(MotorALT_M1, OUTPUT);
  pinMode(MotorALT_M0, OUTPUT);
  pinMode(MotorALT_Ativa, OUTPUT);
  pinMode(MotorAZ_Direcao, OUTPUT);
  pinMode(MotorAZ_Passo, OUTPUT);
  pinMode(MotorAZ_Sleep, OUTPUT);
  pinMode(MotorAZ_Reset, OUTPUT);
  pinMode(MotorAZ_M2, OUTPUT);
  pinMode(MotorAZ_M1, OUTPUT);
  pinMode(MotorAZ_M0, OUTPUT);
  pinMode(MotorAZ_Ativa, OUTPUT);

  //Aciona os pinos por padrão
  digitalWrite(MotorALT_Direcao, LOW);
  digitalWrite(MotorALT_Passo, LOW);
  digitalWrite(MotorALT_Sleep, HIGH);
  digitalWrite(MotorALT_Reset, HIGH);
  digitalWrite(MotorALT_M2, HIGH);
  digitalWrite(MotorALT_M1, HIGH);
  digitalWrite(MotorALT_M0, HIGH);
  digitalWrite(MotorALT_Ativa, LOW);
  digitalWrite(MotorAZ_Direcao, LOW);
  digitalWrite(MotorAZ_Passo, LOW);
  digitalWrite(MotorAZ_Sleep, HIGH);
  digitalWrite(MotorAZ_Reset, HIGH);
  digitalWrite(MotorAZ_M2, HIGH);
  digitalWrite(MotorAZ_M1, HIGH );
  digitalWrite(MotorAZ_M0, HIGH);
  digitalWrite(MotorAZ_Ativa, LOW);

  AltMotor.setMaxSpeed(dMaxSpeedAlt);
  AltMotor.setAcceleration(dReducao);
  AzMotor.setMaxSpeed(dMaxSpeedAz);
  AzMotor.setAcceleration(dReducao);
  Timer3.start(MinTimer);
  Timer3.attachInterrupt(runmotor);

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

void runmotor ()
{
  if (setupflag == 0)
  {
    AltMotor.run();
    AzMotor.run();
  }
  else
  {
    AltMotor.runSpeed();
    AzMotor.runSpeed();
  }
}

void setaccel()
{
  double tempdis;
  tempdis = abs(AzMotor.distanceToGo());
  AzMotor.setMaxSpeed(tempdis * dReducao );
  tempdis = abs(AltMotor.distanceToGo());
  AltMotor.setMaxSpeed(tempdis * dReducao );
}

void setaccel(int Accel)
{
  AltMotor.setMaxSpeed(Accel);
  AzMotor.setMaxSpeed(Accel);
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
    digitalWrite(MotorALT_M2, LOW);
    digitalWrite(MotorALT_M1, LOW);
    digitalWrite(MotorALT_M0, LOW);
    digitalWrite(MotorAZ_M2, LOW);
    digitalWrite(MotorAZ_M1, LOW );
    digitalWrite(MotorAZ_M0, LOW);
    AltMotor.setCurrentPosition((int)AltMotor.currentPosition() / dReducao);
    AzMotor.setCurrentPosition((int)AzMotor.currentPosition() / dReducao);
    AltMotor.setAcceleration(dReducao * 4);
    AzMotor.setAcceleration(dReducao * 4);
    CalculaResolucao();
    CalcPosicaoPasso();
    ledStateG = HIGH;
  }
}
void BaixaResolucaoAz ()
{
  if ( MaxPassoAz == dMaxPassoAz)
  {
    MaxPassoAz = dMaxPassoAz / dReducao;
    digitalWrite(MotorAZ_M2, LOW);
    digitalWrite(MotorAZ_M1, LOW );
    digitalWrite(MotorAZ_M0, LOW);
    AzMotor.setCurrentPosition((int)AzMotor.currentPosition() / dReducao);
    AzMotor.setAcceleration(dReducao * 4);
    CalculaResolucao();
    CalcPosicaoPasso();
    ledStateG = HIGH;
  }
}
void BaixaResolucaoAlt ()
{
  if ( MaxPassoAlt == dMaxPassoAlt)
  {
    MaxPassoAlt = dMaxPassoAlt / dReducao;
    digitalWrite(MotorALT_M2, LOW);
    digitalWrite(MotorALT_M1, LOW);
    digitalWrite(MotorALT_M0, LOW);
    AltMotor.setCurrentPosition((int)AltMotor.currentPosition() / dReducao);
    AltMotor.setAcceleration(dReducao * 4);
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
    digitalWrite(MotorALT_M2, AltaM2);
    digitalWrite(MotorALT_M1, AltaM1);
    digitalWrite(MotorALT_M0, AltaM0);
    digitalWrite(MotorAZ_M2, AltaM2);
    digitalWrite(MotorAZ_M1, AltaM1);
    digitalWrite(MotorAZ_M0, AltaM0);
    AltMotor.setCurrentPosition((int)AltMotor.currentPosition() * dReducao);
    AzMotor.setCurrentPosition((int)AzMotor.currentPosition() * dReducao);
    CalculaResolucao();
    CalcPosicaoPasso();
    AltMotor.setAcceleration(dReducao * dReducao * 2);
    AzMotor.setAcceleration(dReducao * dReducao * 2);
    ledStateG = LOW;
  }
}
void AltaResolucaoAz ()
{
  if ( MaxPassoAz != dMaxPassoAz)
  {
    MaxPassoAz = dMaxPassoAz;
    digitalWrite(MotorAZ_M2, AltaM2);
    digitalWrite(MotorAZ_M1, AltaM1);
    digitalWrite(MotorAZ_M0, AltaM0);
    AzMotor.setCurrentPosition((int)AzMotor.currentPosition() * dReducao);
    CalculaResolucao();
    CalcPosicaoPasso();
    AzMotor.setAcceleration(dReducao * dReducao * 2);
    ledStateG = LOW;
  }
}
void AltaResolucaoAlt ()
{
  if ( MaxPassoAlt != dMaxPassoAlt)
  {
    MaxPassoAlt = dMaxPassoAlt;
    digitalWrite(MotorALT_M2, AltaM2);
    digitalWrite(MotorALT_M1, AltaM1);
    digitalWrite(MotorALT_M0, AltaM0);
    AltMotor.setCurrentPosition((int)AltMotor.currentPosition() * dReducao);
    CalculaResolucao();
    CalcPosicaoPasso();
    AltMotor.setAcceleration(dReducao * dReducao * 2);
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
