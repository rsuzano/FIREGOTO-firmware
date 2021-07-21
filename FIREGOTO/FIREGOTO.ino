/*
 *  FireGoTo - an Arduino Motorized Telescope Project for Dobsonian Mounts
 *  https://firegoto.com.br
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
 * 
 */
#include <AccelStepper.h>
#include <Arduino.h>
#include <math.h>
#include <Time.h>
#include <TimeLib.h>
//#include <DueTimer.h>
//#include <DueFlashStorage.h>
#include <EEPROM.h>
#include "EEPROMAnything.h"
//#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 

//DEBUG
int flagDebug = 0;

//2. Pinos Joystick
#define xPin     -1//32   
#define yPin     -1//33   
#define kPin     -1//34

//Menu e joystick
int tCount1;
bool refresh;//lcd clear On/Off
//leerJoystick
int joyRead;
int joyPos; 
int lastJoyPos;
long lastDebounceTime = 0; 
long debounceDelay = 70;                 //user define
//Control Joystick
bool PQCP;
bool editMode;
//sistema de menus
int mNivel1;  
int mNivel2;  
//editmode
byte n[19];
int lastN;
int lcdX;
//int lcdY;
bool exiT;


//Criacao dos motores
#define MotorALT_Direcao 26
#define MotorALT_Passo 27
#define MotorALT_Sleep -1//12
#define MotorALT_Reset -1//12
#define MotorALT_M2 -1
#define MotorALT_M1 14
#define MotorALT_M0 13
#define MotorALT_Ativa 12
#define MotorAZ_Direcao 0
#define MotorAZ_Passo 18
#define MotorAZ_Sleep -1//12
#define MotorAZ_Reset -1//12
#define MotorAZ_M2 -1
#define MotorAZ_M1 14
#define MotorAZ_M0 13
#define MotorAZ_Ativa 12


LiquidCrystal_I2C lcd(0x27,-1,-1);
 

AccelStepper AltMotor(AccelStepper::DRIVER, MotorALT_Passo, MotorALT_Direcao);
AccelStepper AzMotor(AccelStepper::DRIVER, MotorAZ_Passo, MotorAZ_Direcao);

int accel = 1;


//LEDs
#define LedB 25
#define LedR 25
#define LedG 25
int ledStateR = LOW;
int ledStateB = LOW;
int ledStateG = LOW;


/*valores maximo para o passo (Valor ideal 1286400)*/
double dMaxPassoAlt = 3844654; /* //valor de resolucao AR = Passo * MicroPasso * reducao ex(200*16*402)/4    (16*200*(117/11)*56)*/
double dMaxPassoAz = 3844654; /*/valor de resolucao AR = Passo * MicroPasso * reducao ex(200*16*402)   (16*200*(118/11)*57)*/
int dMinTimer = 500; /*/passo*/
double dMaxSpeedAlt = 1000;
double dMaxSpeedAz = 1000;
int dReducao = 32;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
volatile SemaphoreHandle_t timerSemaphore;
volatile boolean callRunMotor = false;
//Variaveis de persistencia e estrutura de dados ----------------------------------------------------------------------------------------------------------------

// The struct of the configuration.
struct Configuration {
  int32_t Reducao;
  int32_t MaxPassoAlt;
  int32_t MaxPassoAz;
  int32_t MinTimer;
  int32_t AltaM2;
  int32_t AltaM1;
  int32_t AltaM0;
  int32_t SentidoDEC;
  int32_t SentidoRA;
  uint32_t DataHora;
  double latitude;
  double longitude;
  int32_t UTC;
  char* Local;
};
Configuration configuration;
Configuration configurationFromFlash; // create a temporary struct

int Reducao = dReducao;
int MaxPassoAlt = dMaxPassoAlt;
int MaxPassoAz = dMaxPassoAz;
int MaxSpeedAz = dMaxSpeedAz;
int MaxSpeedAlt = dMaxSpeedAlt;
int SentidoDEC = 0;
int SentidoRA = 0;
int MinTimer = dMinTimer;
int AltaM2;
int AltaM1;
int AltaM0;
double latitude;
double longitude;
int UTC;



int fractime;
unsigned long currentMillis, previousMillis, PrimeiroCommanMillis, calculaRADECmountMillis = 0 , previousMillisLed = 0;


//Variaveis de controle para ler comandos LX200  ----------------------------------------------------------------------------------------------------------------
boolean cmdComplete = false, doispontos = true; // whether the string is complete
int pontBuffer = 0;
int numCommand = 0, numCommandexec = 0, flagCommand = 0;
char Command[15][15];



//Variaveis do SETUP
int setupflag = 0;

