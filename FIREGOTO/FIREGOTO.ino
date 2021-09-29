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

#define TMC2209
//#define DRV8825

#define BLUETOOTH
//#define WIFI
#include <AccelStepper.h>
#include <Arduino.h>
#include <math.h>
#include <Time.h>
#include <TimeLib.h>
#include <Wire.h>
//#include <LiquidCrystal_I2C.h>


#ifdef __arm__
#include "pinMapDUE.h"
#ifdef TMC2209
#include "soft_uart.h"
/*SoftwareSerial para Arduino DUE*/
using namespace soft_uart;
using namespace soft_uart::arduino_due;
#define SOFT_UART_BIT_RATE  57600 // 57600 38400 1200 19200 9600 115200 300
#define RX_BUF_LENGTH 256 // software serial port's reception buffer length
#define TX_BUF_LENGTH 256 // software serial port's transmision buffer length
#define RECEPTION_TIMEOUT 100 // milliseconds


serial_tc7_declaration(RX_BUF_LENGTH, TX_BUF_LENGTH);
serial_tc8_declaration(RX_BUF_LENGTH, TX_BUF_LENGTH);
auto& SerialAz = serial_tc7; // serial_tc4_t& serial_obj=serial_tc4;
auto& SerialAlt = serial_tc8; // serial_tc3_t& serial_obj=serial_tc3;
SerialAz.begin(
  SW_RX_SERIAL_AZ,
  SW_TX_SERIAL_AZ,
  SOFT_UART_BIT_RATE,
  soft_uart::data_bit_codes::EIGHT_BITS,
  soft_uart::parity_codes::EVEN_PARITY,
  //soft_uart::stop_bit_codes::ONE_STOP_BIT
  soft_uart::stop_bit_codes::TWO_STOP_BITS
);

SerialAlt.begin(
  SW_RX_SERIAL_ALT,
  SW_TX_SERIAL_ALT,
  SOFT_UART_BIT_RATE,
  soft_uart::data_bit_codes::EIGHT_BITS,
  soft_uart::parity_codes::EVEN_PARITY,
  //soft_uart::stop_bit_codes::ONE_STOP_BIT
  soft_uart::stop_bit_codes::TWO_STOP_BITS
);
#endif
#endif
#ifdef ESP32
#include "pinMapESP32.h"
#ifdef BLUETOOTH
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
#endif
#ifdef WIFI
#include <WiFi.h>
WiFiClient curClient;
#endif
boolean confirmRequestPending = false;
#ifdef TMC2209
#include <SoftwareSerial.h>
SoftwareSerial SerialAz(SW_RX_SERIAL_AZ, SW_TX_SERIAL_AZ);
SoftwareSerial SerialAlt(SW_RX_SERIAL_ALT, SW_TX_SERIAL_ALT);

#include <TMCStepper.h>
#define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2
#define R_SENSE 0.11f //Este valor deve ser alterado de acordo com o Resistor "R_SENSE" da breakout board (Fysetc, Watterott, etc...), consultar datasheet.
TMC2209Stepper driverAz(&SerialAz, R_SENSE, DRIVER_ADDRESS);
TMC2209Stepper driverAlt(&SerialAlt, R_SENSE, DRIVER_ADDRESS);
#endif
#endif


//DEBUG
int flagDebug = 0;

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
bool moveRADEC;


//LiquidCrystal_I2C lcd(0x27, lcd_pin1, lcd_pin2);




AccelStepper AltMotor(AccelStepper::DRIVER, MotorALT_Passo, MotorALT_Direcao);
AccelStepper AzMotor(AccelStepper::DRIVER, MotorAZ_Passo, MotorAZ_Direcao);
int accel = 350;


int ledStateR = LOW;
int ledStateB = LOW;
int ledStateG = LOW;


