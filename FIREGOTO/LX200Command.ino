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



void executecommand()
{
  flagCommand = 1;
  SerialPrintDebug(String(Command[numCommandexec][0])) ;
  SerialPrintDebug(String(Command[numCommandexec])) ;
  
  if (cmdComplete) {
    PrimeiroCommanMillis = currentMillis + 5000;
    addbackslash();
    if(Command[numCommandexec][1] !='G' && (Command[numCommandexec][2] !='R' || Command[numCommandexec][2] !='D')){
      Serial.println(String(Command[numCommandexec]));
    }
    if (Command[numCommandexec][0] != ':')
    {
      if (Command[numCommandexec][0] == 0x06)
      {
        SerialPrint("A");
        /*
          ACK <0x06> Query of alignment mounting mode.
          Returns:
          A If scope in AltAz Mode
          D If scope is currently engaged by the Autostar Updater Program [Autostar and Autostar II telescopes]
          L If scope in Land Mode
          P If scope in Polar Mode */
        cmdComplete = false;
      }
      else {
        // printerror();
        cmdComplete = false;
      }
    }
    if (cmdComplete) {
      if (Command[numCommandexec][1] == '$') {
        switch (Command[numCommandexec][2]) {
          case 'B':
            switch (Command[numCommandexec][3]) {
              case 'D':
                setDECbacklash();
                break;
              case 'R':
                setRAbacklash();
                break;
            }

        }
      }
      if (Command[numCommandexec][1] == 'C') {
        switch (Command[numCommandexec][2]) {
          case 'S':
            synctelescope();
            break;
          case 'M':
            synctelescopeString();
            break;

        }
      }
      if (Command[numCommandexec][1] == 'G') {
        switch (Command[numCommandexec][2]) {
          case 'A':
            printALTmount();
            break;
          case 'C':
            printDataLocal();
            break;
          case 'D':
            printDECmount();
            break;
          case 'g':
            printlongitude();
            break;
          case 'G':
            printUTC();
            break;
          case 'h':
            printAlturaLimite();
            break;
          case 'L':
            PrintLocalHora();
            break;
          case 'M':
            printObservatorioNome();
            break;
          case 'o':
            printHorizonteLimite();
            break;
          case 'R':
            printRAmount();
            break;
          case 'S':
            printSideralHora();
            break;
          case 't':
            printlatitude();
            break;
          case 'T':
            printSIderalRate();
            break;
          case 'U':
            printOperatocao();
            break;
          case 'W':
            printalinhamento();
            break;
          case 'Z':
            printAZmount();
            break;
          case 'V':
            switch (Command[numCommandexec][3]) {
              case 'D':
                printFirmwareDate();
                break;
              case 'F':
                printFirmware();
                break;
              case 'N':
                printFirmwareNumber();
                break;
              case 'P':
                printFirmwareNamne();
                break;
              case 'T':
                printFirmwareTime();
                break;
            }
            break;
        }
      }

      if (Command[numCommandexec][1] == 'H') //set Hardware
      {
        switch (Command[numCommandexec][2]) {
          case 'S':
            if (Command[numCommandexec][3] == 'd')//:HSd#Off DEBUG
            {
              flagDebug = 0;
            }
            if (Command[numCommandexec][3] == 'D')//:HSD#On DEBUG
            {
              flagDebug = 1;
            }
            if (Command[numCommandexec][4] == 'D') //:HSRD00#
            {
              setReducao();
            }
            if (Command[numCommandexec][4] == 'L') //:HSAL0000000#
            {
              setMaxPassoAlt();
            }
            if (Command[numCommandexec][4] == 'Z') //:HSAZ0000000#
            {
              setMaxPassoAz();//:HSRB0000000#
            }
            if (Command[numCommandexec][3] == 'T') //:HST0000000#
            {
              setMinTimer();
            }
            if (Command[numCommandexec][8] == 'N') //:HSETUPON#
            {
              RotinadeSetup();
            }
            if (Command[numCommandexec][8] == 'F') //:HSETUPOFF#
            {
              RotinadeSetupOff();
            }
            if (Command[numCommandexec][5] == 'A') //:HSSRA0#
            {
              setSentidoRA(); //:HSSRA0#
            }
            if (Command[numCommandexec][5] == 'E') //:HSSDEC0#
            {
              setSentidoDEC(); //:HSSDEC0#
            }
            break;

          case 'G':
            if (Command[numCommandexec][4] == 'A')//:HGRA#
            {
              getMaxPassoAlt();
            }
            if (Command[numCommandexec][4] == 'B')//:HGRB#
            {
              getMaxPassoAz();//:HGRB#
            }
            if (Command[numCommandexec][3] == 'T')//:HGT#
            {
              getMinTimer(); //:HGT#
            }
            break;
        }
      }

      if (Command[numCommandexec][1] == 'M') {
        switch (Command[numCommandexec][2]) {
          case 'S':
            gototeleEQAR();
            break;
          case 'M':
            synctelescopeString();
            break;
          case 's':
            movesul();
            break;
          case 'w':
            moveoeste();
            break;
          case 'n':
            movenorte();
            break;
          case 'e':
            moveleste();
            break;
        }
      }
      /* Set rate to Guide 	:RG# 	Reply: [none]
        Set rate to Centering 	:RC# 	Reply: [none]
        Set rate to Move 	:RM# 	Reply: [none]
        Set rate to Slew 	:RS# 	Reply: [none]
        Set rate to n (1-9)*3	:Rn# 	Reply: [none]   */
      if (Command[numCommandexec][1] == 'R') {
        MoveRate();
      }

      if (Command[numCommandexec][1] == 'S') {
        switch (Command[numCommandexec][2]) {
          case 'C':
            setLocalData();
            break;
          case 'd':
            setDECAlvo();
            break;
          case 'G':
            setHoraparaUTC();
            break;
          case 'g':
            setlongitude();
            break;
          case 'h':
            setAlturaLimite();
            break;
          case 'L':
            setLocalHora();
            break;
          case 'M':
            setObservatorioNome(); // Set site 0 name 	:SMsss...#
            break;
          case 'o':
            setHorizonteLimite();
            break;
          case 'r':
            setRAAlvo();
            break;
          case 't':
            setlatitude();
            break;
          case 'T':
            Setsidereal();
            break;
          case 'w':
            setBufferGps();
            break;
        }
        /*
          :U# Toggle between low/hi precision positions
          Low - RA displays and messages HH:MM.T sDD*MM
          High - Dec/Az/El displays and messages HH:MM:SS sDD*MM:SS
          Returns Nothing */
      }
      if (Command[numCommandexec][1] == 'U') {
        SerialPrint("00:00:00#");
      }

      if (Command[numCommandexec][1] == 'Q') {
        Stoptelescope();
        switch (Command[numCommandexec][2]) {
          case 's':
            parasul();
            break;
          case 'w':
            paraoeste();
            break;
          case 'n':
            paranorte();
            break;
          case 'e':
            paraleste();
            break;
        }
      }


    }
  }
  if (statusmovimentacao != 0 )
  {
    gototeleEQAR ();
  }

  for (int j = 0; j < 15; j++) {
    Command[numCommandexec][j] = ' ';
  }
  //Passa para o proximo
  numCommandexec = numCommandexec  + 1;
  if (numCommandexec > 14 )
  {
    numCommandexec = 0;
  }

  flagCommand = 0;
  delay(1);
}

