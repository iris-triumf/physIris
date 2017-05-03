// physIris
// Analysis software for the IRIS experiment @TRIUMF

#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <assert.h>
#include <signal.h>
#include <string>

#include <TTimer.h>
#include <TFile.h>
#include <TTree.h>
//#include "Globals.h"

#include "HandlePHYSICS.h"

// Global Variables
std::string gOutputFile = "";
std::string gCalibFile = "";
std::string gFileDir = "";

Int_t gEventCutoff;

TFile* treeFile = NULL;
TTree *tree = NULL;
TEvent *IrisEvent = NULL;

double GetTimeSec() {
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec + 0.000001*tv.tv_usec;
}

std::string GetBinDir()
{
	std::string path;
	char buff[256];
    ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff)-1);
	if (len != -1) {
			buff[len] = '\0';
		path = std::string(buff);
	}
	std::string BinDir = path.substr(0,path.rfind('/'));
	return BinDir;
}

int Process(std::string dirname)
{
	printf("---- BEGIN PROCESSING ---- \n");
	std::string gBinDir = GetBinDir();
	if(gBinDir.empty()){
		printf("Can't determine path of executable");
		return -1;
	}
	else{
		printf("Running %s.\n",gBinDir.data());
		printf("Writing data to %s.\n",gOutputFile.data());
		HandleBOR_PHYSICS(gBinDir, gFileDir, gCalibFile, gOutputFile); 
		printf("Running %s.\n",gBinDir.data());
		HandlePHYSICS();
		HandleEOR_PHYSICS();
  		printf("End.\n");
  		return 0;
	}
}

void help()
{
	printf("\nUsage:\n");
	printf("\n./bin/physIris [-h] [-c][-o] [-eMaxEvents] [file1 file2 ...]\n");
	printf("\n");
	printf("\t-h: print this help message\n");
	printf("\t-o/-o=/--output=: Path of output file.\n");
	printf("\t-c/-c=/--config=: Path of main configuration file.\n");
	printf("\t-e: Number of events to read from input data files\n");
	printf("\n");
	exit(1);
}

// Main function call

int main(int argc, char *argv[])
{
	setbuf(stdout,NULL);
	setbuf(stderr,NULL);
	
	signal(SIGILL,  SIG_DFL);
	signal(SIGBUS,  SIG_DFL);
	signal(SIGSEGV, SIG_DFL);
	
	std::vector<std::string> args;
	for (int i=0; i<argc; i++)
	{
		if (strcmp(argv[i],"-h")==0) help(); // does not return
	    args.push_back(argv[i]);
	}
	
	bool have_output= false;
	bool have_calib= false;

   	for (unsigned int i=1; i<args.size(); i++) // loop over the commandline options
 	{
       	const char* arg = args[i].c_str();
	   
        if (strncmp(arg,"-e",2)==0)  // Event cutoff flag (only applicable in offline mode)
	 		gEventCutoff = atoi(arg+2);
       	else if (strncmp(arg,"-o=",3)==0){  // Output file 
	 		gOutputFile = arg+3;
			have_output=true;
		}
       	else if (strncmp(arg,"-o",2)==0){  // Output file 
	 		gOutputFile = arg+2;
			have_output=true;
		}
      	else if (strncmp(arg,"--output=",9)==0){  // Output file 
	 		gOutputFile = arg+9;
			have_output=true;
		}
		else if (strncmp(arg,"-c=",3)==0){  // Calibration file 
	 		gCalibFile = arg+3;
			have_calib=true;
		}
       	else if (strncmp(arg,"-c",2)==0){  // Calibration file 
	 		gCalibFile = arg+2;
			have_calib=true;
		}
      	else if (strncmp(arg,"--config=",9)==0){  // Calibration file 
	 		gCalibFile = arg+9;
			have_calib=true;
		}
       	else if (strcmp(arg,"-h")==0)
	 		help(); // does not return
       	else if (arg[0] == '-')
	 		help(); // does not return
	}
	
   	for (unsigned int i=1; i<args.size(); i++) 
 	{
       	const char* arg = args[i].c_str();
    	if (arg[0] != '-')  
		{ 
		   gFileDir = arg;	
			if(have_output==true){	   
				Process(gFileDir);
			}
			else{
				printf("No output file specified!\n");
				printf("Use -o=/path/to/your/output-file!\n");
			}
    	}
    }
   	return 0;
}

//end
