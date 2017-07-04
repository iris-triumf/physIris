#include "CalibPHYSICS.h"
#include <stdio.h>
#include <stdlib.h>

CalibPHYSICS::CalibPHYSICS(){
  	CalibPHYSICS::Clear();
}

void CalibPHYSICS::ReadFilenames(char* line)
{
	bool expect_val=true;
	char *from=line;
	char *to=line;
	while (*from) {
		if (*from>32) {*to=*from;to++;}
		from++;
	}
	*to=0;
	if (*line==0) return; // line is empty
	
	char* val=strchr(line,'=');
	if (!val){
		val=strchr(line, '!');
		expect_val=false;
	}
	if (!val) printf("Missing = or ! in input file, line: '%s'\n",line);
	*val=0;
	
	// trim param name
	char* trm=val-1;
	while (*trm<=32) *(trm--)=0;
	
	val++;
	if (*val==0 && expect_val) printf("Value missing for parameter %s",line);

	char cval[256];	
	TString strval;
	sscanf(val,"%s",cval);
	strval=cval;
	
	//	parameter of type string:
	if (strcmp(line,"PATH")==0){ 
		installPath = strval;
		fileGate = strval;
		fileELoss = strval;
		fileIdedx = strval;
		fileLdedx = strval;
		fileHdedx = strval;
		fileRunDepPar = strval;
		fileGeometry = strval;
		fileTCorrIC = strval;
	}
	if (strcmp(line,"RUNLIST")==0){
		fileRunList += strval;
		boolRunList = kTRUE;
	}
	if (strcmp(line,"GATEFILE")==0){
		fileGate += strval;
		boolFGate = kTRUE;
	}
	if (strcmp(line,"GATENAME")==0){
		nameGate = strval;
		boolNGate = kTRUE;
	}
	if (strcmp(line,"SDGATEFILE")==0){
		fileSdGate += strval;
		boolFSdGate = kTRUE;
	}
	if (strcmp(line,"SDGATENAME")==0){
		nameSdGate = strval;
		boolNSdGate = kTRUE;
	}
	if (strcmp(line,"ELOSS")==0){
		fileELoss += strval;
		boolELoss = kTRUE;
	}
	if (strcmp(line,"DEDX_I")==0){
		fileIdedx += strval;
		boolIdedx = kTRUE;
	}
	if (strcmp(line,"DEDX_L")==0){
		fileLdedx += strval;
		boolLdedx = kTRUE;
	}
	if (strcmp(line,"DEDX_H")==0){
		fileHdedx += strval;
		boolHdedx = kTRUE;
	}
	if (strcmp(line,"RUNDEPPAR")==0){
		fileRunDepPar += strval;
		boolRunDepPar = kTRUE;
	}
	if (strcmp(line,"NO_IC")==0){
		boolIC = kFALSE;
	}
	if (strcmp(line,"USE_ICGATES")==0){
		boolICGates = kTRUE;
	}
	if (strcmp(line,"GEOMETRY")==0){
		fileGeometry += strval;
		boolGeometry = kTRUE;
	}
	if (strcmp(line,"TCORR_IC")==0){
		fileTCorrIC += strval;
		boolTCorrIC =kTRUE;
	}
}

void CalibPHYSICS::Load(TString filename){
	boolIC=kTRUE;
	boolICGates=kFALSE;
	boolFGate=kFALSE;
	boolELoss=kFALSE;
	boolIdedx=kFALSE;
	boolHdedx=kFALSE;
	boolLdedx=kFALSE;
	boolRunDepPar=kFALSE;
	boolGeometry=kFALSE;
	boolRunList=kFALSE;
	boolTCorrIC=kFALSE;
	boolEssential=kFALSE;

	char line[256];
	FILE* file=fopen(filename.Data(),"rb");
	if (!file)
	{
		printf("CalibPHYSICS: Cannot open config file '%s' for reading.\n",filename.Data());
		//exit(0);
	}
	else
	{
		printf("Reading config file '%s'\n",filename.Data());
		
		while (!feof(file))
		{
			if (!fgets(line,256,file)) break;
			printf("%s",line);
			// skip leading white spaces
			char* ptr=line;
			while ((*ptr>0) && (*ptr<32)) ptr++;
			//printf("%s\n",ptr[0]);
			switch (ptr[0])
			{
				case 0   :
				case '#' :
				case '/' :  continue;
				default  :  ReadFilenames(ptr);
			}
		}
		fclose(file);
		file=NULL;
	}
	boolEssential = (boolRunList&&boolGeometry&&boolRunDepPar&&boolIdedx&&boolLdedx&&boolHdedx);
}

void CalibPHYSICS::Print(){
	printf("\n********************************\n");
	printf("** PHYSICS calibration files: **\n");
	if(boolRunList)	printf("List of used runs: %s\n",fileRunList.data());
	else	printf("No list of runs specified.\n");
	if(boolGeometry)	printf("Experiment geometry: %s\n",fileGeometry.data());
	else	printf("No experiment geometry specified.\n");
	if(boolELoss)	printf("Energy loss root file: %s\n",fileELoss.data());
	else	printf("No energy loss root file specified.\n");
	if(boolIdedx)	printf("Incomming ion energy loss tables: %s\n",fileIdedx.data());
	else	printf("No incoming ion energy loss tables specified.\n");
	if(boolLdedx)	printf("Target-like particle energy loss tables: %s\n",fileLdedx.data());
	else	printf("No target-like energy loss tables specified.\n");
	if(boolHdedx)	printf("Beam-like particle energy loss tables: %s\n",fileHdedx.data());
	else	printf("No beam-like particle energy loss tables specified.\n");
	if(boolFGate)	printf("YY1/CsI gate in %s\n",fileGate.data());
	else	printf("No YY1/CsI gate file specified.\n");
	if(boolNGate)	printf("YY1/CsI gate name: %s\n",nameGate.data());
	else	printf("No YY1/CsI gate name specified.\n");
	if(boolFSdGate)	printf("S3 gate in %s\n",fileSdGate.data());
	else	printf("No S3 gate file specified.\n");
	if(boolNSdGate)	printf("S3 gate name: %s\n",nameSdGate.data());
	else	printf("No S3 gate name specified.\n");
	if(boolRunDepPar)	printf("Run dependant parameters: %s\n",fileRunDepPar.data());
	else	printf("No run dependant parameters specified.\n");
	if(boolIC)	printf("Isobutane in ionization chamber.\n");
	else	printf("No isobutane in ionization chamber.\n");
	if(boolICGates)	printf("Using ionization chamber for incoming ID.\n");
	else	printf("No incoming ID using the ionization chamber.\n");
	printf("********************************\n\n");
}

void CalibPHYSICS::Clear(){
}