void printerror()
{
  SerialPrint("!#");
  //Serial.println(Command[numCommandexec][pontBuffer]);
}

void printHelp()
{
  Serial.println("Set date 	:SC MM/DD/YY#	Reply: 0 or 1#");
  //  Serial.println("Get date 	:GC# 	 Reply: MM/DD/YY#");
}






void printUTC() //:GG# Get UTC offset time Returns: sHH# or sHH.H#
{
  char str[3];
  int tmp = abs(UTC);
  if (UTC < 0) {
    sprintf(str, "-%02d#", int(tmp));
  } else {
    sprintf(str, "+%02d#", int(tmp));
  }
  SerialPrint(str);

}
void setLocalData() //:SCMM/DD/YY# Change Handbox Date to MM/DD/YY #:SC 03/20/14#
{
  String str = "";
  str += Command[numCommandexec][3];
  str += Command[numCommandexec][4];
  int mes = str.toInt();
  str = "";
  str += Command[numCommandexec][6];
  str += Command[numCommandexec][7];
  int dia = str.toInt();
  str = "";
  str += Command[numCommandexec][9];
  str += Command[numCommandexec][10];
  int ano = str.toInt();
  str = "";
  ano = ano + 2000;
  int tmp = UTC * 60 * 60;
  adjustTime(-tmp);
  int HH = hour();
  int MM = minute();
  int SS = second();
  setTime(HH, MM, SS, dia, mes, ano);
  adjustTime(tmp);
  SerialPrint("1");
  configurationFromFlash.DataHora =  now();
  byte b2[sizeof(Configuration)]; // create byte array to store the struct
  memcpy(b2, &configurationFromFlash, sizeof(Configuration)); // copy the struct to the byte array
  //  dueFlashStorage.write(4, b2, sizeof(Configuration)); // write byte array to flash
}

void printDataLocal() //Get date 	:GC# 	 Reply: MM/DD/YY#
{
  int dia = day();
  int mes = month();
  int ano = (year() - 2000);
  char str[10];
  sprintf(str, "%02d/%02d/%02d#", int(mes), int(dia), int(ano));
  SerialPrint(str);

}

void setLocalHora()//:SLHH:MM:SS#  Set the local Time
{
  String str = "";
  str += Command[numCommandexec][3];
  str += Command[numCommandexec][4];
  int HH = str.toInt();
  str = "";
  str += Command[numCommandexec][6];
  str += Command[numCommandexec][7];
  int MM = str.toInt();
  str = "";
  str += Command[numCommandexec][9];
  str += Command[numCommandexec][10];
  int SS = str.toInt();
  str = "";
  int dia = day();
  int mes = month();
  int ano = year();
  setTime(HH, MM, SS, dia, mes, ano);
  int tmp = UTC * 60 * 60;
  adjustTime(-tmp);
  SerialPrint("1");
  configurationFromFlash.DataHora =  now();
  byte b2[sizeof(Configuration)]; // create byte array to store the struct
  memcpy(b2, &configurationFromFlash, sizeof(Configuration)); // copy the struct to the byte array
  //  dueFlashStorage.write(4, b2, sizeof(Configuration)); // write byte array to flash
}

void PrintLocalHora()//:Get time (Local) 	:GLHH:MM:SS#	Reply: HH:MM:SS#
{
  int hhl = int(hour());
  if (hhl > 23)
  {
    hhl = hhl - 24;
  }
  if (hhl < 0)
  {
    hhl = hhl + 24;
  }


  char str[10];
  sprintf(str, "%02d:%02d:%02d#", hhl, int(minute()), int(second()));
  SerialPrint(str);
}

void printFirmware() //Get firmware 	:GVF# 	Reply: "ETX Autostar"
{
  SerialPrint("ETX Autostar|A|43Eg|Apr 03 2007@11:25:53#");
  //SerialPrint("Version 2.0i");
}


void printOperatocao()
{
  SerialPrint("N#");
}



void printalinhamento() //:GW# with "AN0#"
{
  SerialPrint("AN0#");
}
/*std Autostars return "Autostar"
  LX200s return "LX2001"
  RCXs return "RCX400"
  LSs returned "ETX-LS" and now just "LS"
  LS/AutostarIII GVF response is (currently) only the firmware version: "1.6#"
  (the Autostar III responds to a :GW# with "AN0#" (if not aligned or tracking))
  After reading the excellent info on http://www.weasner.com   I learn that : " when typing a single control-F on the terminal  the Autostar should echo an "A" or "P" depending upon whether it's
  set up as Alt/Az or Polar mount status and when typing #:GVF#  it should respond with its identification string, including the word Autostar and a time and date (when that firmware version
  was built at Meade) ".
  Indeed  :
  Ctrl -F    ---> A
  #:GVF#   ---> #Autostar|A|12Ea #
*/

void  setBufferGps()
{
  SerialPrint("1");
}


