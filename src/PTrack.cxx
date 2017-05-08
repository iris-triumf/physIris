#include "PTrack.h"

PTrack::PTrack()
{
	E = 0.;
	Ecm = 0.;
	T = sqrt(-1);
	Tdeg = sqrt(-1);
	Tcm = sqrt(-1);
	P = sqrt(-1);
	Pdeg = sqrt(-1);
	AgdE = 0.;
	TrgtdE = 0.;
	Ebt = 0.;
}

void PTrack::Clear()
{
	E = 0.;
	Ecm = 0.;
	T = sqrt(-1);
	Tdeg = sqrt(-1);
	Tcm = sqrt(-1);
	P = sqrt(-1);
	Pdeg = sqrt(-1);
	AgdE = 0.;
	TrgtdE = 0.;
	Ebt = 0.;
}