//Variaveis globais da MEADE
double SideralRate = 60.0; //HZ
int RAbacklash = 0; //(in ArcSec)
int DECbacklash = 0; //(in ArcSec)
int AtivaBack = 1;
int dirAlt, dirAz, dirAltant, dirAzant;

//Variaveis globais de posicao fisica do telescopio  ----------------------------------------------------------------------------------------------------------------
double eixoAltGrausDecimal = 0.0;
double eixoAzGrausDecimal = 0.0;
double ResolucaoeixoAltGrausDecimal = 0.0, ResolucaoeixoAltPassoGrau = 0.0;
double ResolucaoeixoAzGrausDecimal = 0.0, ResolucaoeixoAzPassoGrau = 0.0;
double RAmount = 0.0;
double DECmount = 0.0;
double AZmount = 0.0;
double ALTmount = 0.0;
double AZmountAlvo = 0.1;
double ALTmountAlvo = 0.1;


double DistanciaTotal = 0.0;

//Alvos a serem seguidos ou sincronizado
double RAAlvo = 0.0;
double DECAlvo = 0.0;
double AzAlvo = 0.0;
double AltAlvo = 0.0;
double ErroAlt = 0.0;
double ErroAz = 0.0;


int statusmovimentacao = 0;
int ativaacom = 0;
int gotomount = 0;
//limites da montagem
int HorizonteLimite = 0;
int AltitudeLimite = 90;

//Ajuste fino do tempo
int Segundo;
double Microssegundo = 0 , SegundoFracao = 0.0, MilissegundoSeg = 0.0, MilissegundoI = 0.0;



void setup() {
  
  //Pinos Led RGB
  pinMode(LedR, OUTPUT);
  pinMode(LedG, OUTPUT);
  pinMode(LedB, OUTPUT);
  digitalWrite(LedR, ledStateR);
  digitalWrite(LedB, ledStateB);
  digitalWrite(LedG, ledStateG);
  //Pinos Joystick
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(kPin, INPUT_PULLUP);
  
  



  if (ledStateR == LOW) {
    ledStateR = HIGH;
  } else {
    // ledStateR = LOW;
  }
  digitalWrite(LedR, ledStateR);

  Serial.begin(9600);
  //Serial1.begin(9600);
  //Serial2.begin(9600);
  //Wire1.begin();
  
  //EEPROM.begin(1);

  /* Flash is erased every time new code is uploaded. Write the default configuration to flash if first time */
  // running for the first time?
  uint8_t codeRunningForTheFirstTime;
  EEPROM_readAnything(0, codeRunningForTheFirstTime);
  Serial.print("Primeira Execucao: ");
  if (codeRunningForTheFirstTime) {
    Serial.println("yes");
    /* OK first time running, set defaults */
    configuration.Reducao = dReducao;
    configuration.MaxPassoAlt = dMaxPassoAlt;
    configuration.MaxPassoAz = dMaxPassoAz;
    configuration.MinTimer = dMinTimer;
    configuration.latitude = -25.40;;
    configuration.longitude = -49.20;
    configuration.SentidoDEC = 0;
    configuration.SentidoRA = 0;
    setTime(00, 00, 00, 01, 01, 2021);
    MilissegundoSeg = second();
    configuration.DataHora = now();
    configuration.UTC = -2;
    strcpy (configuration.Local, "Minha Casa");
    // write configuration struct to flash at adress 4
    byte b2[sizeof(Configuration)]; // create byte array to store the struct
    memcpy(b2, &configuration, sizeof(Configuration)); // copy the struct to the byte array
    EEPROM_writeAnything(4,&configuration);
    EEPROM_writeAnything(4,b2);
    // write 0 to address 0 to indicate that it is not the first time running anymore
    EEPROM_writeAnything(0, 0);
  }
  else {
    Serial.println("no");
  }


  
  //byte* b;// EEPROM.read(4); // byte array which is read from flash at adress 4
  EEPROM_readAnything(4,configurationFromFlash);
  //memcpy(&configurationFromFlash, b, sizeof(Configuration)); // copy byte array to temporary struct
  Serial.println(">>>>>>>>>>>>>>>configurationFromFlash.Reducao:");
  Serial.println(configurationFromFlash.Reducao);
  Reducao = 16;//configurationFromFlash.Reducao;
  if(Reducao==32) {
    //AltaM2 = HIGH;
    AltaM1 = LOW;
    AltaM0 = HIGH;
  }
  if(Reducao==16) {
    //AltaM2 = HIGH;
    AltaM1 = LOW;
    AltaM0 = LOW;
  }
  if(Reducao==8) {
    //AltaM2 = LOW;
    AltaM1 = HIGH;
    AltaM0 = HIGH;
  }
  if(Reducao==4) {
    //AltaM2 = LOW;
    AltaM1 = HIGH;
    AltaM0 = LOW;
  }
  if(Reducao==2) {
    //AltaM2 = LOW;
    AltaM1 = LOW;
    AltaM0 = HIGH;
  }
  MaxPassoAlt = configurationFromFlash.MaxPassoAlt;
  MaxPassoAz = configurationFromFlash.MaxPassoAz;
  dReducao = Reducao;
  dMaxPassoAlt =   MaxPassoAlt;
  dMaxPassoAz =  MaxPassoAz;
  MinTimer = configurationFromFlash.MinTimer;
  latitude = configurationFromFlash.latitude;
  longitude = configurationFromFlash.longitude;
  UTC = configurationFromFlash.UTC;
  SentidoDEC = configurationFromFlash.SentidoDEC;
  SentidoRA = configurationFromFlash.SentidoRA;
  setTime(configurationFromFlash.DataHora);
  delay (1000);
  Serial.print("Bem vindo ao FIREGOTO para setup inicial digitar :HSETUPON# \n");
  delay (2000);

  
  IniciaMotores();
  SentidodosMotores();
  SerialPrint("00:00:00#"); //RTA para leitura do driver ASCOM da MEADE autostar I
  delay (200);
  previousMillis = millis();
  PrimeiroCommanMillis = previousMillis;
  ErroAlt = ErroAz = 44.0;
  ResolucaoeixoAltGrausDecimal = 360.0 / MaxPassoAlt ;
  ResolucaoeixoAzGrausDecimal = 360.0 / MaxPassoAz ;
  ResolucaoeixoAltPassoGrau = (MaxPassoAlt  / 360.0);
  ResolucaoeixoAzPassoGrau = (MaxPassoAz  / 360.0);
  //Instruções do LCD
  //lcd.init();
 // lcd.begin(); 
 // lcd.backlight();
 // lcd.clear();
}



