// Graphsdedx.h

#ifndef GraphsH
#define GraphsH
#include <TObject.h>
#include <TClass.h>

//Extern
//extern int gMesytecnitems;
class Graphsdedx : public TObject {
	public:
		Graphsdedx(); 
		virtual ~Graphsdedx() {} //! 

		std::string Ag;
		std::string Al;
		std::string B;
		std::string Tgt;
		std::string My;
		std::string P;
		std::string Si;
		std::string SiO2;
		std::string Iso;
		std::string Wndw;

		Bool_t boolAg;
		Bool_t boolAl;
		Bool_t boolB;
		Bool_t boolTgt;
		Bool_t boolMy;
		Bool_t boolP;
		Bool_t boolSi;
		Bool_t boolSiO2;
		Bool_t boolIso;
		Bool_t boolWndw;

		//virtual void ReadCalibPar(char* line);
		virtual void ReadGraphnames(char* line);
		virtual void Load(TString filename);
		virtual void Print();
		virtual void Clear();
//		ClassDef(Graphsdedx,1)
};

#endif
// end
