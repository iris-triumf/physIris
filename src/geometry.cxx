#include "geometry.h"

geometry::geometry(){
}

void geometry::ReadGeometry(std::string filename)
{
	for(int i=0;i<8;i++) YdThickness[i] = 0.;
	YdDistance = 0.;
	YdInnerRadius = 0.;
	YdOuterRadius = 0.;
	Sd1Thickness = 0.;
	Sd1Distance = 0.;
	Sd2Thickness = 0.;
	Sd2Distance = 0.;
	SdInnerRadius = 0.;
	SdOuterRadius = 0.;
	xShift = 0.;
	yShift = 0.;
	TargetThickness = 0.;
	FoilThickness = 0.;

	char buffer[256];
	FILE * parFile;
	parFile=fopen(filename.data(),"r");

	if (parFile == NULL) {
		perror ("Error opening file");
   	}
	else{	
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
			sscanf(val,"%lf",&v);

			if (strcmp(buffer,"YD1_THICKNESS")==0)	YdThickness[0] = v;
			if (strcmp(buffer,"YD2_THICKNESS")==0)	YdThickness[1] = v;
			if (strcmp(buffer,"YD3_THICKNESS")==0)	YdThickness[2] = v;
			if (strcmp(buffer,"YD4_THICKNESS")==0)	YdThickness[3] = v;
			if (strcmp(buffer,"YD5_THICKNESS")==0)	YdThickness[4] = v;
			if (strcmp(buffer,"YD6_THICKNESS")==0)	YdThickness[5] = v;
			if (strcmp(buffer,"YD7_THICKNESS")==0)	YdThickness[6] = v;
			if (strcmp(buffer,"YD8_THICKNESS")==0)	YdThickness[7] = v;
			if (strcmp(buffer,"YD_DISTANCE")==0)	YdDistance = v;
			if (strcmp(buffer,"YD_INNER_RADIUS")==0)	YdInnerRadius = v;
			if (strcmp(buffer,"YD_OUTER_RADIUS")==0)	YdOuterRadius = v;
			if (strcmp(buffer,"SD1_THICKNESS")==0)	Sd1Thickness = v;
			if (strcmp(buffer,"SD1_DISTANCE")==0)	Sd1Distance = v;
			if (strcmp(buffer,"SD2_THICKNESS")==0)	Sd2Thickness = v;
			if (strcmp(buffer,"SD2_DISTANCE")==0)	Sd2Distance = v;
			if (strcmp(buffer,"SD_INNER_RADIUS")==0)	SdInnerRadius = v;
			if (strcmp(buffer,"SD_OUTER_RADIUS")==0)	SdOuterRadius = v;
			if (strcmp(buffer,"X_SHIFT")==0)	xShift = v;
			if (strcmp(buffer,"Y_SHIFT")==0)	yShift = v;
			if (strcmp(buffer,"TARGET_THICKNESS")==0)	TargetThickness = v;
			if (strcmp(buffer,"FOIL_THICKNESS")==0)	FoilThickness = v;
		}
		fclose(parFile);
	}
}

void geometry::Print()
{
	printf("\n*** Experiment Geometry:\n");		
	printf("Target thickness: %.2f mg/cm^2\n",TargetThickness);
	printf("Ag foil thickness: %.2f mg/cm^2\n",FoilThickness);
	printf("Beam shift in x-direction: %.1f mm\n",xShift);
	printf("Beam shift in y-direction: %.1f mm\n",yShift);
	printf("Thicknesses YY1 Segments: %.1f mg/cm^2, %.1f mg/cm^2, %.1f mg/cm^2, %.1f mg/cm^2, %.1f mg/cm^2, %.1f mg/cm^2, %.1f mg/cm^2,%.1f mg/cm^2\n",YdThickness[0],YdThickness[1],YdThickness[2],YdThickness[3],YdThickness[4],YdThickness[5],YdThickness[6],YdThickness[7]);
	printf("Distance YY1: %.1f mm\n",YdDistance);
	printf("Inner radius YY1: %.1f mm\n",YdInnerRadius);
	printf("Outer radius YY1: %.1f mm\n",YdOuterRadius);
	printf("Thickness 1st S3: %.1f mg/cm^2\n",Sd1Thickness);
	printf("Distance 1st S3: %.1f mm\n",Sd1Distance);
	printf("Thickness 2nd S3: %.1f mg/cm^2\n",Sd2Thickness);
	printf("Distance 2nd S3: %.1f mm\n",Sd2Distance);
	printf("Inner radius S3: %.1f mm\n",SdInnerRadius);
	printf("Outer radius S3: %.1f mm\n",SdOuterRadius);
}
