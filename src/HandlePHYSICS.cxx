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

#include "HandlePHYSICS.h"
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

float ICELossCorr=1.; // has to be implemented!

const int Nchannels = 24;
const int binlimit = 1900;
 		
const Double_t ICLength=22.9*0.062; //cm*mg/cm^3 at 19.5 Torr
const Double_t ICWindow1=0.03*3.44*0.1; //mu*g/cm^3*0.1
const Double_t ICWindow2=0.05*3.44*0.1; //mu*g/cm^3*0.1


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
Double_t PbU; //Light ejectile momentum

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
Double_t thetaCM=sqrt(-1.);
Double_t ECsI1=sqrt(-1.);
Double_t ECsI2=sqrt(-1.);
Double_t EYY1=sqrt(-1.);
Double_t Eb1=sqrt(-1.), Eb2=sqrt(-1.), EbU=sqrt(-1.);
Double_t EB1=sqrt(-1.), EB2=sqrt(-1.), EBU=sqrt(-1.);
Double_t PB1=sqrt(-1.), PB2=sqrt(-1.), PBU=sqrt(-1.);
Double_t Q1=sqrt(-1.), Q2=sqrt(-1.), QU=sqrt(-1.);
Double_t Pb1y=sqrt(-1.), Pb2y=sqrt(-1.), PbUy=sqrt(-1.);
Double_t Pb1xcm=sqrt(-1.), Pb2xcm=sqrt(-1.), PbUxcm=sqrt(-1.);

Int_t CCsI1=-1;
Int_t CCsI2=-1;
	
TCutG *YdCsIGate = NULL;
TCutG *SdGate = NULL;
TCutG *YuGate = NULL;
TCutG *SuGate = NULL;

IDet detec; // calibrated variables from detectors, to be passed to HandlePhysics
IDet *det = &detec;
ITdc timeArray;
ITdc *tdc = &timeArray;
IScaler scal;
IScaler *pscaler = &scal;
PTrack lP;
PTrack *plP = &lP;
PTrack hP;
PTrack *phP = &hP;

UInt_t Run, Event;
UInt_t prevRun =0;

