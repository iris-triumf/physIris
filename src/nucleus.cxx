#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include "nucleus.h"

nucleus::nucleus()
{
	name.clear(); 
	N=-1;
	Z=-1;
	A=-1; 
	mass=0.;  
	Sp=0.; 
	Sn=0.; 
	S2p=0.; 
	S2n=0.; 
}
void nucleus::Clear()
{
	name.clear(); 
	N=-1;
	Z=-1;
	A=-1; 
	mass=0.; 
	Sp=0.; 
	Sn=0.; 
	S2p=0.; 
	S2n=0.; 
}

void nucleus::ReadFile(Int_t inputA, std::string inpEl)
{
	const Double_t Amu=931.494061;
	char buffer[124];
	std::string cN,cZ,cA,cEl,cME,cAM;
	
	Double_t ME,AM;
	Double_t mass1=0.;
	Double_t mass2=0.;
	
	FILE *massfile;
	massfile=fopen("ame/mass.mas12","r");
	if(massfile==NULL) massfile=fopen("../ame/mass.mas12","r");
	if(massfile==NULL){
		printf("Cannot open mass table !!\n");
		exit(0);
	}

	while(!feof(massfile)){
		if(!fgets(buffer,124,massfile)) break;

		cN.assign(buffer+6,buffer+9);
		cN.erase( std::remove_if( cN.begin(), cN.end(), ::isspace ), cN.end() );
		cZ.assign(buffer+11,buffer+14);
		cZ.erase( std::remove_if( cZ.begin(), cZ.end(), ::isspace ), cZ.end() );
		cA.assign(buffer+16,buffer+19);
		cA.erase( std::remove_if( cA.begin(), cA.end(), ::isspace ), cA.end() );
		cEl.assign(buffer+20,buffer+22);
		cEl.erase( std::remove_if( cEl.begin(), cEl.end(), ::isspace ), cEl.end() );
		cME.assign(buffer+30,buffer+41);
		cME.erase( std::remove_if( cME.begin(), cME.end(), ::isspace ), cME.end() );
		cAM.assign(buffer+96,buffer+112);
		cAM.erase( std::remove_if( cAM.begin(), cAM.end(), ::isspace ), cAM.end() );
		sscanf(cN.data(),"%d",&N);
		sscanf(cZ.data(),"%d",&Z);
		sscanf(cA.data(),"%d",&A);
		sscanf(cME.data(),"%lf",&ME);
		sscanf(cAM.data(),"%lf",&AM);
		mass1=Amu*A+ME/1000.;
		mass2=Amu*AM/1000000.;
		if(A==inputA&&cEl==inpEl) {
			mass=mass1;
			name=cA+cEl;
			printf("\tN=%d\tZ=%d\tA=%d\t%s\t%.3lf\t%.3lf\n",N,Z,A,cEl.data(),mass1,mass2);
			break;
		}
	}
	fclose(massfile);

	FILE *rctfile1;
	rctfile1=fopen("ame/rct1.mas12","r");
	if(rctfile1==NULL) rctfile1=fopen("../ame/rct1.mas12","r");
	if(rctfile1==NULL){
		printf("Cannot open first separation energy table !!\n");
		exit(0);
	}

	std::string cA1,cEl1,cS2n,cS2p;
	Int_t A1=-1;

	while(!feof(rctfile1)){
		if(!fgets(buffer,124,rctfile1)) break;

		cA1.assign(buffer+1,buffer+4);
		cA1.erase( std::remove_if( cA1.begin(), cA1.end(), ::isspace ), cA1.end() );
		cEl1.assign(buffer+5,buffer+7);
		cEl1.erase( std::remove_if( cEl1.begin(), cEl1.end(), ::isspace ), cEl1.end() );
		cS2n.assign(buffer+13,buffer+22);
		cS2n.erase( std::remove_if( cS2n.begin(), cS2n.end(), ::isspace ), cS2n.end() );
		cS2p.assign(buffer+32,buffer+40);
		cS2p.erase( std::remove_if( cS2p.begin(), cS2p.end(), ::isspace ), cS2p.end() );
		sscanf(cA1.data(),"%d",&A1);
		if(cS2n=="*") S2n=0.;
		else sscanf(cS2n.data(),"%lf",&S2n);
		if(cS2p=="*") S2p=0.;
		else sscanf(cS2p.data(),"%lf",&S2p);
		if(A1==inputA&&cEl1==inpEl) {
			S2n = S2n/1000.;
			S2p = S2p/1000.;
			printf("\tS(2p)=%.3lf\tS(2n)=%.3lf\n",S2p,S2n);
			break;
		}
	}
	fclose(rctfile1);

	FILE *rctfile2;
	rctfile2=fopen("ame/rct2.mas12","r");
	if(rctfile2==NULL) rctfile2=fopen("../ame/rct2.mas12","r");
	if(rctfile2==NULL){
		printf("Cannot open second separation energy table !!\n");
		exit(0);
	}

	std::string cA2,cEl2,cSn,cSp;
	Int_t A2=-1;

	while(!feof(rctfile2)){
		if(!fgets(buffer,124,rctfile2)) break;

		cA2.assign(buffer+1,buffer+4);
		cA2.erase( std::remove_if( cA2.begin(), cA2.end(), ::isspace ), cA2.end() );
		cEl2.assign(buffer+5,buffer+7);
		cEl2.erase( std::remove_if( cEl2.begin(), cEl2.end(), ::isspace ), cEl2.end() );
		cSn.assign(buffer+13,buffer+22);
		cSn.erase( std::remove_if( cSn.begin(), cSn.end(), ::isspace ), cSn.end() );
		cSp.assign(buffer+32,buffer+40);
		cSp.erase( std::remove_if( cSp.begin(), cSp.end(), ::isspace ), cSp.end() );
		sscanf(cA2.data(),"%d",&A2);
		if(cSn=="*") Sn=0.;
		else sscanf(cSn.data(),"%lf",&Sn);
		if(cSp=="*") Sp=0.;
		else sscanf(cSp.data(),"%lf",&Sp);
		if(A2==inputA&&cEl2==inpEl) {
			Sn = Sn/1000.;
			Sp = Sp/1000.;
			printf("\tS(p) =%.3lf\tS(n) =%.3lf\n",Sp,Sn);
			break;
		}
	}
	fclose(rctfile2);

	return;
}

