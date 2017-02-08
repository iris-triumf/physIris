//IScaler.cxx

#include "IScaler.h"
#include <math.h>
IScaler::IScaler(){
  	IScaler::Clear();
}

void IScaler::Clear()
{
	EventID = 0;

	ICSum = 0;
	ScintSum = 0;
	AccepTrigSum = 0;
	FreeTrigSum = 0;
	
	ICRate = 0;
	ScintRate = 0;
	AccepTrigRate = 0;
	FreeTrigRate = 0;
}

