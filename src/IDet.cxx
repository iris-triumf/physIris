//IDet.cxx
#include "IDet.h"
#include <math.h>

IDet::IDet(){
  //IDet::Class()->IgnoreTObjectStreamer;
  IDet::Clear();
}

void IDet::Clear()
{
  	TYdMul=0;  
	TYdADC.clear();
	TYdEnergy.clear();
  	TYdTheta.clear();// Yd theta angle                                                                       
	TYdChannel.clear();
	TYdNo.clear();
	TYdRing.clear();
	
	TCsI1Mul=0;
	TCsI2Mul=0;
  	TCsI1Energy.clear();
  	TCsI2Energy.clear();
  	//TCsIEnergy.clear();
  	TCsI1ADC.clear();
  	TCsI2ADC.clear();
//	TCsIChannel.clear();
	TCsI1Channel.clear();
	TCsI2Channel.clear();

  	TYdCsI1ETot=0;
  	TYdCsI2ETot=0;
  	
	SSB=0;
	TICADC.clear();
   	TICEnergy.clear();
	TICChannel.clear();

	TSd1rMul=0;
	TSd1sMul=0;
	TSd2rMul=0;
	TSd2sMul=0;
  	TSdETot=0;
  	TSdThetaCM=0;
  	TSd1rEnergy.clear();
	TSd1rChannel.clear();
  	TSd1rADC.clear();
  	TSd2rEnergy.clear();
	TSd2rChannel.clear();
  	TSd2rADC.clear();
  	TSd1sEnergy.clear();
	TSd1sChannel.clear();
  	TSd1sADC.clear();
  	TSd2sEnergy.clear();
	TSd2sChannel.clear();
  	TSd2sADC.clear();
  	TSdTheta.clear();
  	TSdPhi.clear();
//	TQv1=0; // Q-value  
//	TQv2=0; // Q-value
// 	QValue1=sqrt(-1.);//Not a number (nan) AS
// 	QValue2=sqrt(-1.);//                                                                                
// 	QValue=sqrt(-1.);//                                                                                
// 	TBE=0; //Beam energy                                                                             
}

