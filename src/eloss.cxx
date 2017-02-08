//Alisher Sanetullaev
//Energy loss calculator using Bethe formula
// modified by Matthias Holl, December 2015
// stopping power can now be directly read from file 

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <TRandom3.h>
#include <TMath.h>
#include "eloss.h"

void loadELoss(std::string filename, Double_t e[100], Double_t dedx[100], Double_t m)
{
	std::ifstream infile;
 	Char_t line[2000];
                                                                                                                                                                                                               
	infile.open(filename.data());
	if(!infile.is_open()){
		printf("Cannot open the file %s!!\n",filename.data());
		exit(0);
	}
	infile.getline(line,2000);
	for(UInt_t i=0; i<100; i++){
		infile.getline(line,2000);
		sscanf(line,"%*f\t%*f\t%lf\t%lf\t%*f\t%*f\t%*f\t%*f\t%*f\t%*f\t%*f\t%*f\t\n",&e[i],&dedx[i]);
		e[i] *= m/931.494061; //stopping power for this mass
	}
	infile.close();
}

Double_t fncBethe(Double_t *x, Double_t *par) //Bethe like function for fitting
{
  Double_t val=0;
 
  //  if (par[1]!=0) val = par[0]*log(x[0]*par[1])/x[0]; //Bethe
  //  if (par[1]>0) val = par[0]*TMath::Log(x[0]/par[1])/x[0]; //Bethe
  //val = par[0]+par[1]*x[0]+ par[2]*x[0]*x[0];//quadratic function
   val = par[0]+par[1]*x[0]+ par[2]*x[0]*x[0]+ par[3]*x[0]*x[0]*x[0]+ par[4]*x[0]*x[0]*x[0]*x[0]+par[5]*x[0]*x[0]*x[0]*x[0]*x[0]+par[6]*x[0]*x[0]*x[0]*x[0]*x[0]*x[0];
     // val = par[0]+par[1]*x[1]+ par[2]*x[1]*x[1]+ par[3]*x[1]*x[1]*x[1]+ par[4]*x[1]*x[1
  /***Fit

  ****************************************************************************************
*/

  return val;
}

Double_t eval(Double_t in, Double_t x[100], Double_t y[100])
{
	Double_t dxin=0., dx=0., dy=0., e=0.;
	if(in<=0.){
		e = 0.;
	}
	else if(in<x[0]){
		e = y[0]*in/x[0];
	}
	else if(in>x[99]){
		dxin = in-x[99];
		dx = x[99]-x[98];
		dy = y[99]-y[98];
		e = y[99]+dy*dxin/dx;
	}
	else{
		for(int i=1; i<100;i++){   
			if(in>x[i-1]&&in<x[i]){
				dxin = in-x[i-1];
				dx = x[i]-x[i-1];
				dy = y[i]-y[i-1];
				e = y[i-1]+dy*dxin/dx;
				break;
			}
		}
	}
	return e;
}

//Make it a method for a particle class.
// Double_t simEloss(nucleus P, Double_t TZoverA, Double_t ein, Double_t th , Double_t x[100], Double_t y[100])//initial energy and thickness are given as arguments 
// {
// 	Double_t k;
// 	Double_t Bohr;
// 	Double_t sgm;
// 	Double_t strg;
// 
// 	TRandom3 *rndm = new TRandom3(0);
// 	if(ein==0.) return 0;
// 	//Energy loss calculation including energy Straggling
// 	Double_t dx =th/100.; //in mg/cm2
// 	Bohr = TMath::Sqrt(0.157/1000. * dx * P.Z*P.Z *TZoverA);
// 	Double_t de = 0; //energy loss
// 	Double_t en= ein; //the energy variable
// 	for (int i=0; i<100; i++){
// 	  	de = (dx * eval(en,x,y));//energy loss in dx
// 		if(de>en){
// 		   	en=0.;	
// 			break;
// 		}
// 		k= 1.1+0.47*TMath::Log10(en/Double_t(P.A));
// 		sgm = k*Bohr;
// 		strg = rndm->Gaus(de,sgm);
// 		de = (strg>0.) ? strg : 0.;
// 	  	if(de>en){
// 		   	en=0.;	
// 			break;
// 		}
// 		en = en - de; // energy remaining after dx
// 	}
// 	return ein-en;
// }

