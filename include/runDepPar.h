// run dependent parameters
#ifndef runDep_H
#define runDep_H
#include <TObject.h>
#include <TClass.h>
#include <string>

class runDep : public TObject {
	public:
		runDep(); 
		virtual ~runDep() {} //! 
	 	Double_t EBAC; //beam energy from accelerator
	 	Double_t energy; //beam energy behind target (also initialized with value from accelerator)
	  	Double_t momentum;// momentum in MeV. Initial value calculated using EBAC, has to be properly set in HandleBOR
	  	Double_t beta;// beam beta in center-of-mass system. Initial value calculated using EBAC, has to be properly set in HandleBOR
	  	Double_t gamma;// beam gamma in center-of-mass system. Initial value calculated using EBAC, has to be properly set in HandleBOR 
		Double_t ICmin; // Lower limit of ionization chamber gate for incoming beam.
		Double_t ICmax; // Upper limit of ionization chamber gate for incoming beam.
		std::string nA;
		std::string na;
		std::string nB;
		std::string nb;

		void setRunDepPar(std::string);
		void Print();
};
#endif
// end
