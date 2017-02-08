// HandlePhysics.cxx
// calculating Physics variables requiring data from more than one detector

#include <iostream>
#include <assert.h>
#include <fstream>
#include <string>
#include <vector>

#include <TFile.h>
#include <TChain.h>
#include <TMath.h>
#include "TCutG.h"
//#include "HandleMesytec.h"
#include "HandlePHYSICS.h"
//#include "Globals.h"
#include "eloss.h"
#include "nucleus.h"
#include "runDepPar.h"
#include "CalibPHYSICS.h"
#include "Graphsdedx.h"
#include "geometry.h"

//#define pd // (p,d) reaction analysis
extern TEvent *IrisEvent;
extern TFile* treeFile;
extern TTree* tree;

TChain *input_chain;

CalibPHYSICS calPhys;
geometry geoP;
Graphsdedx dedx_i, dedx_l, dedx_h;

float ICELossCorr;

const int Nchannels = 24;
const int binlimit = 1900;
 	
Double_t eATgt[100], eAIso[100], eAWndw[100], eAAg[100];	
Double_t dedxATgt[100], dedxAIso[100], dedxAWndw[100], dedxAAg[100];	
Double_t eBSi[100], eBTgt[100], eBSiO2[100], eBB[100], eBP[100], eBAl[100], eBMy[100], eBCsI[100];	
Double_t dedxBSi[100], dedxBTgt[100], dedxBSiO2[100], dedxBB[100], dedxBP[100], dedxBAl[100], dedxBMy[100], dedxBCsI[100];	
Double_t ebTgt[100], ebSi[100], ebAl[100], ebB[100], ebMy[100], ebP[100], ebCsI[100], ebSiO2[100];	
Double_t dedxbTgt[100], dedxbSi[100], dedxbAl[100], dedxbB[100], dedxbMy[100], dedxbP[100], dedxbCsI[100], dedxbSiO2[100];	

double EBAC = 0.; //Beam energy from accelerator
runDep runDepPar; // run dependant parameters
nucleus beam; // beam particle properties
nucleus target; // target properties
nucleus lej; // light ejetcile properties
nucleus hej; // heavy ejectile properties  
Int_t useYCalc = 0;//Use calculated YY1 energy  : do we need to ? :Jaspreet
Double_t PResid; //Momentum of residue
Double_t PBeam; // Calculated beam momentum after scattering off Ag
Double_t PA; //Beam momentum before reaction
Double_t Pb1; //Light ejectile momentum
Double_t Pb2; //Light ejectile momentum

Double_t A,B,C; //Used for quadratic equations
Double_t MBeam = 0.; // Beam mass
Double_t kBF = 108.904752/(MBeam/931.494013); //Ratio of Beam particle mass and 109-Ag foil nucleus mass
const Double_t MFoil = 931.494013*108.; //Ag foil mass AS
//Double_t geoP.FoilThickness = 5.7;                      /// ?????????? confrim if 5.7 or 5.44 --Jaspreet
Double_t energy = 0;
Double_t cosTheta = 1.;// S3 angle cosine
Double_t EBeam;
Double_t betaCM, gammaCM; //CM velocity

Double_t mA= 0.; //11.;//Beam mass //Reassigned in HandleBOR
Double_t ma = 0.; //target mass
Double_t mb= 0.; //1.;//Light ejectile mass
Double_t mB= 0.; // Heavy ejectile mass

Double_t thetaR=sqrt(-1.);
Double_t thetaD=sqrt(-1.);
Double_t Eb1=sqrt(-1.);
Double_t Eb2=sqrt(-1.);
Double_t ECsI1=sqrt(-1.);
Double_t ECsI2=sqrt(-1.);
Double_t EYY1=sqrt(-1.);
Double_t Q1=sqrt(-1.);
Double_t Q2=sqrt(-1.);
Double_t thetaCM1=sqrt(-1.);
Double_t thetaCM2=sqrt(-1.);
Double_t Pb1y=sqrt(-1.);
Double_t Pb2y=sqrt(-1.);
Double_t Pb1xcm=sqrt(-1.);
Double_t Pb2xcm=sqrt(-1.);
	