//Make it a method for a particle class.
Double_t eloss(Double_t ein, Double_t th , Double_t x[100], Double_t y[100])//initial energy and thickness are given as arguments 
{
	if(ein==0.) return 0;
	//Energy loss calculation
	Double_t dx =th/100.; //in mg/cm2
	Double_t de = 0; //energy loss
	Double_t en= ein; //the energy variable
	for (int i=0; i<100; i++){
	  	de = (dx * eval(en,x,y));//energy loss in dx
		if(de>en){
		   	en=0.;	
			break;
		}
	  	en = en - de; // energy remaining after dx
	}
	return ein-en;
}

Double_t eloss(Double_t ein, Double_t th , TGraph *func)//initial energy and thickness are given as arguments 
{
	//Energy loss calculation
	Double_t dx =th/100.; //in mg/cm2
	Double_t de = 0; //energy loss
	Double_t en= ein; //the energy variable
	//Double_t pos = 0.;// the position variable
	//while (pos <= th){
	for (int i=0; i<100; i++){
	  	// de = (dx * func->Eval(en))/2.; //
	   	// en = en - de;
	  	// if(de>en){
		//    	en=0.;	
		// 	break;
		// }
		de = (dx * func->Eval(en));//energy loss in dx
	  	if(de>en){
		   	en=0.;	
			break;
		}
		en = en - de; // energy remaining after dx
		//pos = pos + dx;
	}
	return ein-en;
}
//Make it a method for a particle class.
Double_t elossFi(Double_t efi, Double_t th, Double_t x[100], Double_t y[100])//final energy and thickness are given as arguments 
{
	if (th==0) return efi;
	//Energy loss calculation
	Double_t dx =th/100.; //in 
	Double_t de = 0; //energy loss
	Double_t en= efi; //the energy variable
	Double_t pos = 0.;// the position variable
	
	while (pos<= th){
		de =  (dx* eval(en,x,y))/2.; //
		en = en + de;
		de =(dx* eval(en,x,y))/2.;//energy loss in dx
		en = en +de; // energy remaining after dx
		pos = pos +dx;
	}
 	return en-efi;
}

Double_t elossFi(Double_t efi, Double_t th , TGraph *func)//final energy and thickness are given as arguments 
{

 // for (Int_t )
    if (th==0) return efi;

 //Energy loss calculation
 Double_t dx =th/100.; //in 
 Double_t de = 0; //energy loss
 Double_t en= efi; //the energy variable
 Double_t pos = 0.;// the position variable
 
 while (pos<= th){
   de =  (dx*func->Eval(en))/2.; //
en = en + de;
   de =(dx*func->Eval(en))/2.;//energy loss in dx
   en = en +de; // energy remaining after dx

   pos = pos +dx;

 }
 return en-efi;
}
//Make it a method for a particle class.
Double_t thickness(Double_t ein, Double_t efi, Double_t x[100], Double_t y[100])//initial energy and final energy are given as arguments, calculates target thickness 
{

 // for (Int_t )
  if (ein<=efi) return 0.0;
 //Energy loss calculation
  Double_t dx = 0; 
  Double_t de = (ein-efi)/1000.; //energy loss step in MeV
  //  Double_t de = 0.01; //energy loss step in MeV
 Double_t en= ein; //the energy variable
 Double_t th = 0.;// the thickness variable

 //Integrate numerically 
 
 while (en > efi){
   dx = de/eval(en,x,y)/2.; //
   th = th +dx;
   en = en-de;

   dx =  de/eval(en,x,y)/2.; //
   th = th +dx;
   
 }
 return th;
}

//Make it a method for a particle class.
Double_t thickness(Double_t ein, Double_t efi , TGraph *func)//initial energy and final energy are given as arguments, calculates target thickness 
{

 // for (Int_t )
  if (ein<=efi) return 0.0;
 //Energy loss calculation
  Double_t dx = 0; 
  Double_t de = (ein-efi)/1000.; //energy loss step in MeV
  //  Double_t de = 0.01; //energy loss step in MeV
 Double_t en= ein; //the energy variable
 Double_t th = 0.;// the thickness variable

 //Integrate numerically 
 
 while (en > efi){
   dx =  de/func->Eval(en)/2.; //
   th = th +dx;
   en = en-de;

   dx =  de/func->Eval(en)/2.; //
   th = th +dx;
   
 }
 return th;
}


// Remaining energy calculator
Double_t erem(Double_t eLoss, TGraph *func) //energy loss function for remaining energy calculation for the given energy loss and energy loss function as arguments
{
  Double_t val;
  TGraph *gr2 = new TGraph(func->GetN(), func->GetY(), func->GetX());
   // if(draw) gr->Draw("AP");
  val = gr2->Eval(eLoss)-eLoss;
if (val<0)  val = 0.;
  return val;
}
