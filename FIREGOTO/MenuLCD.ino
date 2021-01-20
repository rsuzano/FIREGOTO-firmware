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

/*  
    Joytick & LCD & Menu   by JMLoureiro77

    http://jmloureiro77.blogspot.com.es

  328P Flash 5292 (17%) RAM 409(19%)

*/
//====================================================
// Menu
//====================================================
void menu(){
  switch (mNivel1){
    case 0:
      menu0();//Tela de Inicio
    break;
    case 1:
        if(mNivel2==1){menu11();
        }else if (mNivel2==2){menu12();
        }else if (mNivel2==3){menu13();
        }else if (mNivel2==4){menu14();
        }else if (mNivel2==5){menu15();}
        else{menu1();}
    break;
    case 2:
        if(mNivel2==1){menu21();
        }else if (mNivel2==2){menu22();
        }else if (mNivel2==3){menu23();}
        else{menu2();}
    break;
    case 3:
        menu3();
    break;
    case 4:
      menu4();
    break;
  }//switch
}
//----------------------------------------------------
// Tela de Inicio
//----------------------------------------------------
void menu0(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print("      FireGoTo      ");
  lcd.setCursor(0,1);
  //lcd.print("19/01/21 - 00:00:00");
  LCDClockDisplay();
  lcd.setCursor(0,2);
  lcd.print("RA:  ");
  lcd.setCursor(4,2);
  lcdALTmount();
  lcd.setCursor(0,3);
  lcd.print("DEC: ");
  lcd.setCursor(4,3);
  lcdAZmount();
}
//----------------------------------------------------
// Menu Configuracoes
//----------------------------------------------------
void menu1(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Configuracoes");
  lcd.setCursor(0,1);
}
//-------------------------------------------------1.1
void menu11(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Micropassos");
  //++++++++++++++++++++
  while(editMode){
    controlJoystick();
    lcd.setCursor(lcdX,1);
    if(n[lcdX]!=lastN){
      lcd.print(n[lcdX]);
      lastN=n[lcdX];
    }
  }
}
//-------------------------------------------------1.2
void menu12(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Reducao");
}
//-------------------------------------------------1.3
void menu13(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Velocidade do Motor");
}
void menu14(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Sentido RA");
}
void menu15(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Sentido DEC");
}
//----------------------------------------------------
// Menu Alinhamento
//----------------------------------------------------
void menu2(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Alinhamento");
}
//-------------------------------------------------2.1
void menu21(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">1a estrela");
}
//-------------------------------------------------2.2
void menu22(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">2a estrela");
}
//-------------------------------------------------2.3
void menu23(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">3a estrela");
}
//----------------------------------------------------
// Menu Mover DEC e RA
//----------------------------------------------------
void menu3(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Mover RA e DEC");
}
//-------------------------------------------------2.1

//----------------------------------------------------
// Menu Microfocalizador
//----------------------------------------------------
void menu4(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Microfocalizar");
}

//====================================================
// Controlar Joystick
//====================================================
void controlJoystick(){
  leeJoystick();
  if(PQCP) {
    PQCP=0;
    if (joyPos==5){editMode=!editMode;}
    switch (editMode){
       case 1: 
          lcd.blink();
          if (joyPos==4&&n[lcdX]<9){n[lcdX]++;   //cima
              refresh=0;}
          if (joyPos==3&&n[lcdX]>0){n[lcdX]--;   //baixo
              refresh=0;} 
          if (joyPos==1&&lcdX<19){lcdX++;        //direita
            refresh=0;}
          if (joyPos==2&&lcdX>0){lcdX--;         //esquerda
           refresh=0;}
        break;
        case 0:
          lcd.noBlink();
          if (mNivel1<4&&joyPos==3){mNivel1++;    //baixo
            refresh=1;
            mNivel2=0;}
          if (mNivel1>0&&joyPos==4){mNivel1--;    //cima
            mNivel2=0;
            refresh=1;}
          if (mNivel2<5&&joyPos==1){mNivel2++;   //direita
            refresh=1;}
          if (mNivel2>0&&joyPos==2){mNivel2--;    //esquerda
           refresh=1;}
        }//!edit
  }//PQCP
}
int leeJoystick(){
  int x = analogRead(xPin);
  int y = analogRead(yPin);
  int k = digitalRead(kPin);
    if(x>900){joyRead=1;        //x+
    }else if(x<100){joyRead=2;  //x-
    }else if(y>900){joyRead=3;  //y+
    }else if(y<100){joyRead=4;  //y-
    }else if(!k){joyRead=5;
    }else{joyRead=0;}

  if (joyRead != lastJoyPos){lastDebounceTime = millis();}
  if(((millis() - lastDebounceTime) > debounceDelay)&&(joyRead!=joyPos)){
    joyPos=joyRead;
    if(!PQCP){PQCP=1;}
    }
  if(((millis() - lastDebounceTime) > (5*debounceDelay))&&(joyPos==3||joyPos==4)){
    joyPos=joyRead;                     //repeat time only for UP/DOWN
    if(!PQCP){PQCP=1;}
    }
  lastJoyPos=joyRead;
}
