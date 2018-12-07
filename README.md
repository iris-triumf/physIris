# physIris
Analysis software for the IRIS experiment @ TRIUMF

## Download and Installation ##
	
treeIris requires ROOT (https://root.cern.ch).

To install physIris use

	git clone https://github.com/iris-triumf/physIris

and compile physIris with

	make


## Running physIris ##

From the main physIris folder, you can run physIris with
 
	./bin/physIris -c=/path/to/your/config.file -o=/path/to/to/your/output.file /path/to/your/input-folder

The input folder is the folder containing root files created using treeIris, the output file will be a root file.


## The Config File ##

In the main config file the paths to further files which in turn contain detector calibration parameters are defined. The structure is always

	FILETYPE=/path/to/calibration.file

The following files can be specified

	RUNLIST : List of all runs to be analyzed, usually taken from MIDAS runlog.txt.
	GATEFILE : root file containing graphical cut on nuclei of interest
	GATENAME : Name of graphical cut in GATEFILE
	DEDX_I : Files containing SRIM/LISE++ stopping power tables for incoming nuclei.
	DEDX_L : Files containing SRIM/LISE++ stopping power tables for light (target-like) nuclei.
	DEDX_H : Files containing SRIM/LISE++ stopping power tables for heavy (beam-like) nuclei.
	RUNDEPPAR : Run dependant parameters. See section below.
	GEOMETRY : Contains information on detector distances and thicknesses, and the target thickness and configuration. 

## Run Dependant Parameters ##

physIris calculates ...The file defined under RUNDEPPAR in the main config file is needed to define the reaction that is analyzed.

The reaction is defined as A(a,b)B @ EBAC MeV with
	EBAC: Beam energy as delivered by the accelerator. physIris will calculate the beam energy at the center of the target using the energy loss in IC, silver foil and half the target from this.
	A: Incoming projectile
	a: Target
	B: Scattered beam-like particle
	b: target-like particle

If the beam energy and target thickness changed throughout the experiment, a file containing run-by-run list of the values can be specified using RUNPAR. If this file is specified, the values in this file will override EBAC and the target thickness given in the geometry file. Lastly, the lower and upper limit of the IC gate is defined using ICMIN and ICMAX.

## The Output File Structure ##

The resulting output is a root file containing the TTree Iris. In addition to the branches already present in the input files, it contains the following branches:

	fEYY1: dead-layer corrected YY1 energy
	fECsI1: dead-layer corrected CsI1 energy
	fECsI2: dead-layer corrected CsI2 energy
	
	fThetacm1/2/U: center-of-mass angle from CsI1/CsI2/upstream detectors
	fThetaD/DU: downstream/upstream angles
	
	fEBAC: beam energy from accelerator
	fmA/a/B/b: mass of particle A,a,B,b
	fEBeam: beam energy at center of target
	fbetaCM: beam beta
	fgammaCM: beam gamma
	fPA: beam momentum at center of target
	
	fEb1/2/U/USd: Reconstructed light particle energy using CsI1/CsI2/upstream YY1/upstream S3 
	fPb1/2/U/USd: Reconstructed light particle momentum using CsI1/CsI2/upstream YY1/upstream S3 
	fPb1/2/U/USdy: Reconstructed light particle momentum y-component using CsI1/CsI2/upstream YY1/upstream S3
	fPb1/2/U/USdxcm: Reconstructed light particle momentum x-component in center-of-mass using CsI1/CsI2/upstream YY1/upstream S3
 
	fLP:	Light particle energy
	fHP:	Heavy particle energy
	fEB:	Measured heavy particle energy
	
	fEB1/2/U/USd:	Calculated heavy particle energy using CsI1/CsI2/upstream YY1/upstream S3
	fPB1/2/U/USd:	Calculated heavy particle momentum using CsI1/CsI2/upstream YY1/upstream S3
	fQv1/2/U/USd:	Q-value using CsI1/CsI2/upstream YY1/upstream S3
	
Apart from that, the branches TSdETot and TYdCsI1/2ETot, i.e. the dead-layer corrected total energies are filled by physIris. 

A few other variables are maybe left in for debugging of the Q-value calculation:

	fkBF: Ratio of Beam particle mass and 109-Ag foil nucleus mass
	fA/B/C: quadratic equation parameters
		A = kBF-1.;
		B = 2.0*PResid* cos(TMath::DegToRad()*det->TSd1Theta.at(0));
		C = -1.*(kBF+1)*PResid*PResid; 
	fPResid: Momentum of residue
		PResid = sqrt(2.*det->TSdETot*mA);
	fPBeam: Calculated beam momentum after scattering off Ag
		if (A!=0)    PBeam = (sqrt(B*B-4.*A*C)-B)/(2*A);



