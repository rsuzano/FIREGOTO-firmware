
//2. Pinos Joystick
#define xPin     A0   
#define yPin     A1   
#define kPin     A2   
//Criacao dos motores
#define MotorALT_Direcao 22
#define MotorALT_Passo 24
#define MotorALT_Sleep 26
#define MotorALT_Reset 28
#define MotorALT_M2 30
#define MotorALT_M1 32
#define MotorALT_M0 34
#define MotorALT_Ativa 36
#define MotorAZ_Direcao 38
#define MotorAZ_Passo 40
#define MotorAZ_Sleep 42
#define MotorAZ_Reset 44
#define MotorAZ_M2 46
#define MotorAZ_M1 48
#define MotorAZ_M0 50
#define MotorAZ_Ativa 52
#define lcd_pin1 20 
#define lcd_pin2 4
#define SW_TX_SERIAL_AZ MotorAZ_Reset
#define SW_RX_SERIAL_AZ MotorAZ_M2
#define SW_TX_SERIAL_ALT MotorALT_Reset
#define SW_RX_SERIAL_ALT MotorALT_M2
#define TMC2209_ALT_MS1 MotorALT_M1
#define TMC2209_ALT_MS2 MotorALT_M0
#define TMC2209_AZ_MS1 MotorAZ_M1
#define TMC2209_AZ_MS2 MotorAZ_M0


//LEDs
#define LedB 49
#define LedR 53
#define LedG 51
