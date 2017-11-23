// Author: Alisher Sanetullaev  TRIUMF, 2012/10/01                                           
// Rewritten: Matthias Holl, 2017/09/01
#include <math.h>
#include "TEvent.h"

TEvent::TEvent() {
	//TEvent::Class()->IgnoreTObjectStreamer();
	TEvent::Clear();
	
}

TEvent::TEvent(const TEvent &evt) : TObject(evt) //
{
	// -- Copy constructor.                                                                
	((TEvent&)evt).Copy(*this);
}

void TEvent::Clear(){
	fEYY1 =0;             //
	fCCsI1=0; // HLC 6/25/17
	fCCsI2=0; // HLC 6/25/17
	fECsI1=0;              //
	fECsI2=0;              //
	
	fEBAC = sqrt(-1.);
	fmA = sqrt(-1.);
	fma = sqrt(-1.);
	fmB = sqrt(-1.);
	fmb = sqrt(-1.);
	fkBF = sqrt(-1.);
	fEBeam = sqrt(-1.);
	fbetaCM = sqrt(-1.);
	fgammaCM = sqrt(-1.);
	fPA = sqrt(-1.);
  	fPBeam = sqrt(-1.);
  	fPResid = sqrt(-1.);
  	fA = sqrt(-1.);
  	fB = sqrt(-1.);
  	fC = sqrt(-1.);
  	fEb1 = sqrt(-1.);
  	fPb1 = sqrt(-1.);
  	fPb1y = sqrt(-1.);
  	fPb1xcm = sqrt(-1.);
	fEb2 = sqrt(-1.);
  	fPb2 = sqrt(-1.);
  	fPb2y = sqrt(-1.);
  	fPb2xcm = sqrt(-1.);
	fEbU = sqrt(-1.);
  	fPbU = sqrt(-1.);
  	fPbUy = sqrt(-1.);
  	fPbUxcm = sqrt(-1.);
	fEbUSd = sqrt(-1.);
  	fPbUSd = sqrt(-1.);
  	fPbUSdy = sqrt(-1.);
  	fPbUSdxcm = sqrt(-1.);

	fLP=0; //Light particle energy
	fHP=0; //Heavy particle energy
	fEB=0; //Measured heavy particle energy
	fEB1=0; //Calculated heavy particle energy
	fPB1=0; //Calculated heavy particle momentum
	fQv1=0;
	fEB2=0; //Calculated heavy particle energy
	fPB2=0; //Calculated heavy particle momentum
	fQv2=0;
	fEBU=0; //Calculated heavy particle energy
	fPBU=0; //Calculated heavy particle momentum
	fQvU=0;
	fEBUSd=0; //Calculated heavy particle energy
	fPBUSd=0; //Calculated heavy particle momentum
	fQvUSd=0;
	
	fThetacm1=0;
	fThetacm2=0;
	fThetacmU=0;
	fThetaD=0;
	fThetaDU=0;
}

