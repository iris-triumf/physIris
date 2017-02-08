// Eloss.h

#ifndef EneLoss_H
#define EneLoss_H

#include "TGraph.h"
// #include "nucleus.h"
//Extern

void loadELoss(std::string, Double_t[100], Double_t[100], Double_t);
Double_t eval(Double_t, Double_t[100], Double_t[100]);
Double_t eloss(Double_t, Double_t, TGraph *);
Double_t eloss(Double_t, Double_t, Double_t[100], Double_t[100]);
// Double_t simEloss(nucleus, Double_t, Double_t, Double_t, Double_t[100], Double_t[100]);
Double_t erem(Double_t, TGraph *);
Double_t elossFi(Double_t, Double_t, TGraph *);
Double_t elossFi(Double_t, Double_t, Double_t[100], Double_t[100]);
Double_t thickness(Double_t, Double_t, TGraph *);//initial energy and final energy are given as arguments, calculates target thickness 
Double_t thickness(Double_t, Double_t, Double_t[100], Double_t[100]);

Double_t fncBethe(Double_t *x, Double_t *par); //Bethe like function for fitting    
// Double_t straggling(nucleus, Double_t, Double_t); //Energy straggling    
#endif
// end
