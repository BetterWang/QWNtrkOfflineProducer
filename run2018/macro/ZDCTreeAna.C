#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

double EMscaleP = 0.1;
double EMscaleM = 0.1;
/* TS 4,5 calibration */
double nPHAD1 = 1.00;
double nPHAD2 = 0.61;
double nPHAD3 = 0.46;
double nPHAD4 = 0.50;
double nPEM1  = EMscaleP * 1.1096590;
double nPEM2  = EMscaleP * 1.2292624;
double nPEM3  = EMscaleP * 0.93249696;
double nPEM4  = EMscaleP * 1.0451228;
double nPEM5  = EMscaleP * 1.0;

double nMHAD1 = 1.00;
double nMHAD2 = 0.72;
double nMHAD3 = 0.46;
double nMHAD4 = 0.58;
double nMEM1  = EMscaleM * 0.83302228;
double nMEM2  = EMscaleM * 0.85790654;
double nMEM3  = EMscaleM * 0.94278081;
double nMEM4  = EMscaleM * 0.92705166;
double nMEM5  = EMscaleM * 1.0;

double P1nT1 = 2000.;
double P1nT2 = 11000.;
double M1nT1 = 5000.;
double M1nT2 = 17000.;

double P2nT1 = 12000.;
double P2nT2 = 18000.;
double M2nT1 = 18000.;
double M2nT2 = 30000.;

double P3nT1 = 18000.;
double P3nT2 = 24000.;
double M3nT1 = 30000.;
double M3nT2 = 42000.;

/*
double calib_PEM1 = 0.63303993;
double calib_PEM2 = 0.70174452;
double calib_PEM3 = 1.;
double calib_PEM4 = 0.76402055;
double calib_PEM5 = 0.67015023;

double calib_MEM1 = 0.57976449;
double calib_MEM2 = 0.68429638;
double calib_MEM3 = 1.;
double calib_MEM4 = 0.77988718;
double calib_MEM5 = 0.68628745;
*/
double calib_PEM1 = 1.;
double calib_PEM2 = 1.;
double calib_PEM3 = 1.;
double calib_PEM4 = 1.;
double calib_PEM5 = 1.;

double calib_MEM1 = 1.;
double calib_MEM2 = 1.;
double calib_MEM3 = 1.;
double calib_MEM4 = 1.;
double calib_MEM5 = 1.;

double calib_PEM[6] = {0, calib_PEM1, calib_PEM2, calib_PEM3, calib_PEM4, calib_PEM5};
double calib_MEM[6] = {0, calib_MEM1, calib_MEM2, calib_MEM3, calib_MEM4, calib_MEM5};

void loadTree(TChain* trV, TChain* trC, string input);
int peakTS(double* ch);

