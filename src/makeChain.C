#include "TChain.h"

TChain* makeChain(Int_t *runs, Int_t nRuns, TString root_dir, TString treeName, TString chainName = "")
{
  	if(string(chainName) == "") {
    	cout << "... Creating New TChain (Tree Name = \"" << treeName << "\") ...\n";
  	}
  	else {
    	cout << "... Creating New TChain (Tree Name = \"" << treeName << ", Chain Name = " << chainName << "\") ...\n";
  	}
  	TString cmd;
  	cmd = "new TChain(\"" + treeName + "\")";
  	//TChain *ch = reinterpret_cast<TChain*>(gROOT->ProcessLineFast(cmd.str().c_str()));
  	TChain *ch = (TChain*)(gROOT->ProcessLineFast(cmd));
	cout << "Adding " << nRuns << " runs!\n";
  	for(int i=0; i< nRuns; ++i) {
    	TString filename;
		TString runNo;
		runNo.Form("%d",runs[i]);
    	filename = root_dir + "/tree0" + runNo + ".root";
    	int addReturn = ch->AddFile(filename);
    	TFile testFile(filename);
    	if(addReturn == 1 && !testFile.IsZombie()) {
      	cout << "Adding run: " << filename.Data() << "\n";
    	} 
		else {
      		cout << "WARNING: Tried to add an invalid file: " << filename.Data()  << " ... Skipping.\n";
    	}
  	}
  	cout << "----------\n\n";
  	return ch;
}
