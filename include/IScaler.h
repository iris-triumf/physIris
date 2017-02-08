// IScaler.h

#ifndef IScaler_H
#define IScaler_H
#include <vector>
#include <TObject.h>
#include <TClass.h>

class IScaler : public TObject {
 	public:  		
		IScaler(); //! Create
  		virtual ~IScaler() {} //!  
	
  		Int_t EventID;
  		
		Int_t ICSum;
  		Int_t ScintSum;
  		Int_t AccepTrigSum;
  		Int_t FreeTrigSum;

  		Int_t ICRate;
  		Int_t ScintRate;
  		Int_t AccepTrigRate;
  		Int_t FreeTrigRate;

  		void Clear();
  		ClassDef(IScaler,1)
};


#endif
// end