void printSideralHora()
{
  double horadec = Hora2DecHora(hour(), minute(), second()) ;
  //Serial.println(horadec,10);
  double jdia = JulianoDay (year(), month(), day(), horadec) ;
  //Serial.println((jdia+2451545.0),10);
  double HST = HoraSideral(jdia);
  while (HST > 24.0) {
    HST = HST - 24.0;
  }
  char str[10];
  sprintf(str, "%02d:%02d:%02d#", int(DecHour2HoursHH(HST)), int(DecHour2HoursMM(HST)), int(DecHour2HoursSEC(HST)));
  SerialPrint(str);
}


/*:GZ# Get telescope azimuth
  Returns: DDD*MM#T or DDD*MM'SS#*/
void printAZmount()
{
  char str[10];
  sprintf(str, "%03d*%02d*%02d#", int(DecDegtoDeg(eixoAzGrausDecimal)), int(DecDegtoMin(eixoAzGrausDecimal)), int(DecDegtoSec(eixoAzGrausDecimal)));
  str[6] = 223;
  SerialPrint(str);
}

void printALTmount() //:GA# Get Telescope Altitude Returns: sDD*MM# or sDD*MM'SS#
{
  char str[9];
  int Ddeg, Min, Sec;
  Ddeg = (int)DecDegtoDeg(eixoAltGrausDecimal);
  Min = (int)DecDegtoMin(eixoAltGrausDecimal);
  Sec = (int)DecDegtoSec(eixoAltGrausDecimal);
  if (Ddeg < 0) {
    sprintf(str, "-%02d*%02d:%02d#", int(Ddeg), int(Min), int(Sec));
  } else {
    sprintf(str, "+%02d*%02d:%02d#", int(Ddeg), int(Min), int(Sec));
  }
  SerialPrint(str);

}

void printRAmount() //:GR# Get Telescope RA Returns: HH:MM.T# or HH:MM:SS#
{
  int HH = DecDeg2HoursHH(RAmount);
  int MM = DecDeg2HoursMM(RAmount);
  int SS = DecDeg2HoursSEC(RAmount);
  char str[9];
  sprintf(str, "%02d:%02d:%02d#", int(HH), int(MM), int(SS));
  SerialPrint(str);
}

void printDECmount() //:GD# Get Telescope Declination. Returns: sDD*MM# or sDD*MM'SS#
{
  int Ddeg = abs((int)DecDegtoDeg(DECmount));
  int Min = abs((int)DecDegtoMin(DECmount));
  int Sec = abs((int)DecDegtoSec(DECmount));
  char str[10];
  if (DECmount < 0) {
    sprintf(str, "-%02d*%02d:%02d#", int(Ddeg), int(Min), int(Sec));
  } else {
    sprintf(str, "+%02d*%02d:%02d#", int(Ddeg), int(Min), int(Sec));
  }
  SerialPrint(str);
  
  
}

void setlatitude() //:StsDD*MM# Sets the current site latitude to sDD*MM# Returns: 0 ÃƒÂ¢Ã¢â€šÂ¬Ã¢â‚¬Å“ Invalid 1 - Valid
{

  String str = "";
  str += Command[numCommandexec][4];
  str += Command[numCommandexec][5];
  int DD = str.toInt();
  str = "";
  str += Command[numCommandexec][7];
  str += Command[numCommandexec][8];
  int MM = str.toInt();
  str = "";
  if (Command[numCommandexec][3] == '-')
  {
    DD = DD * (-1);
  }

  latitude = DegMinSec2DecDeg(DD, MM, 0.0);
  configurationFromFlash.latitude = latitude;
  SerialPrint("1");
  byte b2[sizeof(Configuration)]; // create byte array to store the struct
  memcpy(b2, &configurationFromFlash, sizeof(Configuration)); // copy the struct to the byte array
  //  dueFlashStorage.write(4, b2, sizeof(Configuration)); // write byte array to flash
}

void printlatitude()// :Gt# Get Current Site Latitude Returns: sDD*MM# The latitude of the current site. Positive inplies North latitude.
{

  int Ddeg = abs((int)DecDegtoDeg(latitude));
  int Min = abs((int)DecDegtoMin(latitude));
  char str[7];
  if (latitude < 0) {
    sprintf(str, "-%02d*%02d#", int(Ddeg), int(Min));
  } else {
    sprintf(str, "+%02d*%02d#", int(Ddeg), int(Min));
  }
  SerialPrint(str);
}

void setlongitude() //:SgsDDD*MM# Set current site's longitude to DDD*MM an ASCII position string Returns: 0 ÃƒÂ¢Ã¢â€šÂ¬Ã¢â‚¬Å“ Invalid 1 - Valid
//NO PROTOCOLO DA MEADE O SINAL ÃƒË† AO CONTRARIO
{
  String str = "";
  str += Command[numCommandexec][4];
  str += Command[numCommandexec][5];
  str += Command[numCommandexec][6];
  int DD = str.toInt();
  str = "";
  str += Command[numCommandexec][8];
  str += Command[numCommandexec][9];
  int MM = str.toInt();
  str = "";
  if (Command[numCommandexec][3] != '-')
  {
    DD = DD * (-1);
  }
  longitude = DegMinSec2DecDeg(DD, MM, 0.0);
  configurationFromFlash.longitude = longitude;
  SerialPrint("1");
  byte b2[sizeof(Configuration)]; // create byte array to store the struct
  memcpy(b2, &configurationFromFlash, sizeof(Configuration)); // copy the struct to the byte array
  //  dueFlashStorage.write(4, b2, sizeof(Configuration)); // write byte array to flash
}

void printlongitude() // Get Current Site Longitude Returns: sDDD*MM#

{

  int Ddeg = abs((int)DecDegtoDeg(longitude));
  int Min = abs((int)DecDegtoMin(longitude));
  char str[8];
  if (longitude < 0) {
    sprintf(str, "-%03d*%02d#", int(Ddeg), int(Min));
  } else {
    sprintf(str, "+%03d*%02d#", int(Ddeg), int(Min));
  }
  SerialPrint(str);

}

void calculaAZALTmount() {
  double horadec = Hora2DecHora(hour(), minute(), SegundoFracao) ;
  double jdia = JulianoDay (year(), month(), day(), horadec) ;
  double HST = HoraSideral(jdia);
  double HSL = HoraSiderallocal(longitude, HST) ;
  Radec2Azalt(HSL, latitude, RAmount, DECmount, &eixoAzGrausDecimal, &eixoAltGrausDecimal);
}



