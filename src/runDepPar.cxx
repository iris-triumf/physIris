//Run dependent parameters in BOR AS
#include "runDepPar.h"

runDep::runDep(){
}

void runDep::setRunDepPar(std::string filename)
{
	ICmin=0;
	ICmax=0;
	
	char buffer[256];
	FILE * parFile;
	parFile=fopen(filename.data(),"r");

	if (parFile == NULL) {
		perror ("Error opening file");
   	}	
	printf("Reading config file '%s'\n",filename.data());
	
	while (!feof(parFile))
	{
		if (!fgets(buffer,256,parFile)) break;
		printf("%s",buffer);
		
		char* val=strchr(buffer,'=');
		if (!val) printf("Missing = in input parFile, line: '%s'\n",buffer);
		*val=0;	
		
		// trim param name
		char* trm=val-1;
		while (*trm<=32) *(trm--)=0;
	
		val++;
		if (*val==0) printf("Value missing for parameter %s",buffer);
		
		// parse float parameter (if any)
		Double_t v;
		char cval[256];	
		std::string strval;
		sscanf(val,"%lf",&v);
		sscanf(val,"%s",cval);
		strval=cval;

		if (strcmp(buffer,"EBAC")==0){
			EBAC = v;
			energy = v;
		}
		if (strcmp(buffer,"ICMIN")==0)	ICmin = v;
		if (strcmp(buffer,"ICMAX")==0)	ICmax = v;
		if (strcmp(buffer,"A")==0)	  	nA = strval;
		if (strcmp(buffer,"a")==0)	  	na = strval;
		if (strcmp(buffer,"B")==0)	  	nB = strval;
		if (strcmp(buffer,"b")==0)	  	nb = strval;
	}
	fclose(parFile);
}

void runDep::Print()
{
	printf("\n*** Run dependant parameters:\n");
	printf("Reaction %s(%s,%s)%s\n",nA.data(),na.data(),nb.data(),nB.data());
	printf("Energy: %f (%f at center of target)\n",EBAC,energy);
	printf("IC energy gate: min=%f\tmax=%f\n\n",ICmin,ICmax);
}