TChain* createChain(std::vector<Int_t> runs, std::string Directory)
{
    printf("***Creating New TChain***\n");
  	
	TChain *ch = new TChain("Iris");
	Int_t nRuns = runs.size();
	printf("Adding %d runs!\n",nRuns);
  	for(int i=0; i< nRuns; ++i) {
		std::string filename;
		std::string runNo;
		if(runs.at(i)>99999) runNo = Form("%d",runs.at(i));
		else runNo = Form("0%04d",runs.at(i));
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
  	printf("----------\n\n");
  	return ch;
}

void getRunPar(Int_t runNo)
{
	FILE *pFile;
	int i = 0;
	double a,b;
	int run_for_corr = 0;
	
	pFile = fopen(runDepPar.runPar.data(), "r");	
	if (pFile == NULL) {
		printf("No run dependent energy and target thickness. Skipping correction.\n");
	}
	else  {
		printf("Reading config file '%s'\n",runDepPar.runPar.data());
	
		while (!feof(pFile)){
			fscanf(pFile,"%d\t%lf\t%lf\n",&i,&a,&b);
			//printf("%d\t%lf\t%lf\n",i,a,b);
			if(i==runNo){ 
				run_for_corr = i;
				runDepPar.energy = a;
			   	geoP.TargetThickness = b;	
			}
		}
		fclose (pFile);	
	
		if(run_for_corr==0){ 
			printf("Run %d not in list. No correction applied!\n",runNo);
		}
		else{
			printf("Run: %d\tBeam energy: %f\tTarget thickness: %f\n",run_for_corr,runDepPar.energy,geoP.TargetThickness);
		}
	}
}

void calculateBeamEnergy(Double_t E)
{
	runDepPar.EBAC = E;
	printf("New Beam Energy: %f\n" ,E);
	Double_t temp_E = E;
	if(calPhys.boolIC==kTRUE){
		E = E-eloss(E,ICWindow1,eAWndw,dedxAWndw);  
		E = E-eloss(E,ICLength,eAIso,dedxAIso)/ICELossCorr;  
		E = E-eloss(E,ICWindow2,eAWndw,dedxAWndw);  
		printf("Energy loss in IC (including windows): %.3f MeV\n" ,temp_E-E);

		temp_E = E;
	}
	//if(geoP.FoilThickness>0.) E = runDepPar.energy-eloss(E,geoP.FoilThickness,eAAg,dedxAAg);  
	if(geoP.FoilThickness>0.) E = E-eloss(E,geoP.FoilThickness,eAAg,dedxAAg);  
	else E = temp_E;
	printf("Energy loss in silver foil: %.3f MeV\n" ,temp_E-E);
	printf("Energy after silver foil: %.3f MeV\n",E);

	temp_E = E;
	E = E-eloss(E,geoP.TargetThickness/2.,eATgt,dedxATgt);  
	printf("Energy loss in half target: %.3f MeV\n" ,temp_E-E);
	printf("Beam energy in center of target: %.3f MeV\n" ,E);

	runDepPar.energy = E;
	runDepPar.momentum = sqrt(runDepPar.energy*runDepPar.energy+2.*runDepPar.energy*beam.mass);//beam momentum
	runDepPar.beta = runDepPar.momentum/(runDepPar.energy + beam.mass + target.mass);
	runDepPar.gamma = 1./sqrt(1.-runDepPar.beta*runDepPar.beta);

	EBAC = runDepPar.EBAC;
	EBeam= runDepPar.energy;
	PA = runDepPar.momentum;//beam momentum
	betaCM = runDepPar.beta;
	gammaCM = runDepPar.gamma;
	printf("Beam momentum in center of target: %.3f MeV\n",PA);
	printf("Beta: %f\tGamma: %f\n",betaCM,gammaCM);
}

//---------------------------------------------------------------------------------
void HandleBOR_PHYSICS(std::string BinPath, std::string Directory, std::string CalibFile, std::string OutputFile)
{
  	printf("In HandleBOR_PHYSICS...\n");	
	if(CalibFile=="") printf("No calibration file specified!\n\n");
	calPhys.Load(CalibFile);
	calPhys.Print();

	Int_t runTmp;
	std::vector<Int_t> runs;
	std::ifstream rFile(calPhys.fileRunList.data());
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
	
	// deactivate some branches, only relevant for simulated data from simIris
	if(input_chain->GetListOfBranches()->FindObject("Evnt")) input_chain->SetBranchStatus("Evnt",0); 
	if(input_chain->GetListOfBranches()->FindObject("beamE")) input_chain->SetBranchStatus("beamE",0); 
	if(input_chain->GetListOfBranches()->FindObject("beamBeta")) input_chain->SetBranchStatus("beamBeta",0); 
	if(input_chain->GetListOfBranches()->FindObject("beamGamma")) input_chain->SetBranchStatus("beamGamma",0); 
	if(input_chain->GetListOfBranches()->FindObject("beamEcm")) input_chain->SetBranchStatus("beamEcm",0); 
	if(input_chain->GetListOfBranches()->FindObject("reacX")) input_chain->SetBranchStatus("reacX",0); 
	if(input_chain->GetListOfBranches()->FindObject("reacY")) input_chain->SetBranchStatus("reacY",0); 
	if(input_chain->GetListOfBranches()->FindObject("reacZ")) input_chain->SetBranchStatus("reacZ",0); 
	if(input_chain->GetListOfBranches()->FindObject("hPdec")) input_chain->SetBranchStatus("hPdec*",0); 
	if(input_chain->GetListOfBranches()->FindObject("lPdec1")) input_chain->SetBranchStatus("lPdec1*",0); 
	if(input_chain->GetListOfBranches()->FindObject("lPdec2")) input_chain->SetBranchStatus("lPdec2*",0); 
	if(input_chain->GetListOfBranches()->FindObject("wght")) input_chain->SetBranchStatus("wght",0); 
	if(input_chain->GetListOfBranches()->FindObject("Qgen")) input_chain->SetBranchStatus("Qgen",0); 
	if(input_chain->GetListOfBranches()->FindObject("qdet")) input_chain->SetBranchStatus("Qdet",0); 
	if(input_chain->GetListOfBranches()->FindObject("ICdE")) input_chain->SetBranchStatus("ICdE",0);
	if(input_chain->GetListOfBranches()->FindObject("SSBdE")) input_chain->SetBranchStatus("SSBdE",0);
	if(input_chain->GetListOfBranches()->FindObject("yd")) input_chain->SetBranchStatus("yd*",0);
	if(input_chain->GetListOfBranches()->FindObject("csi")) input_chain->SetBranchStatus("csi*",0);
	if(input_chain->GetListOfBranches()->FindObject("sd1")) input_chain->SetBranchStatus("sd1*",0);
	if(input_chain->GetListOfBranches()->FindObject("sd2")) input_chain->SetBranchStatus("sd2*",0);
	//***************************************************************************
	
	if(input_chain->GetListOfBranches()->FindObject("det")){
	   	input_chain->SetBranchAddress("det",&det);
	}
	if(input_chain->GetListOfBranches()->FindObject("tdc")){ 
		input_chain->SetBranchAddress("tdc",&tdc);
	}
	if(input_chain->GetListOfBranches()->FindObject("scaler")){
	   	input_chain->SetBranchAddress("scaler",&pscaler);
	}
	if(input_chain->GetListOfBranches()->FindObject("lP")){
	   	input_chain->SetBranchAddress("lP",&plP);
	}
	if(input_chain->GetListOfBranches()->FindObject("hP")){
	   	input_chain->SetBranchAddress("hP",&phP);
	}		
	if(input_chain->GetListOfBranches()->FindObject("Run")){
	   	input_chain->SetBranchAddress("Run",&Run);
	}
	if(input_chain->GetListOfBranches()->FindObject("Event")){
	   	input_chain->SetBranchAddress("Event",&Event);
	}
	treeFile = new TFile(OutputFile.data(),"RECREATE");
	tree=input_chain->CloneTree(0);	
	IrisEvent = new TEvent();
 	tree->Branch("IrisEvent","TEvent",&IrisEvent,32000,99);

	if(calPhys.boolFGate==kTRUE){
		TFile *fYdCsIGates = new TFile(calPhys.fileGate.data());
   		printf("opened file %s\n",calPhys.fileGate.data());
   
		if(calPhys.boolNGate==kTRUE){
			YdCsIGate = (TCutG*)fYdCsIGates->FindObjectAny(calPhys.nameGate.data());
  			if(!YdCsIGate) printf("No Yd/CsI gate.\n");  
			else printf("Grabbed Yd/CsI gate %s.\n",calPhys.nameGate.data());
		}
		else printf("No Yd/CsI gate.\n"); 
	}
	else printf("No Yd/CsI gate.\n");  
	
	if(calPhys.boolFSdGate==kTRUE){
		TFile *fSdGates = new TFile(calPhys.fileSdGate.data());
   		printf("opened file %s\n",calPhys.fileSdGate.data());
   
		if(calPhys.boolNSdGate==kTRUE){
			SdGate = (TCutG*)fSdGates->FindObjectAny(calPhys.nameSdGate.data());
  			if(!SdGate) printf("No S3 gate.\n");  
			else printf("Grabbed S3 gate %s.\n",calPhys.nameSdGate.data());
		}
		else printf("No S3 gate.\n"); 
	}
	else printf("No S3 gate.\n");  

	if(calPhys.boolFYuGate==kTRUE){
		TFile *fYuGates = new TFile(calPhys.fileYuGate.data());
   		printf("opened file %s\n",calPhys.fileYuGate.data());
   
		if(calPhys.boolNYuGate==kTRUE){
			YuGate = (TCutG*)fYuGates->FindObjectAny(calPhys.nameYuGate.data());
  			if(!YuGate) printf("No Yu gate.\n");  
			else printf("Grabbed Yu gate %s.\n",calPhys.nameGate.data());
		}
		else printf("No Yu gate.\n"); 
	}
	else printf("No Yu gate.\n");  
	
	if(calPhys.boolFSuGate==kTRUE){
		TFile *fSuGates = new TFile(calPhys.fileSuGate.data());
   		printf("opened file %s\n",calPhys.fileSuGate.data());
   
		if(calPhys.boolNSuGate==kTRUE){
			SuGate = (TCutG*)fSuGates->FindObjectAny(calPhys.nameSuGate.data());
  			if(!SuGate) printf("No upstream S3 gate.\n");  
			else printf("Grabbed upstream S3 gate %s.\n",calPhys.nameSuGate.data());
		}
		else printf("No upstream S3 gate.\n"); 
	}
	else printf("No upstream S3 gate.\n");  
	
	geoP.ReadGeometry(calPhys.fileGeometry.data());
	geoP.Print();

//	ICELossCorr = 1.;	


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

		beam.getInfo(BinPath,runDepPar.nA);
		target.getInfo(BinPath,runDepPar.na);
		hej.getInfo(BinPath,runDepPar.nB);
		lej.getInfo(BinPath,runDepPar.nb);

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
			if(dedx_h.boolTgt==kTRUE) loadELoss(dedx_h.Tgt,eBTgt,dedxBTgt,mB);	
			if(dedx_h.boolSi==kTRUE) loadELoss(dedx_h.Si,eBSi,dedxBSi,mB);	
			if(dedx_h.boolAl==kTRUE) loadELoss(dedx_h.Al,eBAl,dedxBAl,mB);	
			if(dedx_h.boolB==kTRUE) loadELoss(dedx_h.B, eBB,dedxBB,mB);	
			if(dedx_h.boolP==kTRUE) loadELoss(dedx_h.P, eBP,dedxBP,mB);	
			if(dedx_h.boolSiO2==kTRUE) loadELoss(dedx_h.SiO2,eBSiO2,dedxBSiO2,mB);	
			if(dedx_h.boolIso==kTRUE) loadELoss(dedx_h.Iso,eAIso,dedxAIso,mB);	
			if(dedx_h.boolWndw==kTRUE) loadELoss(dedx_h.Wndw,eAWndw,dedxAWndw,mB);	
		}

		// Initialize runPar with values from first run in chain	
		if(runDepPar.bool_runPar==kTRUE) getRunPar(runs.at(0));
		prevRun=runs.at(0);
		calculateBeamEnergy(runDepPar.energy);
		
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
		if (det->TICEnergy.at(0)<runDepPar.ICmin || det->TICEnergy.at(0)>runDepPar.ICmax) continue; // event in IC YdCsIGate?
		if (YdCsIGate!=NULL&&(det->TYdEnergy.size()==0||det->TCsI1Energy.size()==0||det->TCsI2Energy.size()==0)) continue; // event has YY1 and CsI hit?
		//if (YdCsIGate!=NULL&&det->TCsI1Channel[0]-det->TCsI2Channel.at(0)!=0) continue; // CsI1 and CsI2 channels the same?
		if (YdCsIGate!=NULL&&calPhys.numGate!=2&&int(det->TCsI1Channel[0]/2)-det->TYdNo.at(0)!=0) continue; // CsI hit behind Yd hit?
		if (YdCsIGate!=NULL&&calPhys.numGate!=2&&YdCsIGate->IsInside(det->TCsI1Energy.at(0),det->TYdEnergy.at(0)*cos(TMath::DegToRad()*det->TYdTheta.at(0)))==0) continue; // event in proton/deuteron/etc YdCsIGate?
		if (YdCsIGate!=NULL&&calPhys.numGate==2&&int(det->TCsI2Channel[0]/2)-det->TYdNo.at(0)!=0) continue; // CsI hit behind Yd hit?
		if (YdCsIGate!=NULL&&calPhys.numGate==2&&YdCsIGate->IsInside(det->TCsI2Energy.at(0),det->TYdEnergy.at(0)*cos(TMath::DegToRad()*det->TYdTheta.at(0)))==0) continue; // event in proton/deuteron/etc YdCsIGate?
		if (SdGate!=NULL&&(det->TSd1rEnergy.size()==0||det->TSd1sEnergy.size()==0||det->TSd2rEnergy.size()==0||det->TSd2sEnergy.size()==0)) continue; // event has S3 hit?
		if (SdGate!=NULL&&SdGate->IsInside(det->TSd2sEnergy.at(0),det->TSd1rEnergy.at(0)*cos(TMath::DegToRad()*det->TSd1Theta.at(0)))==0) continue; // event in proton/deuteron/etc SdGate?
		if (YuGate!=NULL&&det->TYuEnergy.size()==0) continue; // event has Yu hit?
		if (YuGate!=NULL&&YuGate->IsInside(det->TYuTheta.at(0),det->TYuEnergy.at(0))==0) continue; // event in proton/deuteron/etc YuGate?
		if (SuGate!=NULL&&(det->TSurEnergy.size()==0||det->TSusEnergy.size()==0)) continue; // event has Su hit?
		if (SuGate!=NULL&&SuGate->IsInside(det->TSuTheta.at(0),det->TSurEnergy.at(0))==0) continue; // event in proton/deuteron/etc SuGate?
		
		if(runDepPar.bool_runPar == kTRUE && Run != prevRun){
		  	getRunPar(Run);
		  	calculateBeamEnergy(runDepPar.energy);
		  	prevRun = Run;
		}
		
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
		if(det->TSd1rEnergy.size()>0 && det->TSd2sEnergy.size()>0){
			if(det->TSd1rEnergy.at(0)>0. && det->TSd2sEnergy.at(0)>0.){
	  			cosTheta = cos(TMath::DegToRad()* (det->TSd1Theta.at(0)));
	  			//Sd2 ring side
	  			energy = det->TSd2sEnergy.at(0);
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
			B = 2.0*PResid* cos(TMath::DegToRad()*det->TSd1Theta.at(0));
			C = -1.*(kBF+1)*PResid*PResid; 
			if (A!=0)    PBeam = (sqrt(B*B-4.*A*C)-B)/(2*A);
			IrisEvent->fPBeam = PBeam;
			IrisEvent->fPResid = PResid;
			IrisEvent->fA = A;
			IrisEvent->fB = B;
			IrisEvent->fC = C;
			//to calculate residue energy from beam
			
			//IrisEvent->fEB = PBeam*PBeam/(2.*mA);
			
			IrisEvent->fEB=  IrisEvent->fEB + elossFi(det->TSdETot,geoP.FoilThickness/2.,eAAg,dedxAAg); //energy loss from the end of H2 to the center of Ag.
			det->TSdThetaCM = TMath::RadToDeg()*atan(tan(TMath::DegToRad()*det->TSd1Theta.at(0))/sqrt(gammaCM-gammaCM*betaCM*(mA+IrisEvent->fEB)/(PBeam*cos(TMath::DegToRad()*det->TSd1Theta.at(0)))));// check if this is still correct for H2 target tk
      	}
       
      	if (det->TYdEnergy.size()>0&&det->TYdRing.size()>0) {    //check if in the proton/deuteron YdCsIGate
			thetaR = atan((geoP.YdInnerRadius+((det->TYdRing.at(0)+0.5)*(geoP.YdOuterRadius-geoP.YdInnerRadius)/16))/geoP.YdDistance);
			thetaD = thetaR*TMath::RadToDeg();
			IrisEvent->fThetaD = thetaD;
			EYY1 = det->TYdEnergy.at(0);
		}	
     
		if (det->TCsI1Energy.size()>0&&det->TYdEnergy.size()>0&&det->TYdRing.size()>0) {    //check if in the proton/deuteron YdCsIGate
		
			CCsI1= det->TCsI1Channel.at(0);
			ECsI1= det->TCsI1Energy.at(0);
			
			ECsI1= ECsI1+elossFi(ECsI1,0.1*1.4*6./cos(thetaR),ebMy,dedxbMy); //Mylar                                                                                  
			ECsI1= ECsI1+elossFi(ECsI1,0.1*2.702*0.3/cos(thetaR),ebAl,dedxbAl); //0.3 u Al                                                                            
			ECsI1= ECsI1+elossFi(ECsI1,0.1*1.822*0.1/cos(thetaR),ebP,dedxbP); // 0.1Phosphorus                                                                      
			
			Eb1= ECsI1+EYY1; //use measured Yd // change june28
			
			Eb1= Eb1+elossFi(Eb1,0.1*2.35*0.05/cos(thetaR),ebB,dedxbB); //0.05 u B 
			Eb1= Eb1+elossFi(Eb1,0.1*2.702*0.1/cos(thetaR),ebAl,dedxbAl); //0.1 u Al
			IrisEvent->fEYY1 = Eb1-ECsI1;
			Eb1= Eb1+elossFi(Eb1,geoP.TargetThickness/2./cos(thetaR),ebTgt,dedxbTgt); //deuteron energy midtarget
			
			det->TYdCsI1ETot = Eb1;
			Pb1 = sqrt(Eb1*Eb1+2.*Eb1*mb);
			Pb1y = Pb1*sin(thetaR);
			Pb1xcm = gammaCM*betaCM*(Eb1+mb)- gammaCM*Pb1*cos(thetaR);
			EB1 = EBeam+mA+ma-Eb1-mb;
			PB1 = sqrt(PA*PA+Pb1*Pb1-2.*PA*Pb1*cos(thetaR));
			//Q1 = mA+ma-mb- sqrt(mA*mA+mb*mb-ma*ma-2.*(mA+EBeam)*(mb+Eb1)+2.*PA*Pb1*cos(thetaR)+2.*(EBeam+mA+ma-Eb1-mb)*ma);  //Alisher's equation 
			Q1 = mA+ma-mb-sqrt(EB1*EB1-PB1*PB1); //Equivalent to the previous equation
			IrisEvent->fCCsI1 = CCsI1;
			IrisEvent->fECsI1 = ECsI1;
			IrisEvent->fEb1 = Eb1;
			IrisEvent->fPb1 = Pb1;
			IrisEvent->fEB1 = EB1;
			IrisEvent->fPB1 = PB1;
			IrisEvent->fPb1y = Pb1y;
			IrisEvent->fPb1xcm = Pb1xcm;
			IrisEvent->fQv1 = Q1;
			thetaCM = TMath::RadToDeg()*atan(Pb1y/Pb1xcm);
			thetaCM = (thetaCM<0) ? thetaCM+180. : thetaCM;
			IrisEvent->fThetacm1 = thetaCM;
		}
      
		if (det->TCsI2Energy.size()>0&&det->TYdEnergy.size()>0&&det->TYdRing.size()>0) {    //check if in the proton/deuteron gate

			CCsI2= det->TCsI2Channel.at(0);	
			ECsI2= det->TCsI2Energy.at(0);
			
			ECsI2= ECsI2+elossFi(ECsI2,0.1*1.4*6./cos(thetaR),ebMy,dedxbMy); //Mylar                                                                                  
			ECsI2= ECsI2+elossFi(ECsI2,0.1*2.702*0.3/cos(thetaR),ebAl,dedxbAl); //0.3 u Al                                                                            
			ECsI2= ECsI2+elossFi(ECsI2,0.1*1.822*0.1/cos(thetaR),ebP,dedxbP); // 0.1Phosphorus                                                                      
			
			Eb2= ECsI2+EYY1; //use measured Yd // change june28
			
			Eb2= Eb2+elossFi(Eb2,0.1*2.35*0.05/cos(thetaR),ebB,dedxbB); //0.05 u B 
			Eb2= Eb2+elossFi(Eb2,0.1*2.702*0.1/cos(thetaR),ebAl,dedxbAl); //0.1 u Al
			Eb2= Eb2+elossFi(Eb2,geoP.TargetThickness/2./cos(thetaR),ebTgt,dedxbTgt); //deuteron energy midtarget
			
			det->TYdCsI2ETot = Eb2;
			Pb2 = sqrt(Eb2*Eb2+2.*Eb2*mb);
			Pb2y = Pb2*sin(thetaR);
			Pb2xcm = gammaCM*betaCM*(Eb2+mb)- gammaCM*Pb2*cos(thetaR);
			EB2 = EBeam+mA+ma-Eb2-mb;
			PB2 = sqrt(PA*PA+Pb2*Pb2-2.*PA*Pb2*cos(thetaR));
			//Q2 = mA+ma-mb- sqrt(mA*mA+mb*mb-ma*ma-2.*(mA+EBeam)*(mb+Eb2)+2.*PA*Pb2*cos(thetaR)+2.*(EBeam+mA+ma-Eb2-mb)*ma);  //Alisher's equation 
			Q2 = mA+ma-mb-sqrt(EB2*EB2-PB2*PB2); //Equivalent to the previous equation
			IrisEvent->fCCsI2 = CCsI2;
			IrisEvent->fECsI2 = ECsI2;
			IrisEvent->fEb2 = Eb2;
			IrisEvent->fPb2 = Pb2;
			IrisEvent->fEB2 = EB2;
			IrisEvent->fPB2 = PB2;
			IrisEvent->fPb2y = Pb2y;
			IrisEvent->fPb2xcm = Pb2xcm;
			IrisEvent->fQv2 = Q2;
			thetaCM = TMath::RadToDeg()*atan(Pb2y/Pb2xcm);
			thetaCM = (thetaCM<0) ? thetaCM+180. : thetaCM;
			IrisEvent->fThetacm2 = thetaCM;
		}
  	
		// Upstream
		// Upstream
		// Upstream
		
		if (det->TYuEnergy.size()>0&&det->TYuRing.size()>0) {    //check if in the proton/deuteron YuGate
			thetaR = atan(TMath::Pi()+(geoP.YdInnerRadius+((det->TYuRing.at(0)+0.5)*(geoP.YdOuterRadius-geoP.YdInnerRadius)/16))/geoP.YuDistance);
			thetaD = thetaR*TMath::RadToDeg();
			IrisEvent->fThetaDU = thetaD;

			EbU=  det->TYuEnergy.at(0); //use measured Yd // change june28
			
			EbU= EbU+elossFi(EbU,0.1*2.35*0.05/cos(TMath::Pi()-thetaR),ebB,dedxbB); //0.05 u B 
			EbU= EbU+elossFi(EbU,0.1*2.70*0.1/cos(TMath::Pi()-thetaR),ebAl,dedxbAl); //0.1 u Al
			EbU= EbU+elossFi(EbU,geoP.TargetThickness/2./cos(TMath::Pi()-thetaR),ebTgt,dedxbTgt); //deuteron energy midtarget
			
			PbU = sqrt(EbU*EbU+2.*EbU*mb);
			PbUy = PbU*sin(thetaR);
			PbUxcm = gammaCM*betaCM*(EbU+mb)- gammaCM*PbU*cos(thetaR);
			EBU = EBeam+mA+ma-EbU-mb;
			PBU = sqrt(PA*PA+PbU*PbU-2.*PA*PbU*cos(thetaR));
			QU = mA+ma-mb-sqrt(EBU*EBU-PBU*PBU);
			IrisEvent->fEbU = EbU;
			IrisEvent->fPbU = PbU;
			IrisEvent->fEBU = EBU;
			IrisEvent->fPBU = PBU;
			IrisEvent->fPbUy = PbUy;
			IrisEvent->fPbUxcm = PbUxcm;
			IrisEvent->fQvU = QU;
			thetaCM = TMath::RadToDeg()*atan(PbUy/PbUxcm);
			thetaCM = (thetaCM<0) ? thetaCM+180. : thetaCM;
			IrisEvent->fThetacmU = thetaCM;
		}
		tree->Fill();
	} // end event loop
}

void HandleEOR_PHYSICS()
{
  	printf(" in Physics EOR\n");
	tree->AutoSave();
	treeFile->Close();
}
 
