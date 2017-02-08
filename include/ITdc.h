// ITdc.h

#ifndef ITdc_H
#define ITdc_H
#include <vector>
#include <TObject.h>
#include <TClass.h>

class ITdc : public TObject {
 	public:
  		ITdc(); //! Create
  		virtual ~ITdc() {} //!  
		
		//Int_t TICTDC;
		Int_t TICTMul;
		std::vector<Double_t> TICTChannel;
		std::vector<Double_t> TICTime;
		std::vector<Double_t> TICTimeRF;
		
		//Int_t TYdTDC;
		Int_t TYdTMul;
		std::vector<Double_t> TYdTChannel;
		std::vector<Double_t> TYdTime;
		std::vector<Double_t> TYdTimeRF;
		
		//Int_t TYuTDC;
		Int_t TYuTMul;
		std::vector<Double_t> TYuTChannel;
		std::vector<Double_t> TYuTime;
		std::vector<Double_t> TYuTimeRF;
		
		//Int_t TSurTDC;
		Int_t TSurTMul;
		std::vector<Double_t> TSurTChannel;
		std::vector<Double_t> TSurTime;
		std::vector<Double_t> TSurTimeRF;
		
		//Int_t TSusTDC;
		Int_t TSusTMul;
		std::vector<Double_t> TSusTChannel;
		std::vector<Double_t> TSusTime;
		std::vector<Double_t> TSusTimeRF;
		
		//Int_t TSd1rTDC;
		Int_t TSd1rTMul;
		std::vector<Double_t> TSd1rTChannel;
		std::vector<Double_t> TSd1rTime;
		std::vector<Double_t> TSd1rTimeRF;
		
		//Int_t TSd1sTDC;
		Int_t TSd1sTMul;
		std::vector<Double_t> TSd1sTChannel;
		std::vector<Double_t> TSd1sTime;
		std::vector<Double_t> TSd1sTimeRF;
		
		//Int_t TSd2rTDC;
		Int_t TSd2rTMul;
		std::vector<Double_t> TSd2rTChannel;
		std::vector<Double_t> TSd2rTime;
		std::vector<Double_t> TSd2rTimeRF;
		
		//Int_t TSd2sTDC;
		Int_t TSd2sTMul;
		std::vector<Double_t> TSd2sTChannel;
		std::vector<Double_t> TSd2sTime;
		std::vector<Double_t> TSd2sTimeRF;

  		void Clear();
  		ClassDef(ITdc,1)
};
#endif
// end
