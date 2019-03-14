 // Author: Alisher Sanetullaev  TRIUMF, 2012/10/01                                          
// Rewritten: Matthias Holl, 2017/09/01

#ifndef __TEVENT_H
#define __TEVENT_H

#include <TClass.h>
#include <TObject.h>

class TEvent : public TObject {
	public:
		TEvent(); //! Create
		TEvent(const TEvent &); //!
		virtual ~TEvent() {} //!
			
		Double_t fEYY1;
		Int_t fCCsI1; // HLC 06/25/17
		Int_t fCCsI2; // HLC 06/25/17
		Double_t fECsI1;
		Double_t fECsI2;
		
		Double_t fThetacm1;
		Double_t fThetacm2;
		Double_t fThetacmU;
		Double_t fThetacmUSd;
		Double_t fThetaD;
		Double_t fThetaDU;
		Double_t fThetaDUSd;
			
		Double_t fEBAC;
		Double_t fmA;
		Double_t fma;
		Double_t fmB;
		Double_t fmb;
		Double_t fkBF;
		Double_t fEBeam;
		Double_t fbetaCM;
		Double_t fgammaCM;
		Double_t fPA;
	  	Double_t fPBeam;
	  	Double_t fPResid;
	  	Double_t fA;
	  	Double_t fB;
	  	Double_t fC;
	   	
		Double_t fEb1;
	  	Double_t fPb1;
	  	Double_t fPb1y;
	  	Double_t fPb1xcm;
 		
		Double_t fEb2;
	  	Double_t fPb2;
	  	Double_t fPb2y;
	  	Double_t fPb2xcm;
		
		Double_t fEbU;
	  	Double_t fPbU;
	  	Double_t fPbUy;
	  	Double_t fPbUxcm;
		
		Double_t fEbUSd;
	  	Double_t fPbUSd;
	  	Double_t fPbUSdy;
	  	Double_t fPbUSdxcm;

		Double_t fLP;	// Light particle energy
		Double_t fHP;	// Heavy particle energy
		Double_t fEB;	// Measured heavy particle energy
		
		Double_t fEB1;	// Calculated heavy particle energy
		Double_t fPB1;	// Calculated heavy particle momentum
		Double_t fQv1;	// Q-value
		
		Double_t fEB2;	// Calculated heavy particle energy
		Double_t fPB2;	// Calculated heavy particle momentum
		Double_t fQv2;	// Q-value
		
		Double_t fEBU;	// Calculated heavy particle energy
		Double_t fPBU;	// Calculated heavy particle momentum
		Double_t fQvU;	// Q-value
		
		Double_t fEBUSd;	// Calculated heavy particle energy
		Double_t fPBUSd;	// Calculated heavy particle momentum
		Double_t fQvUSd;	// Q-value
		
		void Clear();	//! Clear values
		
	private:
		Int_t fRun; // Current run number
		ClassDef(TEvent,4) // Version
};
#endif
