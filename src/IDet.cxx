//IDet.cxx
#include "IDet.h"
#include <math.h>

IDet::IDet(){
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
	TYdNeighbour.clear();
	
	TCsI1Mul=0;
	TCsI2Mul=0;
  	TCsI1Energy.clear();
  	TCsI2Energy.clear();
  	TCsI1ADC.clear();
  	
	TCsI2ADC.clear();
	TCsI1Channel.clear();
	TCsI2Channel.clear();
	TCsI1Phi.clear();
	TCsI2Phi.clear();

  	TYdCsI1ETot=0;
  	TYdCsI2ETot=0;
  	
	TSSBADC=0;
	TSSBEnergy=0;
	TScADC=0;
	TScEnergy=0;

	TTrADC.clear();
   	TTrEnergy.clear();

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
	TSd1rNeighbour.clear();
  	TSd1rADC.clear();
  	TSd2rEnergy.clear();
	TSd2rChannel.clear();
	TSd2rNeighbour.clear();
  	TSd2rADC.clear();
  	TSd1sEnergy.clear();
	TSd1sChannel.clear();
	TSd1sNeighbour.clear();
  	TSd1sADC.clear();
  	TSd2sEnergy.clear();
	TSd2sChannel.clear();
	TSd2sNeighbour.clear();
  	TSd2sADC.clear();
  	TSd1Theta.clear();
  	TSd2Theta.clear();
  	TSd1Phi.clear();
  	TSd2Phi.clear();
  	
	TYuMul=0;  
	TYuADC.clear();
	TYuEnergy.clear();
  	TYuTheta.clear();// Yd theta angle                                                                       
	TYuChannel.clear();
	TYuNo.clear();
	TYuRing.clear();
	TYuNeighbour.clear();
	
	TSurMul=0;
	TSusMul=0;
	TSurEnergy.clear();
	TSurChannel.clear();
	TSurNeighbour.clear();
  	TSurADC.clear();
	TSusEnergy.clear();
	TSusChannel.clear();
	TSusNeighbour.clear();
  	TSusADC.clear();
  	TSuTheta.clear();
  	TSuPhi.clear();
		
	TZdxMul=0;
	TZdxEnergy.clear();
	TZdxChannel.clear();
	TZdxPos.clear();
  	TZdxADC.clear();

	TZdyMul=0;
	TZdyEnergy.clear();
	TZdyChannel.clear();
	TZdyPos.clear();
  	TZdyADC.clear();

	TZdMul=0;
	TZdR.clear();
	TZdTheta.clear();
	TZdPhi.clear();
}

