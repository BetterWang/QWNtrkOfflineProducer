#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

/* TS 4,5 calibration */
double nPHAD1 = 1.00;
double nPHAD2 = 0.61;
double nPHAD3 = 0.46;
double nPHAD4 = 0.50;
double nPEM1  = 0.10;
double nPEM2  = 0.10;
double nPEM3  = 0.10;
double nPEM4  = 0.10;
double nPEM5  = 0.10;

double nMHAD1 = 1.00;
double nMHAD2 = 0.72;
double nMHAD3 = 0.46;
double nMHAD4 = 0.58;
double nMEM1  = 0.10;
double nMEM2  = 0.10;
double nMEM3  = 0.10;
double nMEM4  = 0.10;
double nMEM5  = 0.10;

double P1nT1 = 2000.;
double P1nT2 = 12000.;
double M1nT1 = 5000.;
double M1nT2 = 18000.;

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

void loadTree(TChain* trV, string input);
int peakTS(double* ch);

void
ZDCTreeAna(string input = "", string s = "test.root")
{
	TChain * trV = new TChain("trV");
	TChain * trF = new TChain("trF");
//	loadTree(trV, input);
//
	trV->Add( (input + "/zdcana/fC/trV").c_str() );
	trF->Add( (input + "/QWfilter/trV").c_str() );
	trV->AddFriend(trF);

	Int_t           digiPath;
	Int_t           pprimaryVertexFilter;
	Int_t           phfCoincFilter2Th4;
	Int_t           pclusterCompatibilityFilter;

	trF->SetBranchAddress("digiPath", &digiPath);
	trF->SetBranchAddress("pprimaryVertexFilter", &pprimaryVertexFilter);
	trF->SetBranchAddress("phfCoincFilter2Th4", &phfCoincFilter2Th4);
	trV->SetBranchAddress("pclusterCompatibilityFilter", &pclusterCompatibilityFilter);



	TH1D* hNpeakP = new TH1D("hNpeakP", "hNpeakP", 1000, 0, 1000000);
	TH1D* hNpeakM = new TH1D("hNpeakM", "hNpeakM", 1000, 0, 1000000);
	TH1D* hNpeakPAll = new TH1D("hNpeakPAll", "hNpeakPAll", 1000, 0, 1000000);
	TH1D* hNpeakMAll = new TH1D("hNpeakMAll", "hNpeakMAll", 1000, 0, 1000000);

	TH2D* hNpeakPM = new TH2D("hNpeakPM", "hNpeakPM", 1000, 0, 1000000, 1000, 0, 1000000);
	TH2D* hNpeakPMAll = new TH2D("hNpeakPMAll", "hNpeakPMAll", 1000, 0, 1000000, 1000, 0, 1000000);

	TH1D* hP1NEM[5];
	TH1D* hM1NEM[5];
	TH1D* hPAllEM[5];
	TH1D* hMAllEM[5];
	TH1D* hPIncEM[5];
	TH1D* hMIncEM[5];
	string t;
	for ( int i = 0; i < 5; i++ ) {
		t = string("hP1NEM")+to_string(i+1);
		hP1NEM[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
		t = string("hM1NEM")+to_string(i+1);
		hM1NEM[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

		t = string("hPAllEM")+to_string(i+1);
		hPAllEM[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
		t = string("hMAllEM")+to_string(i+1);
		hMAllEM[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

		t = string("hPIncEM")+to_string(i+1);
		hPIncEM[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
		t = string("hMIncEM")+to_string(i+1);
		hMIncEM[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
	}

	TH1D* hP1NHAD[4];
	TH1D* hM1NHAD[4];
	TH1D* hP2NHAD[4];
	TH1D* hM2NHAD[4];
	TH1D* hP3NHAD[4];
	TH1D* hM3NHAD[4];

	TH1D* hP1NHADInc[4];
	TH1D* hM1NHADInc[4];
	TH1D* hP2NHADInc[4];
	TH1D* hM2NHADInc[4];
	TH1D* hP3NHADInc[4];
	TH1D* hM3NHADInc[4];

	TH1D* hPAllHAD[4];
	TH1D* hMAllHAD[4];
	TH1D* hPIncHAD[4];
	TH1D* hMIncHAD[4];
	for ( int i = 0; i < 4; i++ ) {
		t = string("hP1NHAD")+to_string(i+1);
		hP1NHAD[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
		t = string("hM1NHAD")+to_string(i+1);
		hM1NHAD[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

		t = string("hP2NHAD")+to_string(i+1);
		hP2NHAD[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
		t = string("hM2NHAD")+to_string(i+1);
		hM2NHAD[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

		t = string("hP3NHAD")+to_string(i+1);
		hP3NHAD[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
		t = string("hM3NHAD")+to_string(i+1);
		hM3NHAD[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

		t = string("hP1NHADInc")+to_string(i+1);
		hP1NHADInc[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
		t = string("hM1NHADInc")+to_string(i+1);
		hM1NHADInc[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

		t = string("hP2NHADInc")+to_string(i+1);
		hP2NHADInc[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
		t = string("hM2NHADInc")+to_string(i+1);
		hM2NHADInc[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

		t = string("hP3NHADInc")+to_string(i+1);
		hP3NHADInc[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
		t = string("hM3NHADInc")+to_string(i+1);
		hM3NHADInc[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);




		t = string("hPAllHAD")+to_string(i+1);
		hPAllHAD[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
		t = string("hMAllHAD")+to_string(i+1);
		hMAllHAD[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

		t = string("hPIncHAD")+to_string(i+1);
		hPIncHAD[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
		t = string("hMIncHAD")+to_string(i+1);
		hMIncHAD[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
	}

	TH1D* hP1NRPD[16];
	TH1D* hM1NRPD[16];
	TH1D* hPAllRPD[16];
	TH1D* hMAllRPD[16];
	TH1D* hPIncRPD[16];
	TH1D* hMIncRPD[16];
	for ( int i = 0; i < 16; i++ ) {
		t = string("hP1NRPD")+to_string(i);
		hP1NRPD[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
		t = string("hM1NRPD")+to_string(i);
		hM1NRPD[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

		t = string("hPAllRPD")+to_string(i);
		hPAllRPD[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
		t = string("hMAllRPD")+to_string(i);
		hMAllRPD[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);

		t = string("hPIncRPD")+to_string(i);
		hPIncRPD[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
		t = string("hMIncRPD")+to_string(i);
		hMIncRPD[i] = new TH1D( t.c_str(), t.c_str(), 1000, 0, 100000);
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
		//if ( (!pprimaryVertexFilter) or (!phfCoincFilter2Th4) or (!pclusterCompatibilityFilter) ) {
		//	idx++;
		//	continue;
		//}
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


		hNpeakPAll->Fill(P_NpeakT);
		hNpeakMAll->Fill(M_NpeakT);
		hNpeakPMAll->Fill(P_NpeakT, M_NpeakT);

		if ( P_select ) hNpeakP->Fill(P_NpeakT);
		if ( M_select ) hNpeakM->Fill(M_NpeakT);

		if ( P_select and M_select ) {
			hNpeakPM->Fill(P_NpeakT, M_NpeakT);
		}

		if ( P_select and bP1nT ) {
			for ( int i = 0; i < 5; i++ ) {
				hP1NEM[i]->Fill(P_EM[i+1] * calib_PEM[i+1]);
			}
			for ( int i = 0; i < 4; i++ ) {
				hP1NHAD[i]->Fill(P_HAD[i+1]);
			}
			for ( int i = 0; i < 16; i++ ) {
				hP1NRPD[i]->Fill(P_RPD[i]);
			}
		}
		if ( P_select and bP2nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hP2NHAD[i]->Fill(P_HAD[i+1]);
			}
		}
		if ( P_select and bP3nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hP3NHAD[i]->Fill(P_HAD[i+1]);
			}
		}
		if ( bP1nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hP1NHADInc[i]->Fill(P_HAD[i+1]);
			}
		}
		if ( bP2nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hP2NHADInc[i]->Fill(P_HAD[i+1]);
			}
		}
		if ( bP3nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hP3NHADInc[i]->Fill(P_HAD[i+1]);
			}
		}

		if ( P_select ) {
			for ( int i = 0; i < 5; i++ ) {
				hPAllEM[i]->Fill(P_EM[i+1] * calib_PEM[i+1]);
			}
			for ( int i = 0; i < 4; i++ ) {
				hPAllHAD[i]->Fill(P_HAD[i+1]);
			}
			for ( int i = 0; i < 16; i++ ) {
				hPAllRPD[i]->Fill(P_RPD[i]);
			}
		}
		for ( int i = 0; i < 5; i++ ) {
			hPIncEM[i]->Fill(P_EM[i+1] * calib_PEM[i+1]);
		}
		for ( int i = 0; i < 4; i++ ) {
			hPIncHAD[i]->Fill(P_HAD[i+1]);
		}
		for ( int i = 0; i < 16; i++ ) {
			hPIncRPD[i]->Fill(P_RPD[i]);
		}

		if ( M_select and bM1nT ) {
			for ( int i = 0; i < 5; i++ ) {
				hM1NEM[i]->Fill(M_EM[i+1] * calib_MEM[i+1]);
			}
			for ( int i = 0; i < 4; i++ ) {
				hM1NHAD[i]->Fill(M_HAD[i+1]);
			}
			for ( int i = 0; i < 16; i++ ) {
				hM1NRPD[i]->Fill(M_RPD[i]);
			}
		}
		if ( M_select and bM2nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hM2NHAD[i]->Fill(M_HAD[i+1]);
			}
		}
		if ( M_select and bM3nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hM3NHAD[i]->Fill(M_HAD[i+1]);
			}
		}
		if ( bM1nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hM1NHADInc[i]->Fill(M_HAD[i+1]);
			}
		}
		if ( bM2nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hM2NHADInc[i]->Fill(M_HAD[i+1]);
			}
		}
		if ( bM3nT ) {
			for ( int i = 0; i < 4; i++ ) {
				hM3NHADInc[i]->Fill(M_HAD[i+1]);
			}
		}

		if ( M_select ) {
			for ( int i = 0; i < 5; i++ ) {
				hMAllEM[i]->Fill(M_EM[i+1] * calib_MEM[i+1]);
			}
			for ( int i = 0; i < 4; i++ ) {
				hMAllHAD[i]->Fill(M_HAD[i+1]);
			}
			for ( int i = 0; i < 16; i++ ) {
				hMAllRPD[i]->Fill(M_RPD[i]);
			}
		}
		for ( int i = 0; i < 5; i++ ) {
			hMIncEM[i]->Fill(M_EM[i+1] * calib_MEM[i+1]);
		}
		for ( int i = 0; i < 4; i++ ) {
			hMIncHAD[i]->Fill(M_HAD[i+1]);
		}
		for ( int i = 0; i < 16; i++ ) {
			hMIncRPD[i]->Fill(M_RPD[i]);
		}
		idx++;
	}


	TFile * fsave = new TFile(s.c_str(), "recreate");
	hNpeakP->Write();
	hNpeakM->Write();
	hNpeakPM->Write();
	hNpeakPAll->Write();
	hNpeakMAll->Write();
	hNpeakPMAll->Write();
	auto fEM = fsave->mkdir("EM");
	fEM->cd();
	for ( int i = 0; i < 5; i++ ) {
		hP1NEM[i] ->Write();
		hM1NEM[i] ->Write();
		hPAllEM[i]->Write();
		hMAllEM[i]->Write();
		hPIncEM[i]->Write();
		hMIncEM[i]->Write();
	}
	auto fHad = fsave->mkdir("HAD");
	fHad->cd();
	for ( int i = 0; i < 4; i++ ) {
		hP1NHAD[i] ->Write();
		hM1NHAD[i] ->Write();
		hP2NHAD[i] ->Write();
		hM2NHAD[i] ->Write();
		hP3NHAD[i] ->Write();
		hM3NHAD[i] ->Write();

		hP1NHADInc[i] ->Write();
		hM1NHADInc[i] ->Write();
		hP2NHADInc[i] ->Write();
		hM2NHADInc[i] ->Write();
		hP3NHADInc[i] ->Write();
		hM3NHADInc[i] ->Write();

		hPAllHAD[i]->Write();
		hMAllHAD[i]->Write();
		hPIncHAD[i]->Write();
		hMIncHAD[i]->Write();
	}
	auto fRPD = fsave->mkdir("RPD");
	fRPD->cd();
	for ( int i = 0; i < 16; i++ ) {
		hP1NRPD[i] ->Write();
		hM1NRPD[i] ->Write();
		hPAllRPD[i]->Write();
		hMAllRPD[i]->Write();
		hPIncRPD[i]->Write();
		hMIncRPD[i]->Write();
	}
}


void loadTree(TChain* trV, string input)
{
	trV->Add( (string("ZDC2018/") + input + "/*.root/zdcana/fC/trV").c_str() );

//	trV->SetMakeClass(1);
}

int peakTS(double* ch)
{
	return std::distance(ch, std::max_element(ch, ch+10));
}