void calculaRADECmount() {

  double horadec = Hora2DecHora(hour(), minute(), SegundoFracao) ;
  double jdia = JulianoDay (year(), month(), day(), horadec) ;
  double HST = HoraSideral(jdia);
  double HSL = HoraSiderallocal(longitude, HST) ;
  Azalt2Radec(latitude, longitude, eixoAzGrausDecimal, eixoAltGrausDecimal, &RAmount, &DECmount, HSL);
}

void calculaalvoRADECmount() {
  double horadec = Hora2DecHora(hour(), minute(), SegundoFracao) ;
  double jdia = JulianoDay (year(), month(), day(), horadec) ;
  double HST = HoraSideral(jdia);
  double HSL = HoraSiderallocal(longitude, HST) ;
  Azalt2Radec(latitude, longitude, eixoAzGrausDecimal, eixoAltGrausDecimal, &RAAlvo, &DECAlvo, HSL);
}

void printFirmwareDate() //Get firmware date 	:GVD# 	Reply: MM DD YY#
{
  int dia = day();
  int mes = month();
  int ano = year();

  if (mes < 10) {
    SerialPrint("0");
  }
  SerialPrint(String(mes));
  if (dia < 10) {
    SerialPrint("0");
  }
  SerialPrint(String(dia));
  ano = ano - 2000;
  if (ano < 10) {
    SerialPrint("0");
  }
  SerialPrint(String(ano));
  SerialPrint("#");
}
void printFirmwareTime() //Get firmware time 	:GVT# 	Reply: HH:MM:SS#
{
  int HH = hour();
  int MM = minute();
  int SS = second();
  char str[9];
  sprintf(str, "%02d:%02d:%02d#", int(HH), int(MM), int(SS));
  SerialPrint(str);
}
void printFirmwareNumber() //Get firmware number 	:GVN# 	Reply: 0.99a5#
{
  SerialPrint("1.0.1#");
}
void printFirmwareNamne() //Get firmware name 	:GVP# 	Reply: On-Step#
{
  SerialPrint("FireGoTo#");

}
void printAlturaLimite() //:Gh# Get High Limit Returns: sDD*
{
  if (AltitudeLimite < 10) {
    SerialPrint("0");
  }
  SerialPrint(String(AltitudeLimite));
  SerialPrint("*");
  SerialPrint("#");
}

void printHorizonteLimite() //:Go# Get Lower Limit Returns: DD*#
{ if (HorizonteLimite < 10) {
    SerialPrint("0");
  }
  SerialPrint(String(HorizonteLimite));
  SerialPrint("*");
  SerialPrint("#");
}

void setAlturaLimite() //:Sh DD# Set the maximum object elevation limit to DD#
{
  String str = "";
  str += Command[numCommandexec][4];
  str += Command[numCommandexec][5];
  AltitudeLimite = str.toInt();
  str = "";
  SerialPrint("1");
}

void setHorizonteLimite() //:SoDD*# Set lowest elevation to which the telescope will slew
{
  String str = "";
  str += Command[numCommandexec][4];
  str += Command[numCommandexec][5];
  HorizonteLimite = str.toInt();
  str = "";
  SerialPrint(String(HorizonteLimite));
  SerialPrint("1");
}

void printObservatorioNome() //:GM# Get Site 1 Name Returns: <string>#
{
  String str = "";
  str += "Minha Casa";
  str += "#";
  SerialPrint(str);
}

void setObservatorioNome() // Set site 0 name 	:SMsss...# 	Reply: 0 or 1#
{
  String str = "";
  int i = 4;
  while (Command[numCommandexec][i] != '#')
  { str += Command[numCommandexec][i];
    i++;
  }
  str.toCharArray(  configuration.Local, sizeof(str));
}


void setHoraparaUTC() //:SG-03# :SGsHH.H# Set the number of hours added to local time to yield UTC
{
  String str = "";
  str += Command[numCommandexec][4];
  str += Command[numCommandexec][5];
  UTC = str.toInt();
  if (Command[numCommandexec][3] == '-')
  {
    UTC = UTC * -1;
  }
  UTC = UTC * -1;
  SerialPrint("1");
}

void printSIderalRate() //Get sidereal rate RA (0 or 60Hz)	:GT# 	Reply: 0 or 60#
{
  if (SideralRate < 10) {
    SerialPrint("0");
  }
  //  SerialPrint(String(SideralRate));
  //  SerialPrint("0");
  SerialPrint("60#");
}

void setRAAlvo() //:Sr03:43:56# Set target RA 	:SrHH:MM:SS# * 	Reply: 0 or 1#
{
  ativaacom = 0;
  String str = "";
  str += Command[numCommandexec][3];
  str += Command[numCommandexec][4];
  int HH = str.toInt();
  str = "";
  str += Command[numCommandexec][6];
  str += Command[numCommandexec][7];
  int MM = str.toInt();
  str = "";
  str += Command[numCommandexec][9];
  str += Command[numCommandexec][10];
  int SS = str.toInt();
  str = "";
  RAAlvo = Hours2DecDegrees(HH, MM, SS);
  SerialPrint("1");

}

void setDECAlvo() //Set target Dec 	:SdsDD:MM:SS# *	Reply: 0 or 1#
{
  ativaacom = 0;
  String str = "";
  str += Command[numCommandexec][4];
  str += Command[numCommandexec][5];
  int DD = str.toInt();
  str = "";
  str += Command[numCommandexec][7];
  str += Command[numCommandexec][8];
  int MM = str.toInt();
  str = "";
  str += Command[numCommandexec][10];
  str += Command[numCommandexec][11];
  int SS = str.toInt();
  DECAlvo = DegMinSec2DecDeg(DD, MM, SS);
  str = "";
  if (Command[numCommandexec][3] == '-')
  {
    DECAlvo = DECAlvo * (-1);
  }
  SerialPrint("1");
}