TCutG *gate;

IDet detec; // calibrated variables from detectors, to be passed to HandlePhysics
IDet *det = &detec;
ITdc timeArray;
ITdc *tdc = &timeArray;
IScaler scal;
IScaler *pscaler = &scal;

TChain* createChain(std::vector<Int_t> runs, std::string Directory)
{
    printf("***Creating New TChain***\n");
  	
	TChain *ch = new TChain("Iris");
	Int_t nRuns = runs.size();
	cout << "Adding " << nRuns << " runs!\n";
  	for(int i=0; i< nRuns; ++i) {
		std::string filename;
		std::string runNo;
		if(runs.at(i)>99999) runNo = Form("%d",runs.at(i));
		else runNo = Form("0%d",runs.at(i));
    	filename = Directory + "/tree" + runNo + ".root";
    	
		int addReturn = ch->AddFile(filename.data());
    	TFile tFile(filename.data());
    	if(addReturn == 1 && !tFile.IsZombie()) {
      		printf("Adding run: %s\n",filename.data());
    	} 
		else{
      		printf("WARNING: Tried to add an invalid file: %s ==> Skipping.\n",filename.data());
    	}
  	}
  	cout << "----------\n\n";
  	return ch;
}

//---------------------------------------------------------------------------------
void HandleBOR_PHYSICS(std::string Directory, std::string CalibFile, std::string OutputFile)
{
  	printf("In HandleBOR_PHYSICS...\n");	
	if(CalibFile=="") printf("No calibration file specified!\n\n");
	calPhys.Load(CalibFile);
	calPhys.Print();

	Int_t runTmp;
	std::vector<Int_t> runs;
	ifstream rFile(calPhys.fileRunList.data());
	char rLine[256]; 
	if (rFile == NULL || calPhys.boolRunList==false) {
		printf("No list of runs.\n");
   	}  
 	else  {
		printf("Reading run List '%s'\n",calPhys.fileRunList.data());

		while(!rFile.eof()){
			rFile.getline(rLine,256);
			printf("%s\n",rLine);
       		sscanf(rLine,"%*s %*s %*s %d",&runTmp);
			if(runs.size()==0 || runTmp>runs.at(runs.size()-1)) runs.push_back(runTmp);
     	}
     	rFile.close();
		printf("\n");
 	}

	input_chain = createChain(runs,Directory);
	input_chain->SetBranchAddress("det",&det);
	input_chain->SetBranchAddress("scaler",&pscaler);
	treeFile = new TFile(OutputFile.data(),"RECREATE");
	tree=input_chain->CloneTree(0);	
	IrisEvent = new TEvent();
 	tree->Branch("IrisEvent","TEvent",&IrisEvent,32000,99);

	TFile *fgates = new TFile(calPhys.fileGate.data());
   	printf("opened file %s\n",calPhys.fileGate.data());
  
	gate = (TCutG*)fgates->FindObjectAny(calPhys.nameGate.data());
  	if(!gate) printf("No gate.\n");  
	else printf("Grabbed gate %s.\n",calPhys.nameGate.data());
	
	geoP.ReadGeometry(calPhys.fileGeometry.data());
	geoP.Print();

	ICELossCorr = 1.;	
//	// Time dependent correction of IC energy loss 
//	FILE *pFile;
//	int Chan = 0;
//	double a,b;
//	int run_for_corr = 0;
//	
//	if (pFile == NULL || calPhys.boolTCorrIC==false) {
//		//fprintf(logFile,"No time dependent correction for IC energy loss. Skipping correction.\n");
//		printf("No time dependent correction for IC energy loss. Skipping correction.\n");
//		ICELossCorr =1.;
//	}
//	else  {
//		printf("Reading config file '%s'\n",calPhys.fileTCorrIC.data());
//
//		while (!feof(pFile)){
//    		fscanf(pFile,"%d%lf%lf",&Chan,&a,&b);
//			if(Chan==run){ 
//				run_for_corr = Chan;
//				ICELossCorr = b; 
//			}
//    	}
//    	fclose (pFile);	
//
//		if(run_for_corr==0){ 
//			printf("Run %d not in list. No correction applied!\n",run);
//		}
//		else{
//			printf("Run: %d\tIC Gain correction: %f\n\n",Chan,ICELossCorr);
//		}
//  	}

	if(calPhys.boolRunDepPar){
		runDepPar.setRunDepPar(calPhys.fileRunDepPar);// setting run dependent parameters.
		runDepPar.Print();

		beam.getInfo(runDepPar.nA);
		target.getInfo(runDepPar.na);
		hej.getInfo(runDepPar.nB);
		lej.getInfo(runDepPar.nb);

		EBAC = runDepPar.EBAC;
		mA = beam.mass; //Beam mass //Reassigned in HandleBOR
		ma = target.mass;
		mb = lej.mass; //Light ejectile mass
		mB = hej.mass;
    	printf("mA=%f, ma=%f, mb=%f, mB=%f\n",mA,ma,mb,mB); 
		kBF = MFoil/mA;
	
		printf("Beam energy: %f\n", runDepPar.energy);
		// printf("Target thickness: %f\n",geoP.TargetThickness);
	
		if(calPhys.boolIdedx==kTRUE){
			printf("\n\nLoading dedx Graphs for incoming %s ...\n",runDepPar.nA.data());
			dedx_i.Load(calPhys.fileIdedx);
			dedx_i.Print();
			if(dedx_i.boolAg==kTRUE) loadELoss(dedx_i.Ag,eAAg,dedxAAg,mA);	
			if(dedx_i.boolTgt==kTRUE) loadELoss(dedx_i.Tgt,eATgt,dedxATgt,mA);	
			if(dedx_i.boolSi==kTRUE) loadELoss(dedx_i.Si,eBSi,dedxBSi,mB);	
			if(dedx_i.boolAl==kTRUE) loadELoss(dedx_i.Al,eBAl,dedxBAl,mB);	
			if(dedx_i.boolB==kTRUE) loadELoss(dedx_i.B, eBB,dedxBB,mB);	
			if(dedx_i.boolP==kTRUE) loadELoss(dedx_i.P, eBP,dedxBP,mB);	
			if(dedx_i.boolSiO2==kTRUE) loadELoss(dedx_i.SiO2,eBSiO2,dedxBSiO2,mB);	
			if(dedx_i.boolIso==kTRUE) loadELoss(dedx_i.Iso,eAIso,dedxAIso,mA);	
			if(dedx_i.boolWndw==kTRUE) loadELoss(dedx_i.Wndw,eAWndw,dedxAWndw,mA);	
		}

		if(calPhys.boolLdedx==kTRUE){
			printf("\n\nLoading dedx Graphs for target-like %s...\n",runDepPar.nb.data());
			dedx_l.Load(calPhys.fileLdedx);
			dedx_l.Print();
			if(dedx_l.boolSi==kTRUE) loadELoss(dedx_l.Si,ebSi,dedxbSi,mb);	
			if(dedx_l.boolAl==kTRUE) loadELoss(dedx_l.Al,ebAl,dedxbAl,mb);	
			if(dedx_l.boolB==kTRUE) loadELoss(dedx_l.B,ebB,dedxbB,mb);	
			if(dedx_l.boolP==kTRUE) loadELoss(dedx_l.P,ebP,dedxbP,mb);	
			if(dedx_l.boolMy==kTRUE) loadELoss(dedx_l.My,ebMy,dedxbMy,mb);	
			if(dedx_l.boolTgt==kTRUE) loadELoss(dedx_l.Tgt,ebTgt,dedxbTgt,mb);	
		}

		if(calPhys.boolHdedx==kTRUE){
			printf("\n\nLoading dedx Graphs for beam-like %s...\n",runDepPar.nB.data());
			dedx_h.Load(calPhys.fileHdedx);
			dedx_h.Print();
			if(dedx_h.boolAg==kTRUE) loadELoss(dedx_h.Ag,eAAg,dedxAAg,mA);	
			if(dedx_h.boolTgt==kTRUE) loadELoss(dedx_h.Tgt,eATgt,dedxATgt,mA);	
			if(dedx_h.boolSi==kTRUE) loadELoss(dedx_h.Si,eBSi,dedxBSi,mB);	
			if(dedx_h.boolAl==kTRUE) loadELoss(dedx_h.Al,eBAl,dedxBAl,mB);	
			if(dedx_h.boolB==kTRUE) loadELoss(dedx_h.B, eBB,dedxBB,mB);	
			if(dedx_h.boolP==kTRUE) loadELoss(dedx_h.P, eBP,dedxBP,mB);	
			if(dedx_h.boolSiO2==kTRUE) loadELoss(dedx_h.SiO2,eBSiO2,dedxBSiO2,mB);	
			if(dedx_h.boolIso==kTRUE) loadELoss(dedx_h.Iso,eAIso,dedxAIso,mB);	
			if(dedx_h.boolWndw==kTRUE) loadELoss(dedx_h.Wndw,eAWndw,dedxAWndw,mB);	
		}
		//Needs to be moved!
		const Double_t ICLength=22.9*0.062; //cm*mg/cm^3 at 19.5 Torr
		const Double_t ICWindow1=0.03*3.44*0.1; //mu*g/cm^3*0.1
		const Double_t ICWindow2=0.05*3.44*0.1; //mu*g/cm^3*0.1
	
		Double_t temp_energy = runDepPar.energy;
		runDepPar.energy = runDepPar.energy-eloss(runDepPar.energy,ICWindow1,eAWndw,dedxAWndw);  
		runDepPar.energy = runDepPar.energy-eloss(runDepPar.energy,ICLength,eAIso,dedxAIso)/ICELossCorr;  
		runDepPar.energy = runDepPar.energy-eloss(runDepPar.energy,ICWindow2,eAWndw,dedxAWndw);  
		printf("Energy loss in IC (including windows): %f\n" ,temp_energy-runDepPar.energy);

		temp_energy = runDepPar.energy;
		runDepPar.energy = runDepPar.energy-eloss(runDepPar.energy,geoP.FoilThickness,eAAg,dedxAAg);  
		printf("Energy loss in silver foil: %f\n" ,temp_energy-runDepPar.energy);
		printf("Energy after silver foil: %f\n",runDepPar.energy);

		temp_energy = runDepPar.energy;
		runDepPar.energy = runDepPar.energy-eloss(runDepPar.energy,geoP.TargetThickness/2.,eATgt,dedxATgt);  
		printf("Energy loss in half target: %f\n" ,temp_energy-runDepPar.energy);

		runDepPar.momentum = sqrt(runDepPar.energy*runDepPar.energy+2.*runDepPar.energy*beam.mass);//beam momentum
		runDepPar.beta = runDepPar.momentum/(runDepPar.energy + beam.mass + target.mass);
		runDepPar.gamma = 1./sqrt(1.-runDepPar.beta*runDepPar.beta);

		EBeam= runDepPar.energy;
		PA = runDepPar.momentum;//beam momentum
		betaCM = runDepPar.beta;
		gammaCM = runDepPar.gamma;
	
		printf("Resulting energy: %f\n",EBeam);
		printf("Beam momentum: %f\n",PA);
		printf("Beam beta: %f\tBeam gamma: %f\n",betaCM,gammaCM);
		printf("MBeam: %f\t MFoil: %f\t kBF: %f\n",beam.mass,MFoil,kBF);
		printf("beam mass: %f MeV (%f)\ttarget mass: %f MeV (%f)\n",mA,mA/931.494061,ma,ma/931.494061);
		printf("heavy ejectile mass: %f MeV (%f)\tlight ejectile mass: %f MeV (%f)\n",mB,mB/931.494061,mb,mb/931.494061);
	}
 	if (calPhys.boolICGates==kFALSE){
		runDepPar.ICmin=0;
		runDepPar.ICmax=4096;
	}
	
//--------------------------------------------------------------------------------
		printf("End of HandleBOR_Physics\n");
} //HandleBOR_Physics

