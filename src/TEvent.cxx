// Author: Alisher Sanetullaev  TRIUMF, 2012/10/01                                           
#include <TClonesArray.h>
#include <TStopwatch.h>
#include <TMath.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <fstream>

//#include "PhysicalConstants.h"
#include "TEvent.h"
//#include "IParticle.h"
#ifndef kAmu
#define kAmu 931.494013
#endif

//ClassImp(TEvent);
//ClassImp(IParticle);
//using namespace TMath;

using namespace std;

//ofstream fout;

TEvent::TEvent() {
  //  TBranch       *b_fqV;                                            // */

   //TTree        *fChain;                       // pointer to the analyzed TTree or TChain

  TEvent::Class()->IgnoreTObjectStreamer();
  //IParticle::Class()->IgnoreTObjectStreamer();   
  //fPart = new TParticle();        //

  // fEYY1 =0;             //
  // fCsI=0;              //
  // fSd1=0;              //
  // fSd2=0;              //
  // fIC=0;               //

  fLP=0; //Light particle energy //
  fHP=0; //Heavy particle energy //
  fEB=0; //Measured heavy particle energy
  fEB1=0; //Calculated heavy particle energy
  fPB1=0; //Calculated heavy particle momentum
  fQv1=0;                         //
  fQv1_nocorr=0;                         //
  fEB2=0; //Calculated heavy particle energy
  fPB2=0; //Calculated heavy particle momentum
  fQv2=0;                         //
  fQv2_nocorr=0;                         //
  fThetacm1=0;                         //
  fThetacm2=0;                         //


}
TEvent::TEvent(Int_t k) {fRun = k;}
TEvent::TEvent(const TEvent &evt) : TObject(evt) //
{
  // -- Copy constructor.                                                                
  ((TEvent&)evt).Copy(*this);
}

Int_t TEvent::Clear(){
	fEYY1 =0;             //
	fECsI1=0;              //
	fECsI2=0;              //
	// fSd1=0;              //
	// fSd2=0;              //
	// fIC=0;               //
	
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

	fLP=0; //Light particle energy
	fHP=0; //Heavy particle energy
	fEB=0; //Measured heavy particle energy
	fEB1=0; //Calculated heavy particle energy
	fPB1=0; //Calculated heavy particle momentum
	fQv1=0;
	fQv1_nocorr=0;
	fEB2=0; //Calculated heavy particle energy
	fPB2=0; //Calculated heavy particle momentum
	fQv2=0;
	fQv2_nocorr=0;
	fThetacm1=0;
	fThetacm2=0;
	fThetaD=0;
	// fPart.Clear();
return 1;
}

Int_t TEvent::Calculate(){

  //  fEB = fHP;
  //fQv = 1.;
   return 1;
}
