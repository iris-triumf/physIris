#include "Graphsdedx.h"
#include <stdio.h>
#include <stdlib.h>
#include "TString.h"

Graphsdedx::Graphsdedx(){
  //Graphsdedx::Class()->IgnoreTObjectStreamer;
  Graphsdedx::Clear();
}

void Graphsdedx::ReadGraphnames(char* line)
{
	char *from=line;
	char *to=line;
	while (*from) {
		if (*from>32) {*to=*from;to++;}
		from++;
	}
	*to=0;
	if (*line==0) return; // line is empty
	
	char* val=strchr(line,'=');
	if (!val) printf("Missing = in input file, line: '%s'\n",line);
	*val=0;
	
	// trim param name
	char* trm=val-1;
	while (*trm<=32) *(trm--)=0;
	
	val++;
	if (*val==0) printf("Value missing for parameter %s",line);

	char cval[256];	
	std::string strval;
	sscanf(val,"%s",cval);
	strval=cval;
	
	//	parameter of type string:
	if (strcmp(line,"Ag")==0){
		Ag = strval;
		boolAg = kTRUE;
	}
	if (strcmp(line,"Al")==0){
		Al = strval;
		boolAl = kTRUE;
	}
	if (strcmp(line,"B")==0){
		B = strval;
		boolB = kTRUE;
	}
	if (strcmp(line,"TGT")==0){
		Tgt = strval;
		boolTgt = kTRUE;
	}
	if (strcmp(line,"My")==0){
		My = strval;
		boolMy = kTRUE;
	}
	if (strcmp(line,"P")==0){
		P = strval;
		boolP = kTRUE;
	} 
	if (strcmp(line,"Si")==0){
		Si = strval;
		boolSi = kTRUE;
	}
	if (strcmp(line,"SiO2")==0){
		SiO2 = strval;
		boolSiO2 = kTRUE;
	}
	if (strcmp(line,"Iso")==0){
		Iso = strval;
		boolIso = kTRUE;
	}
	if (strcmp(line,"Si3N4")==0||strcmp(line,"Window")==0){
		Wndw = strval;
		boolWndw = kTRUE;
	}
}

void Graphsdedx::Load(TString filename){
	boolAg=kFALSE;
	boolAl=kFALSE;
	boolB=kFALSE;
	boolTgt=kFALSE;
	boolMy=kFALSE;
	boolP=kFALSE;
	boolSi=kFALSE;
	boolSiO2=kFALSE;
	boolIso=kFALSE;
	boolWndw=kFALSE;

	char line[256];
	FILE* file=fopen(filename.Data(),"rb");
	if (file==NULL)
	{
		printf("dedx: Cannot open config file '%s' for reading.\n",filename.Data());
		//exit(0);
	}
	else{
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
				default  :  ReadGraphnames(ptr);
			}
		}
		fclose(file);
		file=NULL;
	}
}

void Graphsdedx::Print(){
	if(boolAg==kTRUE)	printf("Ag stopping power file: %s\n",Ag.data());
	else	printf("Ag stopping power file not specified.\n");
	if(boolAl==kTRUE)	printf("Al stopping power file: %s\n",Al.data());
	else	printf("Al stopping power file not specified.\n");
	if(boolB==kTRUE)	printf("B stopping power file: %s\n",B.data());
	else	printf("B stopping power file not specified.\n");
	if(boolTgt==kTRUE)	printf("Target stopping power file: %s\n",Tgt.data());
	else	printf("Target stopping power file not specified.\n");
	if(boolMy==kTRUE)	printf("My stopping power file: %s\n",My.data());
	else	printf("My stopping power file not specified.\n");
	if(boolP==kTRUE)	printf("P stopping power file: %s\n",P.data());
	else	printf("P stopping power file not specified.\n");
	if(boolSi==kTRUE)	printf("Si stopping power file: %s\n",Si.data());
	else	printf("Si stopping power file not specified.\n");
	if(boolSiO2==kTRUE)	printf("SiO2 stopping power file: %s\n",SiO2.data());
	else	printf("SiO2 stopping power file not specified.\n");
	if(boolIso==kTRUE)	printf("Isobutane stopping power file: %s\n",Iso.data());
	else	printf("Isobutane stopping power file not specified.\n");
	if(boolWndw==kTRUE)	printf("IC window (Si3N4) stopping power file: %s\n",Wndw.data());
	else	printf("IC window (Si3N4) stopping power file not specified.\n");
}

void Graphsdedx::Clear(){
}
