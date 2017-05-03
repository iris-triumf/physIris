// HandlePHYSICS.h
#ifndef HandlePHYSICS_H
#define HandlePHYSICS_H
#include "TChain.h"
#include "TEvent.h"
#include "IDet.h"
#include "ITdc.h"
#include "IScaler.h"
#include "PTrack.h"
#include <string>

TChain* createChain(std::vector<Int_t>, std::string);
void HandlePHYSICS();
void  HandleBOR_PHYSICS(std::string, std::string, std::string, std::string);
void  HandleEOR_PHYSICS();
Double_t Qvalue (Double_t ma, Double_t mA, Double_t mb,Double_t E1, Double_t Eb, Double_t thetaR);
#endif
// end