/*valores maximo para o passo (Valor ideal 1286400)*/
double dMaxPassoAlt = 4659200; /* //valor de resolucao AR = Passo * MicroPasso * reducao ex(200*16*402)/4    (16*200*(117/11)*56)*/
double dMaxPassoAz = 4659200; /*/valor de resolucao AR = Passo * MicroPasso * reducao ex(200*16*402)   (16*200*(118/11)*57)*/
int dMinTimer = 200; /*/passo*/
double dMaxSpeedAlt = 400;
double dMaxSpeedAz = 400;
int dReducao = 32;


//Variaveis de persistencia e estrutura de dados ----------------------------------------------------------------------------------------------------------------
//DueFlashStorage dueFlashStorage;

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

int Reducao;
int MaxPassoAlt;
int MaxPassoAz;
int MaxSpeedAz = dMaxSpeedAz;
int MaxSpeedAlt = dMaxSpeedAlt;
int SentidoDEC = 0;
int SentidoRA = 0;
int MinTimer;
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

  Serial1.begin(19200);
  Serial2.begin(19200);
  Serial.begin(9600);
#ifdef __arm__
  Serial3.begin(9600);
  SerialUSB.begin(9600);
#endif
#ifdef ESP32
#ifdef BLUETOOTH
  iniciaBluetooth();
#endif
#ifdef WIFI
  iniciaWiFi();
#endif
#endif
  delay(2000);
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

  Wire1.begin();
#ifdef TMC2209
  Serial.println("Chamando configurarTMC2209");
  configurarTMC2209();
#endif
  /* Flash is erased every time new code is uploaded. Write the default configuration to flash if first time */
  // running for the first time?
  uint8_t codeRunningForTheFirstTime = 255;//dueFlashStorage.read(0); // flash bytes will be 255 at first run
  Serial.print("Primeira Execucao: ");
  if (codeRunningForTheFirstTime) {
    Serial.println("yes");
    /* OK first time running, set defaults */
    configuration.Reducao = dReducao;
    configuration.MaxPassoAlt = dMaxPassoAlt;
    configuration.MaxPassoAz = dMaxPassoAz;
    configuration.MinTimer = dMinTimer;
    configuration.latitude = -23.3327977;
    configuration.longitude = -51.1813004;
    configuration.SentidoDEC = 0;
    configuration.SentidoRA = 0;
    setTime(00, 00, 00, 01, 01, 2021);
    MilissegundoSeg = second();
    configuration.DataHora = now();
    configuration.UTC = -3;
    //strcpy (configuration.Local, "Minha Casa");
    configurationFromFlash = configuration;
    // write configuration struct to flash at adress 4
    //byte b2[sizeof(Configuration)]; // create byte array to store the struct
    //memcpy(b2, &configuration, sizeof(Configuration)); // copy the struct to the byte array
    //    dueFlashStorage.write(4, b2, sizeof(Configuration)); // write byte array to flash
    // write 0 to address 0 to indicate that it is not the first time running anymore
    //    dueFlashStorage.write(0, 0);
  }
  else {
    Serial.println("no");
  }

  //byte* b = dueFlashStorage.readAddress(4); // byte array which is read from flash at adress 4
  //memcpy(&configurationFromFlash, b, sizeof(Configuration)); // copy byte array to temporary struct
  Reducao = configurationFromFlash.Reducao;
#ifdef DRV8825
  if (Reducao == 32) {
    AltaM2 = HIGH;
    AltaM1 = LOW;
    AltaM0 = HIGH;
  }
  if (Reducao == 16) {
    AltaM2 = HIGH;
    AltaM1 = LOW;
    AltaM0 = LOW;
  }
  if (Reducao == 8) {
    AltaM2 = LOW;
    AltaM1 = HIGH;
    AltaM0 = HIGH;
  }
  if (Reducao == 4) {
    AltaM2 = LOW;
    AltaM1 = HIGH;
    AltaM0 = LOW;
  }
  if (Reducao == 2) {
    AltaM2 = LOW;
    AltaM1 = LOW;
    AltaM0 = HIGH;
  }
#endif
#ifdef TMC2209
  driverAlt.microsteps(Reducao);
  driverAz.microsteps(Reducao);