void synctelescope() //Sync. with current target RA/Dec	:CS#	Reply: [none]
{
  double horadec = Hora2DecHora(hour(), minute(), SegundoFracao) ;
  double jdia = JulianoDay (year(), month(), day(), horadec) ;
  double HST = HoraSideral(jdia);
  double HSL = HoraSiderallocal(longitude, HST) ;
  Radec2Azalt(HSL, latitude, RAAlvo, DECAlvo, &AzAlvo, &AltAlvo);
  if ((AzAlvo >= 0) && (AltAlvo >= 0)) {
    ALTmount = ( AltAlvo * ResolucaoeixoAltPassoGrau);
    AZmount = ( AzAlvo * ResolucaoeixoAzPassoGrau);
    AZmountAlvo = AZmount;
    ALTmountAlvo = ALTmount;
    syncro();
    ativaacom = 1;
  }

}
void synctelescopeString() //:CM# Synchronizes the telescope position with target. Returns static string: " M31 EX GAL MAG 3.5 SZ178.0'#", or "#" if error.
{
  double horadec = Hora2DecHora(hour(), minute(), SegundoFracao) ;
  double jdia = JulianoDay (year(), month(), day(), horadec) ;
  double HST = HoraSideral(jdia);
  double HSL = HoraSiderallocal(longitude, HST) ;
  Radec2Azalt(HSL, latitude, RAAlvo, DECAlvo, &AzAlvo, &AltAlvo);
  if ((AzAlvo >= 0) && (AltAlvo >= 0)) {
    ALTmount = ( AltAlvo * ResolucaoeixoAltPassoGrau);
    AZmount = ( AzAlvo * ResolucaoeixoAzPassoGrau);
    AZmountAlvo = AZmount;
    ALTmountAlvo = ALTmount;
    syncro();
    ativaacom = 1;
    printRAmount();
  }

}
void setRAbacklash ()// Set RA backlash amount (in ArcSec)	:$BRnnn# 	Reply: 0 or 1#
{
  String str = "";
  str += Command[numCommandexec][4];
  if (Command[numCommandexec][5] != '#')
  {
    str += Command[numCommandexec][5];
    if (Command[numCommandexec][6] != '#')
    {
      str += Command[numCommandexec][6];
    }
  }
  RAbacklash = str.toInt();
  str = "";
  SerialPrint("1");

}

void setDECbacklash ()// Set Dec backlash amount (in ArcSec)	:$BDnnn#	Reply: 0 or 1# */

{
  String str = "";
  str += Command[numCommandexec][4];
  if (Command[numCommandexec][5] != '#')
  {
    str += Command[numCommandexec][5];
    if (Command[numCommandexec][6] != '#')
    {
      str += Command[numCommandexec][6];
    }
  }
  DECbacklash = str.toInt();
  str = "";
  SerialPrint("1");

}
void gototeleEQAR ()// Move telescope (to current Equ target)	:MS#
{ double horadec = Hora2DecHora(hour(), minute(), SegundoFracao) ;
  double jdia = JulianoDay (year(), month(), day(), horadec) ;
  double HST = HoraSideral(jdia);
  double HSL = HoraSiderallocal(longitude, HST) ;
  Radec2Azalt(HSL, latitude, RAAlvo, DECAlvo, &AzAlvo, &AltAlvo);
  if ((AzAlvo >= 0) && (AltAlvo >= 0)) {
    ALTmountAlvo = (MaxPassoAlt * AltAlvo / 360.0);
    AZmountAlvo = (MaxPassoAz * AzAlvo / 360.0);

    dirAltant = dirAlt;
    dirAzant = dirAz;
    AtivaBack = 1;

    SerialPrint("0");
  } else
  {
    SerialPrint("1");
  }
  ativaacom = 1;
}

void Setsidereal() //Set sidereal rate RA (0 or 60Hz)	:STDD.D# 	Reply: [none]
{
}

void acompanhamento()
{ double horadec = Hora2DecHora(hour(), minute(), SegundoFracao) ;
  double jdia = JulianoDay (year(), month(), day(), horadec) ;
  double HST = HoraSideral(jdia);
  double HSL = HoraSiderallocal(longitude, HST) ;
  Radec2Azalt(HSL, latitude, RAAlvo, DECAlvo, &AzAlvo, &AltAlvo);
  if ((AzAlvo >= 0) && (AltAlvo >= 0)) {
    ALTmountAlvo = (MaxPassoAlt * AltAlvo / 360.0);
    AZmountAlvo = (MaxPassoAz * AzAlvo / 360.0);
  }
}


void Stoptelescope () // Stop telescope 	:Q# 	Reply: [none]
{
  statusmovimentacao = 0;
  paramotors();
  ativaacom = 0;
  setSpeed(1);
}

//Move telescope east (at current rate) 	:Me#	Reply: [none]
void moveleste()
{ setSpeed(accel);
  AzMotor.moveTo(0);
  ativaacom = 0;
}

//Move telescope west (at current rate) 	:Mw#	Reply: [none]
void moveoeste()
{ setSpeed(accel);
  AzMotor.moveTo(MaxPassoAlt);
  ativaacom = 0;
}

//Move telescope north (at current rate)	:Mn#	Reply: [none]
void movenorte()
{ setSpeed(accel);
  AltMotor.moveTo(0);
  ativaacom = 0;
}

//Move telescope south (at current rate)	:Ms#	Reply: [none]
void movesul()
{ setSpeed(accel);
  AltMotor.moveTo(MaxPassoAlt / 4);
  ativaacom = 0;
}


void paraleste()                                 //:Qe#	Reply: [none]
{
  setSpeed(1);
  AZmount = AzMotor.currentPosition();
  AzMotor.moveTo(AZmount);
}

//Move telescope west (at current rate) 	:Qw#	Reply: [none]
void paraoeste()

{
  setSpeed(1);
  char str[150];
#ifdef TMC2209    
  sprintf(str,"Alt.msteps=>%d   Az.msteps=>%d",driverAlt.microsteps(),driverAz.microsteps());
#endif
  AZmount = AzMotor.currentPosition();
  AzMotor.moveTo(AZmount);
}

//Move telescope north (at current rate)	:Qn#	Reply: [none]
void paranorte()
{
  setSpeed(1);
  ALTmount = AltMotor.currentPosition();
  AltMotor.moveTo(ALTmount);
}

//Move telescope south (at current rate)	:Qs#	Reply: [none]
void parasul()
{
  setSpeed(1);
  ALTmount = AltMotor.currentPosition();
  AltMotor.moveTo(ALTmount);
}





