#ifdef ESP32

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth disabled!
#endif

hw_timer_t * timer1 = NULL;
hw_timer_t * timer2 = NULL;
portMUX_TYPE timerMuxAz = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMuxAlt = portMUX_INITIALIZER_UNLOCKED;
volatile SemaphoreHandle_t timerSemaphoreAlt;
volatile SemaphoreHandle_t timerSemaphoreAz;
void IRAM_ATTR onTimerAz() {

  portENTER_CRITICAL_ISR(&timerMuxAz);
  xSemaphoreGiveFromISR( timerSemaphoreAz, NULL);
  portEXIT_CRITICAL_ISR(&timerMuxAz);
}
void IRAM_ATTR onTimerAlt() {
  portENTER_CRITICAL_ISR(&timerMuxAlt);
  xSemaphoreGiveFromISR( timerSemaphoreAlt, NULL);
  portEXIT_CRITICAL_ISR(&timerMuxAlt);

}
void startTimer(int MinTimer) {
  cli();
  //disableCore1WDT();
  //disableCore0WDT();
  timerSemaphoreAlt = xSemaphoreCreateMutex();
  timerSemaphoreAz = xSemaphoreCreateMutex();
  xTaskCreate( callRunMotorAz,
               "callRunMotorAz",
               1000,
               NULL,
               14,
               NULL);
  xTaskCreate( callRunMotorAlt,
               "callRunMotorAlt",
               1000,
               NULL,
               15,
               NULL);


  timer1 = timerBegin(1, 80, true);
  timerAttachInterrupt(timer1, &onTimerAlt, true);
  timerAlarmWrite(timer1, MinTimer, true);
  timerAlarmEnable(timer1);    //Enable timer
  timer2 = timerBegin(0, 80, true);
  timerAttachInterrupt(timer2, &onTimerAz, true);
  timerAlarmWrite(timer2, MinTimer, true);
  timerAlarmEnable(timer2);    //Enable timer
  sei();//allow interrupts

}

void callRunMotorAz( void * pvParameters ) {
  for (;;) {
    if ( xSemaphoreTake( timerSemaphoreAz, portMAX_DELAY ) == pdTRUE ) {
      runmotorAz();
    }
  }
}
void callRunMotorAlt( void * pvParameters ) {
  for (;;) {
    if ( xSemaphoreTake( timerSemaphoreAlt, portMAX_DELAY ) == pdTRUE ) {
      runmotorAlt();
    }
  }
}

void restartTimer(int MinTimer) {
  cli();
  timerAlarmWrite(timer1, MinTimer , true);
  sei()
  timerRestart(timer1);
  cli();
  timerAlarmWrite(timer2, MinTimer , true);
  sei();
  timerRestart(timer2);
}
#ifdef BLUETOOTH
void iniciaBluetooth() {
  SerialBT.enableSSP();
  SerialBT.begin("Firegoto"); //Bluetooth device name
  Serial.println("BT ready tor pair!");
}
#endif
#ifdef WIFI

#define maxCli  4
WiFiClient clis[maxCli];
const char* ssid = "lar_doce_lar";
const char* password =  "Blessed@1";

WiFiServer wfSrv(9999);

void iniciaWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }

  Serial.println("WiFi CONN.");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Ini TCP srv");
  wfSrv.begin(); // start TCP server

  wfSrv.setNoDelay(true);
  xTaskCreate( handleTCPConnection,
               "handleTCPConnection",
               100000,
               NULL,
               8,
               NULL);
}
void handleTCPConnection(void * pvParameters) {
  for (;;) {
    if (wfSrv.hasClient()) {
      for (byte i = 0; i < maxCli; i++) { // busca uma conexão livre no pool de conexões
        if (!clis[i] || !clis[i].connected()) {
          if (clis[i]) clis[i].stop();
          clis[i] = wfSrv.available();
          continue;
        }
      }
      //Caso não haja conexões disponiveis, rejeita a nova conexão.
      WiFiClient tcli = wfSrv.available();
      tcli.stop();
    }
    for (byte cli = 0; cli < maxCli; cli++) {
      if (clis[cli])
      {
        while (clis[cli].available()) {
          processaLeitura((char)clis[cli].read());
        }
        cmdComplete = true;
        curClient = clis[cli];
        executecommand();
      }
    }
    vTaskDelay(100); //ticks para ms
  }
}
#endif
#endif