#endif
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
  Serial.print("Bem vindo p/ setup :HSETUPON# \n");
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

  //lcd.begin(Wire1);
  //lcd.backlight();
  //lcd.clear();
  /*
    uint32_t Freq = 0;
    Freq = getCpuFrequencyMhz();
    Serial.print("CPU Freq = ");
    Serial.print(Freq);
    Serial.println(" MHz");
    Freq = getXtalFrequencyMhz();
    Serial.print("XTAL Freq = ");
    Serial.print(Freq);
    Serial.println(" MHz");
    Freq = getApbFrequency();
    Serial.print("APB Freq = ");
    Serial.print(Freq);
    Serial.println(" Hz");*/


}



void loop() {

  currentMillis = millis();

  if (ledStateR == LOW) {
    ledStateR = HIGH;
  } else {
    // ledStateR = LOW;
  }

  CalcPosicaoPasso();
#ifdef __arm__
  if (SerialUSB.available() || Serial3.available()) serialEvent();
#endif
#ifdef BLUETOOTH
  if (SerialBT.available()) serialEvent();
#endif
  if (Serial.available() ||  Serial1.available() ||  Serial2.available()) serialEvent();
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
      //PrintLocalHora();
      //SerialPrintDebug(String(Hora2DecHora(hour(), minute(), SegundoFracao), 10)) ;
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
  //controlJoystick();
  //menu();
  //  }
}
#ifdef TMC2209

void configurarTMC2209() {

  /* Iniciando configuração TMC2209_UART */
  /*Definindo endereço dos drivers como 0b00, os estão com o mesmo endereço pois estão em Seriais diferente,
    se compartilhar a mesma Serial, os endereços devem ser único para cada driver*/
  SerialAz.begin(38400);
  SerialAlt.begin(38400);
  pinMode(TMC2209_ALT_MS1, OUTPUT);
  pinMode(TMC2209_ALT_MS2, OUTPUT);
  pinMode(TMC2209_AZ_MS1, OUTPUT);
  pinMode(TMC2209_AZ_MS2, OUTPUT);

  digitalWrite(TMC2209_ALT_MS1, LOW);
  digitalWrite(TMC2209_ALT_MS2, LOW);
  digitalWrite(TMC2209_AZ_MS1, LOW);
  digitalWrite(TMC2209_AZ_MS2, LOW);
  /*Endereço drivers definidos*/


  /* Iniciando SoftwareSerial
    TX=> MotorXX_Reset
    RX=> MotorXX_M2
  */


  /*Configurando motores*/
  driverAz.begin();                 //  SPI: Init CS pins and possible SW SPI pins
  driverAz.pdn_disable(true);       // UART: Init SW UART (if selected) with default 115200 baudrate
  driverAz.toff(4);                 // Enables driver in software
  driverAz.rms_current(800);        // Set motor RMS current
  driverAz.blank_time(24);

  driverAz.microsteps(dReducao);
  driverAz.TPWMTHRS(1000);
  driverAz.en_spreadCycle(0); // Ativa modo SpreadCycle, menos silencioso, porém possibilita maiores rotações sem perder passos.
  driverAz.pwm_autoscale(true);     // Needed for stealthChop


  driverAlt.begin();                 //  SPI: Init CS pins and possible SW SPI pins
  driverAlt.pdn_disable(true);       // UART: Init SW UART (if selected) with default 115200 baudrate
  driverAlt.toff(5);                 // Enables driver in software
  driverAlt.rms_current(800);        // Set motor RMS current
  driverAlt.microsteps(dReducao);
  //driverAlt.TPWMTHRS(1000);
  driverAlt.en_spreadCycle(0); // Ativa modo SpreadCycle, menos silencioso, porém possibilita maiores rotações sem perder passos.
  driverAlt.pwm_autoscale(true);     // Needed for stealthChop

  if (driverAlt.version() != 0x21) {
    Serial.println("Drv X uart issue");
  }
  if (driverAz.version() != 0x21) {
    Serial.println("Drv Y uart issue");
  }

}
#endif
