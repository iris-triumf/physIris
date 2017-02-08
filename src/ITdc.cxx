//ITdc.cxx

#include "ITdc.h"
#include <math.h>
ITdc::ITdc(){
  ITdc::Clear();
}

void ITdc::Clear()
{
	TICTMul = 0;
	//TICTDC.clear();
	TICTChannel.clear();
	TICTime.clear();
	TICTimeRF.clear();
	
	TYdTMul = 0;
	//TYdTDC.clear();
	TYdTChannel.clear();
	TYdTime.clear();
	TYdTimeRF.clear();
	
	TYuTMul = 0;
	//TYuTDC.clear();
	TYuTChannel.clear();
	TYuTime.clear();
	TYuTimeRF.clear();
	
	TSurTMul = 0;
	//TSurTDC.clear();
	TSurTChannel.clear();
	TSurTime.clear();
	TSurTimeRF.clear();
	
	TSusTMul = 0;
	//TSusTDC.clear();
	TSusTChannel.clear();
	TSusTime.clear();
	TSusTimeRF.clear();
	
	TSd1rTMul = 0;
	//TSd1rTDC.clear();
	TSd1rTChannel.clear();
	TSd1rTime.clear();
	TSd1rTimeRF.clear();
	
	TSd1sTMul = 0;
	//TSd1sTDC.clear();
	TSd1sTChannel.clear();
	TSd1sTime.clear();
	TSd1sTimeRF.clear();
	
	TSd2rTMul = 0;
	//TSd2rTDC.clear();
	TSd2rTChannel.clear();
	TSd2rTime.clear();
	TSd2rTimeRF.clear();
	
	TSd2sTMul = 0;
	//TSd2sTDC.clear();
	TSd2sTChannel.clear();
	TSd2sTime.clear();
	TSd2sTimeRF.clear();
}