void loop() {
   AltMotor.moveTo((int)5000);
  AzMotor.moveTo((int)5000);
  AltMotor.runSpeed();
    AzMotor.runSpeed();
  if (ledStateR == LOW) {
    ledStateR = HIGH;
  } else {
    // ledStateR = LOW;
  }
  currentMillis = millis();
  CalcPosicaoPasso();
  //if (Serial2.available() || Serial.available() || Serial1.available()) serialEvent();

  if ((numCommand != numCommandexec) && (flagCommand == 0))
  {
    SerialPrintDebug(String(numCommandexec));
    SerialPrintDebug(String(numCommand));
    cmdComplete = true;
    executecommand();
    SerialPrintDebug(String(numCommandexec));
    SerialPrintDebug(String(numCommand));
  }
  if ( setupflag == 0 )
  {
    if (PrimeiroCommanMillis < currentMillis)

    {
      PrintLocalHora();
      SerialPrintDebug(String(Hora2DecHora(hour(), minute(), SegundoFracao), 10)) ;
      PrimeiroCommanMillis = PrimeiroCommanMillis + 1001;
    }
  }
  if ( setupflag == 1 )
  {
    RotinadeSetup();
  }
  if (calculaRADECmountMillis < currentMillis)
  {
    calculaRADECmountMillis = calculaRADECmountMillis + 113;
    calculaRADECmount();
  }

  AlteraMicroSeg();
  if (previousMillisLed < currentMillis)
  {
    previousMillisLed = currentMillis + 150;
    digitalWrite(LedR, ledStateR);
    digitalWrite(LedB, ledStateB);
    digitalWrite(LedG, ledStateG);
    ledStateB = LOW;
  }


  // print the string when a newline arrives:
  // protegemount();
  ativaacom =1;
  if (ativaacom != 0)
  {
    if (previousMillis < currentMillis)
    {
      setaccel();
      previousMillis = currentMillis + 237;
      DistanciaTotal = abs(AltMotor.distanceToGo()) + abs(AzMotor.distanceToGo());
      if ( MaxPassoAlt == dMaxPassoAlt)
      {
        if ( DistanciaTotal > (MaxPassoAz / 90))
        {
          BaixaResolucao();
        }
      }
      else
      {
        if ( DistanciaTotal < (MaxPassoAz / 180))
        {
          AltaResolucao();
        }
      }
      SerialPrintDebug("Distancia Total: ");
      SerialPrintDebug(String(DistanciaTotal));
      SerialPrintDebug(String(MaxPassoAz));
    }
    SetPosition();
    acompanhamento();
  }
  else
  {
    previousMillis = millis();
  }
  AlteraMicroSeg();
  controlJoystick();
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE){
    portENTER_CRITICAL(&timerMux);
    callRunMotor = false;
    portEXIT_CRITICAL(&timerMux);
    runmotor();
  }
  //menu();
}