void MoveRate()
{
  //int ratepadrao = (int)(MaxPassoAz / 86400);
  int ratepadrao = (dReducao/10)*MinTimer;
  switch (Command[numCommandexec][2]) {
    case '0':
      accel = ratepadrao / 4;
      break;
    case '1':
      accel = ratepadrao / 2;
      break;
    case '2':
      accel = ratepadrao ;
      break;
    case '3':
      accel = ratepadrao * 3;
      break;
    case '4':
      accel = ratepadrao * 4;
      break;
    case '5':
      accel = ratepadrao * 8;
      break;
    case '6':
      accel = ratepadrao * 16;
      break;
    case '7':
      accel = ratepadrao * 24;
      break;
    case '8':
      accel = ratepadrao * 40;
      break;
    case '9':
      accel = ratepadrao * 60;
      break;
    case 'G':
      AltaResolucao();
      accel = ratepadrao * 2;
      break;
    case 'C':
      AltaResolucao();
      accel = ratepadrao * 8;
      break;
    case 'M':
      AltaResolucao();
      accel = ratepadrao * 256;
      break;
    case 'S':
      BaixaResolucao();
      accel = ratepadrao * 256;
      break;
  }
}

void setReducao()  //:HSRD000#
//Set microsteps
{
  String str = "";
  str += Command[numCommandexec][5];
  str += Command[numCommandexec][6];
  str += Command[numCommandexec][7];
  unsigned int SS = str.toInt();
  configurationFromFlash.Reducao = SS;
  Reducao = configurationFromFlash.Reducao;
  // write configuration struct to flash at adress 4
  byte b2[sizeof(Configuration)]; // create byte array to store the struct
  memcpy(b2, &configurationFromFlash, sizeof(Configuration)); // copy the struct to the byte array
  //  dueFlashStorage.write(4, b2, sizeof(Configuration)); // write byte array to flash
  #ifdef TMC2209
    driverAz.microsteps(Reducao);
    driverAlt.microsteps(Reducao);
#endif
  SerialPrint("1 - Steps for stepper motors OK");

}

void setMaxPassoAlt()  //:HSAL00000000#
//Add new value to get microsteps over 10kk
{
  String str = "";
  str += Command[numCommandexec][5];
  str += Command[numCommandexec][6];
  str += Command[numCommandexec][7];
  str += Command[numCommandexec][8];
  str += Command[numCommandexec][9];
  str += Command[numCommandexec][10];
  str += Command[numCommandexec][11];
  str += Command[numCommandexec][12];
  unsigned int SS = str.toInt();
  configurationFromFlash.MaxPassoAlt = SS;
  MaxPassoAlt = configurationFromFlash.MaxPassoAlt;
  // write configuration struct to flash at adress 4
  byte b2[sizeof(Configuration)]; // create byte array to store the struct
  memcpy(b2, &configurationFromFlash, sizeof(Configuration)); // copy the struct to the byte array
  //  dueFlashStorage.write(4, b2, sizeof(Configuration)); // write byte array to flash
  SerialPrint("1 - OK Set AL microsteps");

}

void setMaxPassoAz() //:HSAZ00000000#
//Add new value to get microsteps over 10kk
{
  String str = "";
  str += Command[numCommandexec][5];
  str += Command[numCommandexec][6];
  str += Command[numCommandexec][7];
  str += Command[numCommandexec][8];
  str += Command[numCommandexec][9];
  str += Command[numCommandexec][10];
  str += Command[numCommandexec][11];
  str += Command[numCommandexec][12];
  unsigned int SS = str.toInt();
  configurationFromFlash.MaxPassoAz = SS;
  MaxPassoAz = configurationFromFlash.MaxPassoAz;
  byte b2[sizeof(Configuration)]; // create byte array to store the struct
  memcpy(b2, &configurationFromFlash, sizeof(Configuration)); // copy the struct to the byte array
  //  dueFlashStorage.write(4, b2, sizeof(Configuration)); // write byte array to flash
  SerialPrint("1 - OK Set AZ microsteps");

}

void setMinTimer() //:HST00000#
{
  String str = "";
  str += Command[numCommandexec][4];
  str += Command[numCommandexec][5];
  str += Command[numCommandexec][6];
  str += Command[numCommandexec][7];
  str += Command[numCommandexec][8];
  unsigned int SS = str.toInt();
  configurationFromFlash.MinTimer = SS;//; + 200;
  MinTimer = configurationFromFlash.MinTimer ;  //valor minimo
  byte b2[sizeof(Configuration)]; // create byte array to store the struct
  memcpy(b2, &configurationFromFlash, sizeof(Configuration)); // copy the struct to the byte array
  //  dueFlashStorage.write(4, b2, sizeof(Configuration)); // write byte array to flash
  restartTimer(SS);
  SerialPrint("1 - OK Set timer");
}

void setSentidoRA() //:HSSRA0#
{
  String str = "";
  str += Command[numCommandexec][6];
  unsigned int RA = str.toInt();
  configurationFromFlash.SentidoRA = RA;
  SentidoRA = configurationFromFlash.SentidoRA;
  byte b2[sizeof(Configuration)]; // create byte array to store the struct
  memcpy(b2, &configurationFromFlash, sizeof(Configuration)); // copy the struct to the byte array
  //  dueFlashStorage.write(4, b2, sizeof(Configuration)); // write byte array to flash
  SentidodosMotores();
  SerialPrint("1 - OK Set RA Direction");
}
void setSentidoDEC() //:HSSDEC0#
{
  String str = "";
  str += Command[numCommandexec][7];
  unsigned int Dec = str.toInt();
  configurationFromFlash.SentidoDEC = Dec;
  SentidoDEC = configurationFromFlash.SentidoDEC;
  byte b2[sizeof(Configuration)]; // create byte array to store the struct
  memcpy(b2, &configurationFromFlash, sizeof(Configuration)); // copy the struct to the byte array
  //  dueFlashStorage.write(4, b2, sizeof(Configuration)); // write byte array to flash
  SentidodosMotores();
  SerialPrint("1 - OK Set DEC Direction");
}


void getMaxPassoAlt()  //:HGRA#
{
  char str[8];
  sprintf(str, "%07d#", int(MaxPassoAlt));
  SerialPrint(str);
}

