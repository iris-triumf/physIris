#ifndef __PTRACK_H
#define __PTRACK_H

#include "TObject.h"
#include "TClass.h"
#include "nucleus.h"

class PTrack : public TObject{
 	public:
		//nucleus nuc;
  		Double_t E; 	// Energy
  		Double_t Ecm; 	// Energy in center-of-mass system
  		Double_t T; 	// Theta
		Double_t Tdeg;	// Theta in degrees
		Double_t Tcm; 	// Theta in center-of-mass system in degrees;
  		Double_t P; 	// Phi
		Double_t Pdeg; 	// Phi in degrees
		Double_t TrgtdE; 	// Energy loss in half the target
		Double_t FoildE; 	// Energy loss in half the target
		Double_t Ebt; 	// Energy behind the target

 	public:
  		PTrack();//! Create
  		virtual ~PTrack() {} //!

  		void Clear();  //!
	protected:

 	private:
  
	ClassDef(PTrack,1);
};

#endif