void nucleus::getInfo(Int_t inputN, Int_t inputZ)
{
	Int_t inputA = inputN+inputZ;
	Int_t N0, Z0;
	char buffer[124];
	std::string cN,cZ,cEl;

	FILE *massfile;
	massfile=fopen("ame/mass.mas12","r");
	if(massfile==NULL) massfile=fopen("../ame/mass.mas12","r");
	if(massfile==NULL){
		printf("Cannot open mass table !!\n");
		exit(0);
	}

	while(!feof(massfile)){
		if(!fgets(buffer,124,massfile)) break;

		cN.assign(buffer+6,buffer+9);
		cN.erase( std::remove_if( cN.begin(), cN.end(), ::isspace ), cN.end() );
		cZ.assign(buffer+11,buffer+14);
		cZ.erase( std::remove_if( cZ.begin(), cZ.end(), ::isspace ), cZ.end() );
		cEl.assign(buffer+20,buffer+22);
		cEl.erase( std::remove_if( cEl.begin(), cEl.end(), ::isspace ), cEl.end() );
		sscanf(cN.data(),"%d",&N0);
		sscanf(cZ.data(),"%d",&Z0);
		if(N0==inputN&&Z0==inputZ) {
			break;
		}
	}
	fclose(massfile);

//	std::string inpEl(inputEl);
//	inpEl.erase( std::remove_if( inpEl.begin(), inpEl.end(), ::isspace ), inpEl.end() );

	printf("%d%s\t===================================================================\n",inputA,cEl.data());
	ReadFile(inputA,cEl);
	
	return;
}

void nucleus::getInfo(std::string input)
{
	char inputEl[2];
	Int_t inputA;

	if(input=="p") input="1H";
	else if(input=="d") input="2H";
	else if(input=="t") input="3H";
	else if(input=="n") input="1n";

	sscanf(input.data(),"%d%s",&inputA,inputEl);
	
	std::string inpEl(inputEl);
	inpEl.erase( std::remove_if( inpEl.begin(), inpEl.end(), ::isspace ), inpEl.end() );

	printf("%d%s\t===================================================================\n",inputA,inpEl.data());
	ReadFile(inputA,inpEl);
	
	return;
}

void nucleus::Print()
{	
	printf("====\t%s\t====\n",name.data());
	printf("N=%d\tZ=%d\tA=%d\tmass=%lf\n",N,Z,A,mass);
	printf("S(p)=%lf\tS(n)=%lf\tS(2p)=%lf\tS(2n)=%lf\n",Sp,Sn,S2p,S2n);
	return;
}