void
ZDCTreeAna(string input = "", string s = "test.root")
{
	TChain * trV = new TChain("trV");
	TChain * trC = new TChain("trC");
//	loadTree(trV, trC, input);

	trV->Add( (input + "/zdcana/fC/trV").c_str() );
	trC->Add( (input + "/QWHFTree/trV").c_str() );
	trV->AddFriend(trC);

	Double_t centBin;
	Double_t argp_HFQ2;
	Double_t argm_HFQ2;
	trC->SetBranchAddress("_dbCentBin", &centBin);
	trC->SetBranchAddress("argp_HFQ2", &argp_HFQ2);
	trC->SetBranchAddress("argm_HFQ2", &argm_HFQ2);

	TH1D* hCentBin = new TH1D("hCentBin", "hCentBin", 200, 0, 200);

	TH2D* hEMtoHF2xP[10];
	TH2D* hEMtoHF2xM[10];
	TH2D* hEMPtoEMM[10];
	for ( int c = 0; c < 10; c++ ) {
		hEMtoHF2xP[c] = new TH2D(Form("hEMtoHF2xP_%i", c), "hEMtoHF2xP", 100, -4, 4, 100, -3.1416, 3.1416);
		hEMtoHF2xM[c] = new TH2D(Form("hEMtoHF2xM_%i", c), "hEMtoHF2xM", 100, -4, 4, 100, -3.1416, 3.1416);
		hEMPtoEMM[c]  = new TH2D(Form("hEMPtoEMM_%i", c), "hEMPtoEMM", 100, -4, 4, 100, -4, 4);
	}

	TH1D* hNpeakP[10];
	TH1D* hNpeakM[10];
	TH1D* hNpeakPAll[10];
	TH1D* hNpeakMAll[10];

	TH2D* hNpeakPM[10];
	TH2D* hNpeakPMAll[10];

	for ( int c = 0; c < 10; c++ ) {
		hNpeakP[c] = new TH1D(Form("hNpeakP_%i", c), "hNpeakP", 1000, 0, 1000000);
		hNpeakM[c] = new TH1D(Form("hNpeakM_%i", c), "hNpeakM", 1000, 0, 1000000);
		hNpeakPAll[c] = new TH1D(Form("hNpeakPAll_%i", c), "hNpeakPAll", 1000, 0, 1000000);
		hNpeakMAll[c] = new TH1D(Form("hNpeakMAll_%i", c), "hNpeakMAll", 1000, 0, 1000000);

		hNpeakPM[c] = new TH2D(Form("hNpeakPM_%i", c), "hNpeakPM", 1000, 0, 1000000, 1000, 0, 1000000);
		hNpeakPMAll[c] = new TH2D(Form("hNpeakPMAll_%i", c), "hNpeakPMAll", 1000, 0, 1000000, 1000, 0, 1000000);
	}

	TH1D* hP1NEM[10][5];
	TH1D* hM1NEM[10][5];
	TH1D* hPAllEM[10][5];
	TH1D* hMAllEM[10][5];
	TH1D* hPIncEM[10][5];
	TH1D* hMIncEM[10][5];

	string t;
	for ( int c = 0; c < 10; c++ ) {
		for ( int i = 0; i < 5; i++ ) {
			t = string("hP1NEM")+to_string(i+1)+"_"+to_string(c);
			hP1NEM[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
			t = string("hM1NEM")+to_string(i+1)+"_"+to_string(c);
			hM1NEM[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

			t = string("hPAllEM")+to_string(i+1)+"_"+to_string(c);
			hPAllEM[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
			t = string("hMAllEM")+to_string(i+1)+"_"+to_string(c);
			hMAllEM[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

			t = string("hPIncEM")+to_string(i+1)+"_"+to_string(c);
			hPIncEM[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
			t = string("hMIncEM")+to_string(i+1)+"_"+to_string(c);
			hMIncEM[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
		}
	}

	TH1D* hP1NHAD[10][4];
	TH1D* hM1NHAD[10][4];
	TH1D* hP2NHAD[10][4];
	TH1D* hM2NHAD[10][4];
	TH1D* hP3NHAD[10][4];
	TH1D* hM3NHAD[10][4];

	TH1D* hP1NHADInc[10][4];
	TH1D* hM1NHADInc[10][4];
	TH1D* hP2NHADInc[10][4];
	TH1D* hM2NHADInc[10][4];
	TH1D* hP3NHADInc[10][4];
	TH1D* hM3NHADInc[10][4];

	TH1D* hPAllHAD[10][4];
	TH1D* hMAllHAD[10][4];
	TH1D* hPIncHAD[10][4];
	TH1D* hMIncHAD[10][4];
	for ( int c = 0; c < 10; c++ ) {
		for ( int i = 0; i < 4; i++ ) {
			t = string("hP1NHAD")+to_string(i+1)+"_"+to_string(c);
			hP1NHAD[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
			t = string("hM1NHAD")+to_string(i+1)+"_"+to_string(c);
			hM1NHAD[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

			t = string("hP2NHAD")+to_string(i+1)+"_"+to_string(c);
			hP2NHAD[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
			t = string("hM2NHAD")+to_string(i+1)+"_"+to_string(c);
			hM2NHAD[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

			t = string("hP3NHAD")+to_string(i+1)+"_"+to_string(c);
			hP3NHAD[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
			t = string("hM3NHAD")+to_string(i+1)+"_"+to_string(c);
			hM3NHAD[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

			t = string("hP1NHADInc")+to_string(i+1)+"_"+to_string(c);
			hP1NHADInc[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
			t = string("hM1NHADInc")+to_string(i+1)+"_"+to_string(c);
			hM1NHADInc[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

			t = string("hP2NHADInc")+to_string(i+1)+"_"+to_string(c);
			hP2NHADInc[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
			t = string("hM2NHADInc")+to_string(i+1)+"_"+to_string(c);
			hM2NHADInc[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

			t = string("hP3NHADInc")+to_string(i+1)+"_"+to_string(c);
			hP3NHADInc[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
			t = string("hM3NHADInc")+to_string(i+1)+"_"+to_string(c);
			hM3NHADInc[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

			t = string("hPAllHAD")+to_string(i+1)+"_"+to_string(c);
			hPAllHAD[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
			t = string("hMAllHAD")+to_string(i+1)+"_"+to_string(c);
			hMAllHAD[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

			t = string("hPIncHAD")+to_string(i+1)+"_"+to_string(c);
			hPIncHAD[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
			t = string("hMIncHAD")+to_string(i+1)+"_"+to_string(c);
			hMIncHAD[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
		}
	}

	TH1D* hP1NRPD[10][16];
	TH1D* hM1NRPD[10][16];
	TH1D* hPAllRPD[10][16];
	TH1D* hMAllRPD[10][16];
	TH1D* hPIncRPD[10][16];
	TH1D* hMIncRPD[10][16];
	for ( int c = 0; c < 10; c++ ) {
		for ( int i = 0; i < 16; i++ ) {
			t = string("hP1NRPD")+to_string(i)+"_"+to_string(c);
			hP1NRPD[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
			t = string("hM1NRPD")+to_string(i)+"_"+to_string(c);
			hM1NRPD[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

			t = string("hPAllRPD")+to_string(i)+"_"+to_string(c);
			hPAllRPD[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
			t = string("hMAllRPD")+to_string(i)+"_"+to_string(c);
			hMAllRPD[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

			t = string("hPIncRPD")+to_string(i)+"_"+to_string(c);
			hPIncRPD[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
			t = string("hMIncRPD")+to_string(i)+"_"+to_string(c);
			hMIncRPD[c][i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
		}
	}

	Double_t        hZDCM_EM1[10];
	Double_t        hZDCM_EM2[10];
	Double_t        hZDCM_EM3[10];
	Double_t        hZDCM_EM4[10];
	Double_t        hZDCM_EM5[10];
	Double_t        hZDCM_HAD1[10];
	Double_t        hZDCM_HAD2[10];
	Double_t        hZDCM_HAD3[10];
	Double_t        hZDCM_HAD4[10];
	Double_t        hZDCP_EM1[10];
	Double_t        hZDCP_EM2[10];
	Double_t        hZDCP_EM3[10];
	Double_t        hZDCP_EM4[10];
	Double_t        hZDCP_EM5[10];
	Double_t        hZDCP_HAD1[10];
	Double_t        hZDCP_HAD2[10];
	Double_t        hZDCP_HAD3[10];
	Double_t        hZDCP_HAD4[10];
	Double_t        hZDCM_RPD0[10];
	Double_t        hZDCM_RPD1[10];
	Double_t        hZDCM_RPD2[10];
	Double_t        hZDCM_RPD3[10];
	Double_t        hZDCM_RPD4[10];
	Double_t        hZDCM_RPD5[10];
	Double_t        hZDCM_RPD6[10];
	Double_t        hZDCM_RPD7[10];
	Double_t        hZDCM_RPD8[10];
	Double_t        hZDCM_RPD9[10];
	Double_t        hZDCM_RPD10[10];
	Double_t        hZDCM_RPD11[10];
	Double_t        hZDCM_RPD12[10];
	Double_t        hZDCM_RPD13[10];
	Double_t        hZDCM_RPD14[10];
	Double_t        hZDCM_RPD15[10];
	Double_t        hZDCP_RPD0[10];
	Double_t        hZDCP_RPD1[10];
	Double_t        hZDCP_RPD2[10];
	Double_t        hZDCP_RPD3[10];
	Double_t        hZDCP_RPD4[10];
	Double_t        hZDCP_RPD5[10];
	Double_t        hZDCP_RPD6[10];
	Double_t        hZDCP_RPD7[10];
	Double_t        hZDCP_RPD8[10];
	Double_t        hZDCP_RPD9[10];
	Double_t        hZDCP_RPD10[10];
	Double_t        hZDCP_RPD11[10];
	Double_t        hZDCP_RPD12[10];
	Double_t        hZDCP_RPD13[10];
	Double_t        hZDCP_RPD14[10];
	Double_t        hZDCP_RPD15[10];

	Double_t*	lZDCP_EM[5] = {hZDCP_EM1, hZDCP_EM2, hZDCP_EM3, hZDCP_EM4, hZDCP_EM5};
	Double_t*	lZDCM_EM[5] = {hZDCM_EM1, hZDCM_EM2, hZDCM_EM3, hZDCM_EM4, hZDCM_EM5};

	Double_t*	lZDCP_HAD[4] = {hZDCP_HAD1, hZDCP_HAD2, hZDCP_HAD3, hZDCP_HAD4};
	Double_t*	lZDCM_HAD[4] = {hZDCM_HAD1, hZDCM_HAD2, hZDCM_HAD3, hZDCM_HAD4};

	Double_t*	lZDCP_RPD[16] = {hZDCP_RPD0,
					 hZDCP_RPD1,
					 hZDCP_RPD2,
					 hZDCP_RPD3,
					 hZDCP_RPD4,
					 hZDCP_RPD5,
					 hZDCP_RPD6,
					 hZDCP_RPD7,
					 hZDCP_RPD8,
					 hZDCP_RPD9,
					 hZDCP_RPD10,
					 hZDCP_RPD11,
					 hZDCP_RPD12,
					 hZDCP_RPD13,
					 hZDCP_RPD14,
					 hZDCP_RPD15};

	Double_t*	lZDCM_RPD[16] = {hZDCM_RPD0,
					 hZDCM_RPD1,
					 hZDCM_RPD2,
					 hZDCM_RPD3,
					 hZDCM_RPD4,
					 hZDCM_RPD5,
					 hZDCM_RPD6,
					 hZDCM_RPD7,
					 hZDCM_RPD8,
					 hZDCM_RPD9,
					 hZDCM_RPD10,
					 hZDCM_RPD11,
					 hZDCM_RPD12,
					 hZDCM_RPD13,
					 hZDCM_RPD14,
					 hZDCM_RPD15};

	trV->SetBranchAddress("hZDCM_EM1", hZDCM_EM1);
	trV->SetBranchAddress("hZDCM_EM2", hZDCM_EM2);
	trV->SetBranchAddress("hZDCM_EM3", hZDCM_EM3);
	trV->SetBranchAddress("hZDCM_EM4", hZDCM_EM4);
	trV->SetBranchAddress("hZDCM_EM5", hZDCM_EM5);
	trV->SetBranchAddress("hZDCM_HAD1", hZDCM_HAD1);
	trV->SetBranchAddress("hZDCM_HAD2", hZDCM_HAD2);
	trV->SetBranchAddress("hZDCM_HAD3", hZDCM_HAD3);
	trV->SetBranchAddress("hZDCM_HAD4", hZDCM_HAD4);
	trV->SetBranchAddress("hZDCP_EM1", hZDCP_EM1);
	trV->SetBranchAddress("hZDCP_EM2", hZDCP_EM2);
	trV->SetBranchAddress("hZDCP_EM3", hZDCP_EM3);
	trV->SetBranchAddress("hZDCP_EM4", hZDCP_EM4);
	trV->SetBranchAddress("hZDCP_EM5", hZDCP_EM5);
	trV->SetBranchAddress("hZDCP_HAD1", hZDCP_HAD1);
	trV->SetBranchAddress("hZDCP_HAD2", hZDCP_HAD2);
	trV->SetBranchAddress("hZDCP_HAD3", hZDCP_HAD3);
	trV->SetBranchAddress("hZDCP_HAD4", hZDCP_HAD4);
	trV->SetBranchAddress("hZDCM_RPD0", hZDCM_RPD0);
	trV->SetBranchAddress("hZDCM_RPD1", hZDCM_RPD1);
	trV->SetBranchAddress("hZDCM_RPD2", hZDCM_RPD2);
	trV->SetBranchAddress("hZDCM_RPD3", hZDCM_RPD3);
	trV->SetBranchAddress("hZDCM_RPD4", hZDCM_RPD4);
	trV->SetBranchAddress("hZDCM_RPD5", hZDCM_RPD5);
	trV->SetBranchAddress("hZDCM_RPD6", hZDCM_RPD6);
	trV->SetBranchAddress("hZDCM_RPD7", hZDCM_RPD7);
	trV->SetBranchAddress("hZDCM_RPD8", hZDCM_RPD8);
	trV->SetBranchAddress("hZDCM_RPD9", hZDCM_RPD9);
	trV->SetBranchAddress("hZDCM_RPD10", hZDCM_RPD10);
	trV->SetBranchAddress("hZDCM_RPD11", hZDCM_RPD11);
	trV->SetBranchAddress("hZDCM_RPD12", hZDCM_RPD12);
	trV->SetBranchAddress("hZDCM_RPD13", hZDCM_RPD13);
	trV->SetBranchAddress("hZDCM_RPD14", hZDCM_RPD14);
	trV->SetBranchAddress("hZDCM_RPD15", hZDCM_RPD15);
	trV->SetBranchAddress("hZDCP_RPD0", hZDCP_RPD0);
	trV->SetBranchAddress("hZDCP_RPD1", hZDCP_RPD1);
	trV->SetBranchAddress("hZDCP_RPD2", hZDCP_RPD2);
	trV->SetBranchAddress("hZDCP_RPD3", hZDCP_RPD3);
	trV->SetBranchAddress("hZDCP_RPD4", hZDCP_RPD4);
	trV->SetBranchAddress("hZDCP_RPD5", hZDCP_RPD5);
	trV->SetBranchAddress("hZDCP_RPD6", hZDCP_RPD6);
	trV->SetBranchAddress("hZDCP_RPD7", hZDCP_RPD7);
	trV->SetBranchAddress("hZDCP_RPD8", hZDCP_RPD8);
	trV->SetBranchAddress("hZDCP_RPD9", hZDCP_RPD9);
	trV->SetBranchAddress("hZDCP_RPD10", hZDCP_RPD10);
	trV->SetBranchAddress("hZDCP_RPD11", hZDCP_RPD11);
	trV->SetBranchAddress("hZDCP_RPD12", hZDCP_RPD12);
	trV->SetBranchAddress("hZDCP_RPD13", hZDCP_RPD13);
	trV->SetBranchAddress("hZDCP_RPD14", hZDCP_RPD14);
	trV->SetBranchAddress("hZDCP_RPD15", hZDCP_RPD15);


	int idx = 0;
	while ( trV->GetEntry(idx) ) {
		if (idx%1000 == 0) cout << " --> idx = " << idx << endl;
		int c = int(centBin)/20;
		hCentBin->Fill(centBin);
		double P_HAD[5] = { 0,
			hZDCP_HAD1[4] + hZDCP_HAD1[5],
			hZDCP_HAD2[4] + hZDCP_HAD2[5],
			hZDCP_HAD3[4] + hZDCP_HAD3[5],
			hZDCP_HAD4[4] + hZDCP_HAD4[5] };

		double M_HAD[5] = { 0,
			hZDCM_HAD1[4] + hZDCM_HAD1[5],
			hZDCM_HAD2[4] + hZDCM_HAD2[5],
			hZDCM_HAD3[4] + hZDCM_HAD3[5],
			hZDCM_HAD4[4] + hZDCM_HAD4[5] };

		double P_EM[6] = { 0,
			hZDCP_EM1[4] + hZDCP_EM1[5],
			hZDCP_EM2[4] + hZDCP_EM2[5],
			hZDCP_EM3[4] + hZDCP_EM3[5],
			hZDCP_EM4[4] + hZDCP_EM4[5],
			hZDCP_EM5[4] + hZDCP_EM5[5] };

		double M_EM[6] = { 0,
			hZDCM_EM1[4] + hZDCM_EM1[5],
			hZDCM_EM2[4] + hZDCM_EM2[5],
			hZDCM_EM3[4] + hZDCM_EM3[5],
			hZDCM_EM4[4] + hZDCM_EM4[5],
			hZDCM_EM5[4] + hZDCM_EM5[5] };

		double P_RPD[16], M_RPD[16];
		for ( int i = 0; i < 16; i++ ) {
			P_RPD[i] = lZDCP_RPD[i][4] + lZDCP_RPD[i][5];
			M_RPD[i] = lZDCM_RPD[i][4] + lZDCM_RPD[i][5];
		}


		bool P_select = ((peakTS(hZDCP_HAD1)==4) and (peakTS(hZDCP_HAD2)==4));
		bool M_select = ((peakTS(hZDCM_HAD1)==4) and (peakTS(hZDCP_HAD2)==4));

		double P_NpeakT = nPHAD1*P_HAD[1] + nPHAD2*P_HAD[2] + nPHAD3*P_HAD[3] + nPHAD4*P_HAD[4] + nPEM1*P_EM[1] + nPEM2*P_EM[2] + nPEM3*P_EM[3] + nPEM4*P_EM[4] + nPEM5*P_EM[5];
		double M_NpeakT = nMHAD1*M_HAD[1] + nMHAD2*M_HAD[2] + nMHAD3*M_HAD[3] + nMHAD4*M_HAD[4] + nMEM1*M_EM[1] + nMEM2*M_EM[2] + nMEM3*M_EM[3] + nMEM4*M_EM[4] + nMEM5*M_EM[5];

		bool bP1nT = (P_NpeakT > P1nT1) and (P_NpeakT < P1nT2);
		bool bM1nT = (M_NpeakT > M1nT1) and (M_NpeakT < M1nT2);
		bool bP2nT = (P_NpeakT > P2nT1) and (P_NpeakT < P2nT2);
		bool bM2nT = (M_NpeakT > M2nT1) and (M_NpeakT < M2nT2);
		bool bP3nT = (P_NpeakT > P3nT1) and (P_NpeakT < P3nT2);
		bool bM3nT = (M_NpeakT > M3nT1) and (M_NpeakT < M3nT2);


		hNpeakPAll[c]->Fill(P_NpeakT);
		hNpeakMAll[c]->Fill(M_NpeakT);
		hNpeakPMAll[c]->Fill(P_NpeakT, M_NpeakT);

		if ( P_select ) hNpeakP[c]->Fill(P_NpeakT);
		if ( M_select ) hNpeakM[c]->Fill(M_NpeakT);

		if ( P_select and M_select ) {
			hNpeakPM[c]->Fill(P_NpeakT, M_NpeakT);
		}

		if ( P_select and bP1nT ) {
			for ( int i = 0; i < 5; i++ ) {
				hP1NEM[c][i]->Fill(P_EM[i+1] * calib_PEM[i+1]);
			}
			for ( int i = 0; i < 4; i++ ) {
				hP1NHAD[c][i]->Fill(P_HAD[i+1]);
			}
			for ( int i = 0; i < 16; i++ ) {
				hP1NRPD[c][i]->Fill(P_RPD[i]);
			}
		}
		if ( P_select and bP2nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hP2NHAD[c][i]->Fill(P_HAD[i+1]);
			}
		}
		if ( P_select and bP3nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hP3NHAD[c][i]->Fill(P_HAD[i+1]);
			}
		}
		if ( bP1nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hP1NHADInc[c][i]->Fill(P_HAD[i+1]);
			}
		}
		if ( bP2nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hP2NHADInc[c][i]->Fill(P_HAD[i+1]);
			}
		}
		if ( bP3nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hP3NHADInc[c][i]->Fill(P_HAD[i+1]);
			}
		}

		if ( P_select ) {
			for ( int i = 0; i < 5; i++ ) {
				hPAllEM[c][i]->Fill(P_EM[i+1] * calib_PEM[i+1]);
			}
			for ( int i = 0; i < 4; i++ ) {
				hPAllHAD[c][i]->Fill(P_HAD[i+1]);
			}
			for ( int i = 0; i < 16; i++ ) {
				hPAllRPD[c][i]->Fill(P_RPD[i]);
			}
		}
		for ( int i = 0; i < 5; i++ ) {
			hPIncEM[c][i]->Fill(P_EM[i+1] * calib_PEM[i+1]);
		}
		for ( int i = 0; i < 4; i++ ) {
			hPIncHAD[c][i]->Fill(P_HAD[i+1]);
		}
		for ( int i = 0; i < 16; i++ ) {
			hPIncRPD[c][i]->Fill(P_RPD[i]);
		}

		if ( M_select and bM1nT ) {
			for ( int i = 0; i < 5; i++ ) {
				hM1NEM[c][i]->Fill(M_EM[i+1] * calib_MEM[i+1]);
			}
			for ( int i = 0; i < 4; i++ ) {
				hM1NHAD[c][i]->Fill(M_HAD[i+1]);
			}
			for ( int i = 0; i < 16; i++ ) {
				hM1NRPD[c][i]->Fill(M_RPD[i]);
			}
		}
		if ( M_select and bM2nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hM2NHAD[c][i]->Fill(M_HAD[i+1]);
			}
		}
		if ( M_select and bM3nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hM3NHAD[c][i]->Fill(M_HAD[i+1]);
			}
		}
		if ( bM1nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hM1NHADInc[c][i]->Fill(M_HAD[i+1]);
			}
		}
		if ( bM2nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hM2NHADInc[c][i]->Fill(M_HAD[i+1]);
			}
		}
		if ( bM3nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hM3NHADInc[c][i]->Fill(M_HAD[i+1]);
			}
		}

		if ( M_select ) {
			for ( int i = 0; i < 5; i++ ) {
				hMAllEM[c][i]->Fill(M_EM[i+1] * calib_MEM[i+1]);
			}
			for ( int i = 0; i < 4; i++ ) {
				hMAllHAD[c][i]->Fill(M_HAD[i+1]);
			}
			for ( int i = 0; i < 16; i++ ) {
				hMAllRPD[c][i]->Fill(M_RPD[i]);
			}
		}
		for ( int i = 0; i < 5; i++ ) {
			hMIncEM[c][i]->Fill(M_EM[i+1] * calib_MEM[i+1]);
		}
		for ( int i = 0; i < 4; i++ ) {
			hMIncHAD[c][i]->Fill(M_HAD[i+1]);
		}
		for ( int i = 0; i < 16; i++ ) {
			hMIncRPD[c][i]->Fill(M_RPD[i]);
		}

		if ( P_select ) {
			double EM = (4*nPEM1*P_EM[1] + 2*nPEM2*P_EM[2] - 2*nPEM4*P_EM[4] - 4*nPEM5*P_EM[5]) / (nPEM1*P_EM[1] + nPEM2*P_EM[2] + nPEM2*P_EM[3] + nPEM4*P_EM[4] + nPEM5*P_EM[5]);
			hEMtoHF2xP[c]->Fill(EM, argp_HFQ2);
		}
		if ( M_select ) {
			double EM = (4*nMEM1*M_EM[1] + 2*nMEM2*M_EM[2] - 2*nMEM4*M_EM[4] - 4*nMEM5*M_EM[5]) / (nMEM1*M_EM[1] + nMEM2*M_EM[2] + nMEM2*M_EM[3] + nMEM4*M_EM[4] + nMEM5*M_EM[5]);
			hEMtoHF2xM[c]->Fill(EM, argm_HFQ2);
		}

		if ( P_select and M_select ) {
			double EMP = (4*nPEM1*P_EM[1] + 2*nPEM2*P_EM[2] - 2*nPEM4*P_EM[4] - 4*nPEM5*P_EM[5]) / (nPEM1*P_EM[1] + nPEM2*P_EM[2] + nPEM2*P_EM[3] + nPEM4*P_EM[4] + nPEM5*P_EM[5]);
			double EMM = (4*nMEM1*M_EM[1] + 2*nMEM2*M_EM[2] - 2*nMEM4*M_EM[4] - 4*nMEM5*M_EM[5]) / (nMEM1*M_EM[1] + nMEM2*M_EM[2] + nMEM2*M_EM[3] + nMEM4*M_EM[4] + nMEM5*M_EM[5]);
			hEMPtoEMM[c]->Fill(EMP, EMM);
		}
		idx++;
	}


	TFile * fsave = new TFile(s.c_str(), "recreate");
	for ( int c = 0; c < 10; c++ ) {
		hNpeakP[c]->Write();
		hNpeakM[c]->Write();
		hNpeakPM[c]->Write();
		hNpeakPAll[c]->Write();
		hNpeakMAll[c]->Write();
		hNpeakPMAll[c]->Write();
	}
	hCentBin->Write();
	auto fEM = fsave->mkdir("EM");
	fEM->cd();
	for ( int c = 0; c < 10; c++ ) {
		for ( int i = 0; i < 5; i++ ) {
			hP1NEM[c][i] ->Write();
			hM1NEM[c][i] ->Write();
			hPAllEM[c][i]->Write();
			hMAllEM[c][i]->Write();
			hPIncEM[c][i]->Write();
			hMIncEM[c][i]->Write();
		}
	}
	auto fHad = fsave->mkdir("HAD");
	fHad->cd();
	for ( int c = 0; c < 10; c++ ) {
		for ( int i = 0; i < 4; i++ ) {
			hP1NHAD[c][i] ->Write();
			hM1NHAD[c][i] ->Write();
			hP2NHAD[c][i] ->Write();
			hM2NHAD[c][i] ->Write();
			hP3NHAD[c][i] ->Write();
			hM3NHAD[c][i] ->Write();

			hP1NHADInc[c][i] ->Write();
			hM1NHADInc[c][i] ->Write();
			hP2NHADInc[c][i] ->Write();
			hM2NHADInc[c][i] ->Write();
			hP3NHADInc[c][i] ->Write();
			hM3NHADInc[c][i] ->Write();

			hPAllHAD[c][i]->Write();
			hMAllHAD[c][i]->Write();
			hPIncHAD[c][i]->Write();
			hMIncHAD[c][i]->Write();
		}
	}
	auto fRPD = fsave->mkdir("RPD");
	fRPD->cd();
	for ( int c = 0; c < 10; c++ ) {
		for ( int i = 0; i < 16; i++ ) {
			hP1NRPD[c][i] ->Write();
			hM1NRPD[c][i] ->Write();
			hPAllRPD[c][i]->Write();
			hMAllRPD[c][i]->Write();
			hPIncRPD[c][i]->Write();
			hMIncRPD[c][i]->Write();
		}
	}
	auto fEMtoHF2 = fsave->mkdir("EMtoHF2");
	fEMtoHF2->cd();
	for ( int c = 0; c < 10; c++ ) {
		hEMtoHF2xP[c]->Write();
		hEMtoHF2xM[c]->Write();
		hEMPtoEMM[c]->Write();
	}
}


void loadTree(TChain* trV, TChain* trC, string input)
{
//	trV->Add( "/*.root/zdcana/fC/trV").c_str() );

//	trV->SetMakeClass(1);
}

int peakTS(double* ch)
{
	return std::distance(ch, std::max_element(ch, ch+10));
}