void getMaxPassoAz() //:HGRB#
{
  char str[8];
  sprintf(str, "%07d#", int(MaxPassoAz));
  SerialPrint(str);
}

void getMinTimer() //:HGT#
{
  char str[8];
  sprintf(str, "%07d#", int(MinTimer));
  SerialPrint(str);
}



/*
  Move telescope (to current Equ target)	:MS#	Reply: e# *2
  Move telescope (to current Hor target)	:MA#	Reply: e# *2
  2 = Error codes for the MS and MA commands are as follows: e=0 (no error), e=1 (below horizon), e=2 (no object), e=4 (position unreachable), e=5 (not aligned), e=6 (outside limits)*/
/*

  http://www.stellarjourney.com/index.php?r=site/software_telescope
  Here's a summary of the features/command set currently implemented for the On-Step controller:
  Return values generally indicate failure (0) or success (1).
  Command length is limited to 20 chars, 2 for the command frame ":#" + 2 for the code "CC" + a maximum of 16 for the parameter "P": ":CCPPP...#". Cr/lf chars can be sent along with your command, but are ignored.


  Date/time commands
  pronto Set date 	:SCMM/DD/YY#	Reply: 0 or 1#
  Get date 	:GC# 	Reply: MM/DD/YY#
  Set time (Local) 	:SLHH:MM:SS#	Reply: 0 or 1#
  Get time (Local) 	:GLHH:MM:SS#	Reply: HH:MM:SS#
  Set time (Sidereal) 	:SSHH:MM:SS#	Reply: 0 or 1#
  Get time (Sidereal) 	:GS# 	Reply: HH:MM:SS#

  Site/Location commands
  Set UTC Offset(for current site)	:SGsHH# 	Reply: 0 or 1#
  Get UTC Offset(for current site)	:GG# 	Reply: sHH#
  The UTC Offset value is the number of hours to add to your Local Time (Standard Time) to get Universal Time.
  Set Latitude (for current site)	:StsDD*MM# 	Reply: 0 or 1#
  Get Latitude (for current site)	:Gt# 	Reply: sDD*MM#
  Set Longitude (for current site)	:SgDDD*MM# 	Reply: 0 or 1#
  Get Longitude (for current site)	:Gg# 	Reply: DDD*MM#
  Set site 0 name 	:SMsss...# 	Reply: 0 or 1#
  Set site 1 name 	:SNsss...# 	Reply: 0 or 1#
  Set site 2 name 	:SOsss...# 	Reply: 0 or 1#
  Set site 3 name 	:SPsss...# 	Reply: 0 or 1#
  Get site 0 name 	:GM# 	Reply: sss...#
  Get site 1 name 	:GN# 	Reply: sss...#
  Get site 2 name 	:GO# 	Reply: sss...#
  Get site 3 name 	:GP# 	Reply: sss...#
  Select site n (0-3) 	:Wn# 	Reply: [none]

  Slewing/Movement commands
  Set target RA 	:SrHH:MM:SS# * 	Reply: 0 or 1#
  Get target RA 	:Gr# 	Reply: HH:MM:SS# *
  Set target Dec 	:SdsDD:MM:SS# *	Reply: 0 or 1#
  Get target Dec 	:Gd# 	Reply: sDD*MM'SS# *
  Set target Azm 	:SgDDD:MM:SS# *	Reply: 0 or 1#
  Get target Azm 	:Gg# 	Reply: DDD*MM'SS# *
  Set target Alt 	:StsDD:MM:SS# *	Reply: 0 or 1#
  Get target Alt 	:Gt# 	Reply: sDD*MM'SS# *
  Get telescope HA 	:GH# 	Reply: HH:MM:SS# *
  Get telescope RA 	:GR# 	Reply: HH:MM:SS# *
  Get telescope Dec 	:GD# 	Reply: sDD*MM'SS# *
  Get telescope Azm 	:GZ# 	Reply: DDD*MM'SS# *
  Get telescope Alt 	:GA# 	Reply: sDD*MM'SS# *
  = Defaults to high precision mode, in low precision mode "HH:MM.M", "sDD*MM", or "DDD*MM" are used as appropriate.
  Set horizon limit 	:ShsDD# 	Reply: 0 or 1#
  Get horizon limit 	:GhsDD# 	Reply: sDD#
  Set overhead limit 	:SoDD# 	Reply: 0 or 1#
  Get overhead limit 	:GoDD# 	Reply: sDD#
  The horizon limit sets how far below (or above) the horizon the telescope will point for a goto:
  Valid range (in degrees) is +30 to -30.
  The overhead limit helps keep the telescope tube from hitting the tripod etc. during a goto:
  Valid range (in degrees) is 60 to 90.
  Move telescope (to current Equ target)	:MS#	Reply: e# *2
  Move telescope (to current Hor target)	:MA#	Reply: e# *2
  2 = Error codes for the MS and MA commands are as follows:
  e=0 (no error), e=1 (below horizon), e=2 (no object), e=4 (position unreachable), e=5 (not aligned), e=6 (outside limits)
  Stop telescope 	:Q# 	Reply: [none]
  Move telescope east (at current rate) 	:Me#	Reply: [none]
  Move telescope west (at current rate) 	:Mw#	Reply: [none]
  Move telescope north (at current rate)	:Mn#	Reply: [none]
  Move telescope south (at current rate)	:Ms#	Reply: [none]
  Stop moving east 	:Qe# 	Reply: [none]
  Stop moving west 	:Qw# 	Reply: [none]
  Stop moving north 	:Qn# 	Reply: [none]
  Stop moving south 	:Qs# 	Reply: [none]
  Pulse guide (at current rate):
  d=n,s,e,w
  nnnn=time in mS
  (from 20 to 16399mS) 	:Mgdnnnn# 	Reply: [none]
  Set rate to Guide 	:RG# 	Reply: [none]
  Set rate to Centering 	:RC# 	Reply: [none]
  Set rate to Move 	:RM# 	Reply: [none]
  Set rate to Slew 	:RS# 	Reply: [none]
  Set rate to n (1-9)*3	:Rn# 	Reply: [none]
  3 = Slew rates are as follows.
  All values are in multipules of the sidereal rate:
  R1(RG)=0.5X, R2=1X, R3=2X, R4(RC)=4X, R5=16X, R6=32X, R7(RM)=60X, R8=120X, R9(RS)=240X
  Set sidereal rate RA (0 or 60Hz)	:STDD.D# 	Reply: [none]
  Get sidereal rate RA (0 or 60Hz)	:GT# 	Reply: 0 or 60#
  Get distance bars (indicates slew)	:D#	Reply: \0x7F#
  Pier side 	:Gm# 	Reply: N#, E# or W#

  Sync. command
  Sync. with current target RA/Dec	:CS#	Reply: [none]

  Anti-backlash commands
  Set RA backlash amount (in ArcSec)	:$BRnnn# 	Reply: 0 or 1#
  Set Dec backlash amount (in ArcSec)	:$BDnnn#	Reply: 0 or 1#

  Periodic error correction commands
  Turn PEC on 	:$QZ+# 	Reply: [none]
  Turn PEC off 	:$QZ-# 	Reply: [none]
  Clear PEC data 	:$QZZ# 	Reply: [none]
  Start recording PEC 	:$QZ/# 	Reply: [none]
  Save PEC data/settings to EEPROM	:$QZ!#	Reply: [none]
  Get PEC status returns:
  I-Ignore PEC,
  P-Playing PEC, p-Getting ready to play PEC,
  R-Record PEC, r-Getting ready to record PEC 	:$QZ?# 	Reply: s#
  Readout PEC data 	:VRnnnn# 	Reply: sddd#
  Readout PEC data at current index (while playing/recording),
  also returns index 	:VR# 	Reply: sddd,ddd#
  Write PEC data 	:WRnnnn,sddd# 	Reply: 0 or 1#
  PEC works as follows:

  In-memory values are byte sized integers (corrections) that hold the number of steps to be applied (recorded/played) at a rate of one correction per second. Up to 824 bytes are available for storage (i.e. 824 seconds for a worm rotation). My implementation uses just 240 bytes (360 tooth worm gear, one revolution every 4 minutes).

  After the data buffer is cleared ($QZZ), the next record session stores the guiding corrections without averaging. Subsequent recording of the guiding corrections use a 2:1 weighted average favoring the buffer. Data in the buffer is played back one second before the record time in the cycle to help compensate for guiding correction latency.

  When reading and writing PEC data the units used are steps.

  The save to EEPROM command allows the results to be recovered after a power cycle. This command takes several seconds to complete and should only be issued after you park or home the mount (stop tracking).

  Alignment commands
  Align, polar alignment mode	:AP# 	Reply: [none]
  Align, one-star*4	:A1# 	Reply: 0 or 1#
  Align, two-star*4	:A2# 	Reply: 0 or 1#
  Align, three-star*4	:A3# 	Reply: 0 or 1#
  Align, accept*4 	:A+# 	Reply: 0 or 1#
  4 = The one star alignment is implemented to correct RA/Dec offset. Two star alignment adds a second star to measure/correct for polar axis Altitude misalignment. Three star alignment adds a third star to measure/correct for polar axis Azimuth misalignment. These are saved when Set park is called and maintained when Parking/Unparking the mount. The sync. equatorial coordinates command refines the model for a local area of the sky, this refinement is lost when the power is cycled unless another Set park is called. The intended use of these commands is as follows...
  Call A1. {or A2/A3.}
  Set RA/Dec target (near the Zenith, telescope east of the mount, for minimal offset in RA due to polar Azm misalignment).
  Slew to target.
  Use move commands to center target.
  Call A+. (records offsets, corrects RA/Dec)

  Continue for 2 or 3 star:
  Set RA/Dec target (near the Zenith, telescope west of the mount, for minimal offset in RA due to polar Azm misalignment).
  Slew to target.
  Use move commands to center target.
  Call A+. (records offset, corrects error in polar altitude)

  Continue for 3 star:
  Set RA/Dec target (near pole altitude in southern sky, for maximum offset in RA due to polar azimuth misalignment).
  Slew to target.
  Use move commands to center target.
  Call A+. (records offset, corrects error in polar azimuth)

  Park commands
  Set park position 	:hQ# 	Reply: 0 or 1#
  Move to park position 	:hP# 	Reply: 0 or 1#
  Restore parked telescope to operation	:hR#	Reply: 0 or 1#

  Home commands
  Set home (CWD) 	:hF# 	Reply: [none]
  Move to home (CWD) 	:hC# 	Reply: [none]

  Misc. commands
  Set baud rate:
  1=56.7K, 2=38.4K, 3=28.8K,
  4=19.2K, 5=14.4K, 6=9600,
  7=4800, 8=2400, 9=1200	:SBn# 	Reply: 0 or 1
  Precision toggle 	:P# 	Reply: [none]
  Get firmware date 	:GVD# 	Reply: MM DD YY#
  Get firmware time 	:GVT# 	Reply: HH:MM:SS#
  Get firmware number 	:GVN# 	Reply: 0.99a5#
  Get firmware name 	:GVP# 	Reply: On-Step#
  Get statUs returns:
  N-Not slewing, H-At Home position,
  P-Parked, p-Not parked, F-Park Failed,
  I-park In progress, R-PEC Recorded 	:GU# 	Reply: sss#
*/
void lcdALTmount() //:GA# Get Telescope Altitude Returns: sDD*MM# or sDD*MM'SS#
{
  #ifdef lcd
  char str[10];
  int Ddeg, Min, Sec;
  Ddeg = (int)DecDegtoDeg(eixoAltGrausDecimal);
  Min = (int)DecDegtoMin(eixoAltGrausDecimal);
  Sec = (int)DecDegtoSec(eixoAltGrausDecimal);
  if (Ddeg < 0) {
    sprintf(str, "-%02d*%02d:%02d#", int(Ddeg), int(Min), int(Sec));
  } else {
    sprintf(str, "+%02d*%02d:%02d#", int(Ddeg), int(Min), int(Sec));
  }
  str[3] = 223;
  lcd.print(str);
  #endif
}

void lcdAZmount()
{
  #ifdef lcd
  char str[10];
  sprintf(str, "%03d*%02d:%02d#", int(DecDegtoDeg(eixoAzGrausDecimal)), int(DecDegtoMin(eixoAzGrausDecimal)), int(DecDegtoSec(eixoAzGrausDecimal)));
  str[3] = 223;
  lcd.print(str);
  #endif
}
