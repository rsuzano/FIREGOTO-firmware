#if __arm__
  #include "soft_uart.h"
  #include <DueTimer.h>
  void startTimer(int MinTimer){
    Timer3.start(MinTimer);
    Timer3.attachInterrupt(runmotorAz);
    Timer2.start(MinTimer);
    Timer2.attachInterrupt(runmotorAlt);
  }
  void restartTimer(int MinTimer){
    Timer3.stop();
    Timer3.start(MinTimer);
    Timer2.stop();
    Timer2.start(MinTimer);
  }
  
  
#endif