void HandlePHYSICS()
{
	Int_t nEntries = input_chain->GetEntries();
	printf("%d entries in total.\n",nEntries);
	for(Int_t i=0; i<nEntries; i++)
	{
		Long64_t check_entry = input_chain->LoadTree(i);
		if(check_entry<0) break;
		input_chain->GetEntry(i);
		if((i%100)==0) printf("Processing event %d\r",i);
		
		if (det->TICEnergy.size()==0) continue; 
		if (det->TICEnergy.at(0)<runDepPar.ICmin || det->TICEnergy.at(0)>runDepPar.ICmax) continue; 
		if (det->TYdEnergy.size()==0||det->TCsI1Energy.size()==0) continue; 
		if (gate->IsInside(det->TCsI1Energy.at(0),det->TYdEnergy.at(0)*cos(TMath::DegToRad()*det->TYdTheta.at(0)))==0) continue; 
		IrisEvent->fEBAC = EBAC;
		IrisEvent->fmA = mA;
		IrisEvent->fma = ma;
		IrisEvent->fmB = mB;
		IrisEvent->fmb = mb;
		IrisEvent->fkBF = kBF;
		IrisEvent->fEBeam = EBeam;
		IrisEvent->fbetaCM = betaCM;
		IrisEvent->fgammaCM = gammaCM;
		IrisEvent->fPA = PA;
 		
		//adding dead layer energy losses
		//Sd2 ring side
		if(det->TSd1rEnergy.size()>0 && det->TSd2rEnergy.size()>0){
			if(det->TSd1rEnergy.at(0)>0. && det->TSd2rEnergy.at(0)>0.){
				cosTheta = cos(TMath::DegToRad()* (det->TSdTheta.at(0)));
				energy = det->TSd2rEnergy.at(0);
				energy = energy+elossFi(energy,0.1*2.35*0.5/cosTheta,eBB,dedxBB); //boron junction implant
				energy = energy+elossFi(energy,0.1*2.7*0.3/cosTheta,eBAl,dedxBAl); //first metal
				energy = energy+elossFi(energy,0.1*2.65*2.5/cosTheta,eBSiO2,dedxBSiO2); //SiO2
				energy = energy+elossFi(energy,0.1*2.7*1.5/cosTheta,eBAl,dedxBAl); //second metal
				//Sd1 ring side
				energy = energy+elossFi(energy,0.1*2.7*1.5/cosTheta,eBAl,dedxBAl); //second metal
				energy = energy+elossFi(energy,0.1*2.65*2.5/cosTheta,eBSiO2,dedxBSiO2); //SiO2
				energy = energy+elossFi(energy,0.1*2.7*0.3/cosTheta,eBAl,dedxBAl); //first metal
				energy = energy + det->TSd1rEnergy.at(0);// energy lost and measured in Sd1

				//sector side
				energy = energy+elossFi(energy,0.1*1.822*0.5/cosTheta,eBP,dedxBP); //phosphorus implant
				det->TSdETot = energy+elossFi(energy,0.1*2.7*0.3/cosTheta,eBAl,dedxBAl); //metal
			}
		
			PResid = sqrt(2.*det->TSdETot*mA);     //Beam momentum in MeV/c
			A = kBF-1.;                              //Quadratic equation parameters
		    B = 2.0*PResid* cos(TMath::DegToRad()*det->TSdTheta.at(0));
		    C = -1.*(kBF+1)*PResid*PResid; 
		    if (A!=0)    PBeam = (sqrt(B*B-4.*A*C)-B)/(2*A);
		  	IrisEvent->fPBeam = PBeam;
		  	IrisEvent->fPResid = PResid;
		  	IrisEvent->fA = A;
		  	IrisEvent->fB = B;
		  	IrisEvent->fC = C;
		  	//to calculate residue energy from beam
		    
			IrisEvent->fEB = PBeam*PBeam/(2.*mA);
		   
		    IrisEvent->fEB=  IrisEvent->fEB + elossFi(det->TSdETot,geoP.FoilThickness/2.,eAAg,dedxAAg); //energy loss from the end of H2 to the center of Ag.
		    det->TSdThetaCM = TMath::RadToDeg()*atan(tan(TMath::DegToRad()*det->TSdTheta.at(0))/sqrt(gammaCM-gammaCM*betaCM*(mA+IrisEvent->fEB)/(PBeam*cos(TMath::DegToRad()*det->TSdTheta.at(0)))));// check if this is still correct for H2 target tk
		}

		if (det->TYdEnergy.size()>0&&det->TYdRing.size()>0) {    //check if in the proton/deuteron gate
		  	thetaR = atan((geoP.YdInnerRadius+((det->TYdRing.at(0)+0.5)*(geoP.YdOuterRadius-geoP.YdInnerRadius)/16))/geoP.YdDistance);
		  	// thetaR =( atan((geoP.YdInnerRadius+((det->TYdRing.at(0)+1)*(geoP.YdOuterRadius-geoP.YdInnerRadius)/16))/geoP.YdDistance)
			//	   	+ atan((geoP.YdInnerRadius+((det->TYdRing.at(0))*(geoP.YdOuterRadius-geoP.YdInnerRadius)/16))/geoP.YdDistance) )/2.;
  //thetaR =( atan((Yd1r+((det->TYdRing+1)*(Yd2r-Yd1r)/16))/YdDistance) + atan((Yd1r+((det->TYdRing)*(Yd2r-Yd1r)/16))/YdDistance) )/2.;
		  	thetaD = thetaR*TMath::RadToDeg();
			IrisEvent->fThetaD = thetaD;
			EYY1 = det->TYdEnergy.at(0);
		}	
		if (det->TCsI1Energy.size()>0&&det->TYdEnergy.size()>0&&det->TYdRing.size()>0) {    //check if in the proton/deuteron gate
		
		    ECsI1= det->TCsI1Energy.at(0);
		
			ECsI1= ECsI1+elossFi(ECsI1,0.1*1.4*6./cos(thetaR),ebMy,dedxbMy); //Mylar                                                                                  
			ECsI1= ECsI1+elossFi(ECsI1,0.1*2.70*0.3/cos(thetaR),ebAl,dedxbAl); //0.3 u Al                                                                            
			ECsI1= ECsI1+elossFi(ECsI1,0.1*1.82*0.1/cos(thetaR),ebP,dedxbP); // 0.1Phosphorus                                                                      
		
			Eb1= ECsI1+EYY1; //use measured Yd // change june28
				
		    Eb1= Eb1+elossFi(Eb1,0.1*2.35*0.05/cos(thetaR),ebB,dedxbB); //0.05 u B 
		    Eb1= Eb1+elossFi(Eb1,0.1*2.70*0.1/cos(thetaR),ebAl,dedxbAl); //0.1 u Al
		  	IrisEvent->fEYY1 = Eb1-ECsI1;
		    Eb1= Eb1+elossFi(Eb1,geoP.TargetThickness/2./cos(thetaR),ebTgt,dedxbTgt); //deuteron energy  in mid target midtarget
		
			det->TYdCsI1ETot = Eb1;
			Pb1 = sqrt(Eb1*Eb1+2.*Eb1*mb);
			Pb1y = Pb1*sin(thetaR);
			Pb1xcm = gammaCM*betaCM*(Eb1+mb)- gammaCM*Pb1*cos(thetaR);
		 	Q1 = mA+ma-mb- sqrt(mA*mA+mb*mb-ma*ma-2.*(mA+EBeam)*(mb+Eb1)+2.*PA*Pb1*cos(thetaR)+2.*(EBeam+mA+ma-Eb1-mb)*ma);  //Alisher's equation 
			Double_t Eb1_nocorr = det->TYdEnergy.at(0)+det->TCsI1Energy.at(0);
			Double_t Pb1_nocorr = sqrt(Eb1_nocorr*Eb1_nocorr+2.*Eb1_nocorr*mb);
		 	Double_t Q1_nocorr = mA+ma-mb- sqrt(mA*mA+mb*mb-ma*ma-2.*(mA+EBeam)*(mb+Eb1_nocorr)+2.*PA*Pb1_nocorr*cos(thetaR)+2.*(EBeam+mA+ma-Eb1_nocorr-mb)*ma);// without dedx corr
		  	IrisEvent->fECsI1 = ECsI1;
		  	IrisEvent->fEb1 = Eb1;
		  	IrisEvent->fPb1 = Pb1;
		  	IrisEvent->fPb1y = Pb1y;
		  	IrisEvent->fPb1xcm = Pb1xcm;
		  	IrisEvent->fQv1 = Q1;
		  	IrisEvent->fQv1_nocorr = Q1_nocorr;
			thetaCM1 = TMath::RadToDeg()*atan(Pb1y/Pb1xcm);
			thetaCM1 = (thetaCM1<0) ? thetaCM1+180. : thetaCM1;
			IrisEvent->fThetacm1 = thetaCM1;
		}
		
		if (det->TCsI2Energy.size()>0&&det->TYdEnergy.size()>0&&det->TYdRing.size()>0) {    //check if in the proton/deuteron gate
		
		    ECsI2= det->TCsI2Energy.at(0);
		
		  	if (mb == target.mass) //proton energy loss in dead layers between YY1 and CsI                                                                                       
		    {
		      	ECsI2= ECsI2+elossFi(ECsI2,0.1*1.4*6./cos(thetaR),ebMy,dedxbMy); //Mylar                                                                                  
		      	ECsI2= ECsI2+elossFi(ECsI2,0.1*2.70*0.3/cos(thetaR),ebAl,dedxbAl); //0.3 u Al                                                                            
		      	ECsI2= ECsI2+elossFi(ECsI2,0.1*1.82*0.1/cos(thetaR),ebP,dedxbP); // 0.1Phosphorus                                                                      
		    }
		
			Eb2= ECsI2+EYY1; //use measured Yd // change june28
		
		   	if (mb == target.mass){
		      	Eb2= Eb2+elossFi(Eb2,0.1*2.35*0.05/cos(thetaR),ebB,dedxbB); //0.05 u B 
		      	Eb2= Eb2+elossFi(Eb2,0.1*2.70*0.1/cos(thetaR),ebAl,dedxbAl); //0.1 u Al
		    	Eb2= Eb2+elossFi(Eb2,geoP.TargetThickness/2./cos(thetaR),ebTgt,dedxbTgt); //deuteron energy  in mid target midtarget
			}
			det->TYdCsI2ETot = Eb2;
			Pb2 = sqrt(Eb2*Eb2+2.*Eb2*mb);
			Pb2y = Pb2*sin(thetaR);
			Pb2xcm = gammaCM*betaCM*(Eb2+mb)- gammaCM*Pb2*cos(thetaR);
		 	Q2 = mA+ma-mb- sqrt(mA*mA+mb*mb-ma*ma-2.*(mA+EBeam)*(mb+Eb2)+2.*PA*Pb2*cos(thetaR)+2.*(EBeam+mA+ma-Eb2-mb)*ma);  //Alisher's equation 
			Double_t Eb2_nocorr = det->TYdEnergy.at(0)+det->TCsI2Energy.at(0);
			Double_t Pb2_nocorr = sqrt(Eb2_nocorr*Eb2_nocorr+2.*Eb2_nocorr*mb);
		 	Double_t Q2_nocorr = mA+ma-mb- sqrt(mA*mA+mb*mb-ma*ma-2.*(mA+EBeam)*(mb+Eb2_nocorr)+2.*PA*Pb2_nocorr*cos(thetaR)+2.*(EBeam+mA+ma-Eb2_nocorr-mb)*ma);// without dedx corr
		  	IrisEvent->fECsI2 = ECsI2;
		  	IrisEvent->fQv2 = Q2;
		  	IrisEvent->fQv2_nocorr = Q2_nocorr;
			thetaCM2 = TMath::RadToDeg()*atan(Pb2y/Pb2xcm);
			thetaCM2 = (thetaCM2<0) ? thetaCM2+180. : thetaCM2;
			IrisEvent->fThetacm2 = thetaCM2;
		}
		tree->Fill();
	}
}

void HandleEOR_PHYSICS()
{
  	printf(" in Physics EOR\n");
	tree->AutoSave();
	treeFile->Close();
}
 