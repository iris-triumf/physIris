// CalibPHYSICS.h

#ifndef CalibPHYSICS_H
#define CalibPHYSICS_H
#include <TObject.h>
#include <TClass.h>
#include <string>

//Extern
//extern int gPHYSICSnitems;
class CalibPHYSICS : public TObject {
	public:
		CalibPHYSICS(); 
		virtual ~CalibPHYSICS() {} //! 
		
		std::string installPath;
		std::string fileRunList;
		std::string fileGate;
		std::string nameGate;
		std::string fileSdGate;
		std::string nameSdGate;
		std::string fileYuGate;
		std::string nameYuGate;
		std::string fileSuGate;
		std::string nameSuGate;
		std::string fileELoss;
		std::string fileIdedx;
		std::string fileLdedx;
		std::string fileHdedx;
		std::string fileRunDepPar;
		std::string fileGeometry;
		std::string fileTCorrIC;

		Bool_t boolRunList;
		Bool_t boolICGates;
		Bool_t boolIC;
		Bool_t boolFGate;
		Bool_t boolNGate;
		Bool_t boolFSdGate;
		Bool_t boolNSdGate;
		Bool_t boolFYuGate;
		Bool_t boolNYuGate;
		Bool_t boolFSuGate;
		Bool_t boolNSuGate;
		Bool_t boolELoss;
		Bool_t boolIdedx;
		Bool_t boolLdedx;
		Bool_t boolHdedx;
		Bool_t boolRunDepPar;
		Bool_t boolGeometry;
		Bool_t boolEssential;
		Bool_t boolTCorrIC;

		//virtual void ReadCalibPar(char* line);
		virtual void ReadFilenames(char* line);
		virtual void Load(TString filename);
		virtual void Print();
		virtual void Clear();
//		ClassDef(CalibPHYSICS,1)
};

#endif
// end
