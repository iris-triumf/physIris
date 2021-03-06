// experiment geometry
#ifndef geometry_H
#define geometry_H
#include <TObject.h>
#include <TClass.h>
#include <string>

class geometry : public TObject {
	public:
		geometry(); 
		virtual ~geometry() {} //! 
		
		Double_t TargetThickness;
		Int_t TargetOrientation;
		Double_t FoilThickness;
		Double_t xShift;
		Double_t yShift;
		Double_t YdThickness[8];
		Double_t YdDistance;
		Double_t YuThickness[8];
		Double_t YuDistance;
		Double_t YdInnerRadius;
		Double_t YdOuterRadius;
		Double_t Sd1Thickness;
		Double_t Sd1Distance;
		Double_t Sd2Thickness;
		Double_t Sd2Distance;
		Double_t SuThickness;
		Double_t SuDistance;
		Double_t SdInnerRadius;
		Double_t SdOuterRadius;

		void ReadGeometry(std::string);
		void Print();
};
#endif
// end
