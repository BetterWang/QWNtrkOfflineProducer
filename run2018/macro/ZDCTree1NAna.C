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

double P2nT1 = 11000.;
double P2nT2 = 18000.;
double M2nT1 = 17000.;
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
ZDCTree1NAna(string input = "", string s = "test.root")
{
	TChain * trV = new TChain("trV");
	TChain * trC = new TChain("trC");
//	loadTree(trV, trC, input);

	trV->Add( (input + "/zdcana/fC/trV").c_str() );
	trC->Add( (input + "/QWHFTree/trV").c_str() );
	trV->AddFriend(trC);

	Double_t centBin;
	trC->SetBranchAddress("_dbCentBin", &centBin);

	TH1D* hCentBinP = new TH1D("hCentBinP", "hCentBinP", 200, 0, 200);
	TH1D* hCentBinM = new TH1D("hCentBinM", "hCentBinM", 200, 0, 200);

	TH1D* hEM1toHad1P = new TH1D("hEM1toHad1P", "hEM1toHad1P", 1000, 0, 1);
	TH1D* hEM2toHad1P = new TH1D("hEM2toHad1P", "hEM2toHad1P", 1000, 0, 1);
	TH1D* hEM3toHad1P = new TH1D("hEM3toHad1P", "hEM3toHad1P", 1000, 0, 1);
	TH1D* hEM4toHad1P = new TH1D("hEM4toHad1P", "hEM4toHad1P", 1000, 0, 1);
	TH1D* hEM5toHad1P = new TH1D("hEM5toHad1P", "hEM5toHad1P", 1000, 0, 1);

	TH1D* hEM1toHad1M = new TH1D("hEM1toHad1M", "hEM1toHad1M", 1000, 0, 1);
	TH1D* hEM2toHad1M = new TH1D("hEM2toHad1M", "hEM2toHad1M", 1000, 0, 1);
	TH1D* hEM3toHad1M = new TH1D("hEM3toHad1M", "hEM3toHad1M", 1000, 0, 1);
	TH1D* hEM4toHad1M = new TH1D("hEM4toHad1M", "hEM4toHad1M", 1000, 0, 1);
	TH1D* hEM5toHad1M = new TH1D("hEM5toHad1M", "hEM5toHad1M", 1000, 0, 1);

	TH1D* hEM1toHadAP = new TH1D("hEM1toHadAP", "hEM1toHadAP", 1000, 0, 1);
	TH1D* hEM2toHadAP = new TH1D("hEM2toHadAP", "hEM2toHadAP", 1000, 0, 1);
	TH1D* hEM3toHadAP = new TH1D("hEM3toHadAP", "hEM3toHadAP", 1000, 0, 1);
	TH1D* hEM4toHadAP = new TH1D("hEM4toHadAP", "hEM4toHadAP", 1000, 0, 1);
	TH1D* hEM5toHadAP = new TH1D("hEM5toHadAP", "hEM5toHadAP", 1000, 0, 1);

	TH1D* hEM1toHadAM = new TH1D("hEM1toHadAM", "hEM1toHadAM", 1000, 0, 1);
	TH1D* hEM2toHadAM = new TH1D("hEM2toHadAM", "hEM2toHadAM", 1000, 0, 1);
	TH1D* hEM3toHadAM = new TH1D("hEM3toHadAM", "hEM3toHadAM", 1000, 0, 1);
	TH1D* hEM4toHadAM = new TH1D("hEM4toHadAM", "hEM4toHadAM", 1000, 0, 1);
	TH1D* hEM5toHadAM = new TH1D("hEM5toHadAM", "hEM5toHadAM", 1000, 0, 1);

	TH1D* hEM1_1NP = new TH1D("hEM1_1NP", "hEM1_1NP", 10000, 0, 50000);
	TH1D* hEM2_1NP = new TH1D("hEM2_1NP", "hEM2_1NP", 10000, 0, 50000);
	TH1D* hEM3_1NP = new TH1D("hEM3_1NP", "hEM3_1NP", 10000, 0, 50000);
	TH1D* hEM4_1NP = new TH1D("hEM4_1NP", "hEM4_1NP", 10000, 0, 50000);
	TH1D* hEM5_1NP = new TH1D("hEM5_1NP", "hEM5_1NP", 10000, 0, 50000);

	TH1D* hEM1_1NM = new TH1D("hEM1_1NM", "hEM1_1NM", 10000, 0, 50000);
	TH1D* hEM2_1NM = new TH1D("hEM2_1NM", "hEM2_1NM", 10000, 0, 50000);
	TH1D* hEM3_1NM = new TH1D("hEM3_1NM", "hEM3_1NM", 10000, 0, 50000);
	TH1D* hEM4_1NM = new TH1D("hEM4_1NM", "hEM4_1NM", 10000, 0, 50000);
	TH1D* hEM5_1NM = new TH1D("hEM5_1NM", "hEM5_1NM", 10000, 0, 50000);

	TH1D* hEM1NCount200P = new TH1D("hEM1NCount200P", "hEM1NCount200P", 6, 0, 6);
	TH1D* hEM1NCount200M = new TH1D("hEM1NCount200M", "hEM1NCount200M", 6, 0, 6);
	TH1D* hEM1NCount1000P = new TH1D("hEM1NCount1000P", "hEM1NCount1000P", 6, 0, 6);
	TH1D* hEM1NCount1000M = new TH1D("hEM1NCount1000M", "hEM1NCount1000M", 6, 0, 6);
	TH1D* hEM1NCount2000P = new TH1D("hEM1NCount2000P", "hEM1NCount2000P", 6, 0, 6);
	TH1D* hEM1NCount2000M = new TH1D("hEM1NCount2000M", "hEM1NCount2000M", 6, 0, 6);

	TH1D* hRPDP[16];
	TH1D* hRPDM[16];
	TH1D* hRPDP456[16];
	TH1D* hRPDM456[16];
	TH1D* hRPDP4[16];
	TH1D* hRPDM4[16];

	TH1D* hRPDtoZDCP[16];
	TH1D* hRPDtoZDCM[16];
	for ( int i = 0; i < 16; i++ ) {
		hRPDP[i] = new TH1D(Form("hRPDP_%i", i), Form("hRPDP_%i", i), 10000, 0, 50000);
		hRPDM[i] = new TH1D(Form("hRPDM_%i", i), Form("hRPDM_%i", i), 10000, 0, 50000);
		hRPDP456[i] = new TH1D(Form("hRPDP456_%i", i), Form("hRPDP456_%i", i), 10000, 0, 50000);
		hRPDM456[i] = new TH1D(Form("hRPDM456_%i", i), Form("hRPDM456_%i", i), 10000, 0, 50000);
		hRPDP4[i] = new TH1D(Form("hRPDP4_%i", i), Form("hRPDP4_%i", i), 10000, 0, 50000);
		hRPDM4[i] = new TH1D(Form("hRPDM4_%i", i), Form("hRPDM4_%i", i), 10000, 0, 50000);

		hRPDtoZDCP[i] = new TH1D(Form("hRPDtoZDCP_%i", i), Form("hRPDtoZDCP_%i", i), 1000, 0, 1);
		hRPDtoZDCM[i] = new TH1D(Form("hRPDtoZDCM_%i", i), Form("hRPDtoZDCM_%i", i), 1000, 0, 1);
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
		if ( centBin < 100 ) {
			idx++;
			continue;
		}

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
		double P_RPD456[16], M_RPD456[16];
		for ( int i = 0; i < 16; i++ ) {
			P_RPD[i] = lZDCP_RPD[i][4] + lZDCP_RPD[i][5];
			M_RPD[i] = lZDCM_RPD[i][4] + lZDCM_RPD[i][5];
			P_RPD456[i] = lZDCP_RPD[i][4] + lZDCP_RPD[i][5] + lZDCP_RPD[i][6];
			M_RPD456[i] = lZDCM_RPD[i][4] + lZDCM_RPD[i][5] + lZDCM_RPD[i][6];
		}


		bool P_select = ((peakTS(hZDCP_HAD1)==4) and (peakTS(hZDCP_HAD2)==4));
		bool M_select = ((peakTS(hZDCM_HAD1)==4) and (peakTS(hZDCP_HAD2)==4));

		double P_NpeakT = nPHAD1*P_HAD[1] + nPHAD2*P_HAD[2] + nPHAD3*P_HAD[3] + nPHAD4*P_HAD[4] + nPEM1*P_EM[1] + nPEM2*P_EM[2] + nPEM3*P_EM[3] + nPEM4*P_EM[4] + nPEM5*P_EM[5];
		double M_NpeakT = nMHAD1*M_HAD[1] + nMHAD2*M_HAD[2] + nMHAD3*M_HAD[3] + nMHAD4*M_HAD[4] + nMEM1*M_EM[1] + nMEM2*M_EM[2] + nMEM3*M_EM[3] + nMEM4*M_EM[4] + nMEM5*M_EM[5];

		double P_NpeakTHad = nPHAD1*P_HAD[1] + nPHAD2*P_HAD[2] + nPHAD3*P_HAD[3] + nPHAD4*P_HAD[4];
		double M_NpeakTHad = nMHAD1*M_HAD[1] + nMHAD2*M_HAD[2] + nMHAD3*M_HAD[3] + nMHAD4*M_HAD[4];

		bool bP1nT = (P_NpeakT > P1nT1) and (P_NpeakT < P1nT2);
		bool bM1nT = (M_NpeakT > M1nT1) and (M_NpeakT < M1nT2);
		bool bP2nT = (P_NpeakT > P2nT1) and (P_NpeakT < P2nT2);
		bool bM2nT = (M_NpeakT > M2nT1) and (M_NpeakT < M2nT2);
		bool bP3nT = (P_NpeakT > P3nT1) and (P_NpeakT < P3nT2);
		bool bM3nT = (M_NpeakT > M3nT1) and (M_NpeakT < M3nT2);

		if ( P_select and bP1nT ) {
			hCentBinP->Fill(centBin);

			if ( P_EM[1]>=P_EM[2] and P_EM[1]>=P_EM[3] and P_EM[1]>=P_EM[4] and P_EM[1]>P_EM[5]) hEM1toHad1P->Fill(P_EM[1] / P_HAD[1]);
			if ( P_EM[2]>=P_EM[1] and P_EM[2]>=P_EM[3] and P_EM[2]>=P_EM[4] and P_EM[2]>P_EM[5]) hEM2toHad1P->Fill(P_EM[2] / P_HAD[1]);
			if ( P_EM[3]>=P_EM[1] and P_EM[3]>=P_EM[2] and P_EM[3]>=P_EM[4] and P_EM[3]>P_EM[5]) hEM3toHad1P->Fill(P_EM[3] / P_HAD[1]);
			if ( P_EM[4]>=P_EM[1] and P_EM[4]>=P_EM[2] and P_EM[4]>=P_EM[3] and P_EM[4]>P_EM[5]) hEM4toHad1P->Fill(P_EM[4] / P_HAD[1]);
			if ( P_EM[5]>=P_EM[1] and P_EM[5]>=P_EM[2] and P_EM[5]>=P_EM[3] and P_EM[5]>P_EM[4]) hEM5toHad1P->Fill(P_EM[5] / P_HAD[1]);

			if ( P_EM[1]>=P_EM[2] and P_EM[1]>=P_EM[3] and P_EM[1]>=P_EM[4] and P_EM[1]>P_EM[5]) hEM1toHadAP->Fill(P_EM[1] / P_NpeakTHad);
			if ( P_EM[2]>=P_EM[1] and P_EM[2]>=P_EM[3] and P_EM[2]>=P_EM[4] and P_EM[2]>P_EM[5]) hEM2toHadAP->Fill(P_EM[2] / P_NpeakTHad);
			if ( P_EM[3]>=P_EM[1] and P_EM[3]>=P_EM[2] and P_EM[3]>=P_EM[4] and P_EM[3]>P_EM[5]) hEM3toHadAP->Fill(P_EM[3] / P_NpeakTHad);
			if ( P_EM[4]>=P_EM[1] and P_EM[4]>=P_EM[2] and P_EM[4]>=P_EM[3] and P_EM[4]>P_EM[5]) hEM4toHadAP->Fill(P_EM[4] / P_NpeakTHad);
			if ( P_EM[5]>=P_EM[1] and P_EM[5]>=P_EM[2] and P_EM[5]>=P_EM[3] and P_EM[5]>P_EM[4]) hEM5toHadAP->Fill(P_EM[5] / P_NpeakTHad);

			for ( int i = 0; i < 16; i++ ) {
				hRPDP[i]->Fill(P_RPD[i]);
				hRPDP456[i]->Fill(P_RPD456[i]);
				hRPDP4[i]->Fill(lZDCP_RPD[i][4]);
				hRPDtoZDCP[i]->Fill( P_RPD[i] / P_NpeakT );
			}

			if ( P_HAD[1] > 2000. ) hEM1_1NP->Fill(P_EM[1]);
			if ( P_HAD[1] > 2000. ) hEM2_1NP->Fill(P_EM[2]);
			if ( P_HAD[1] > 2000. ) hEM3_1NP->Fill(P_EM[3]);
			if ( P_HAD[1] > 2000. ) hEM4_1NP->Fill(P_EM[4]);
			if ( P_HAD[1] > 2000. ) hEM5_1NP->Fill(P_EM[5]);

			hEM1NCount1000P->Fill(
					int( P_EM[1] > 1000 ) +
					int( P_EM[2] > 1000 ) +
					int( P_EM[3] > 1000 ) +
					int( P_EM[4] > 1000 ) +
					int( P_EM[5] > 1000 )
					);
			hEM1NCount200P->Fill(
					int( P_EM[1] > 200 ) +
					int( P_EM[2] > 200 ) +
					int( P_EM[3] > 200 ) +
					int( P_EM[4] > 200 ) +
					int( P_EM[5] > 200 )
					);
			hEM1NCount2000P->Fill(
					int( P_EM[1] > 2000 ) +
					int( P_EM[2] > 2000 ) +
					int( P_EM[3] > 2000 ) +
					int( P_EM[4] > 2000 ) +
					int( P_EM[5] > 2000 )
					);
		}

		if ( M_select and bM1nT ) {
			hCentBinM->Fill(centBin);

			if ( M_EM[1]>=M_EM[2] and M_EM[1]>=M_EM[3] and M_EM[1]>=M_EM[4] and M_EM[1]>M_EM[5]) hEM1toHad1M->Fill(M_EM[1] / M_HAD[1]);
			if ( M_EM[2]>=M_EM[1] and M_EM[2]>=M_EM[3] and M_EM[2]>=M_EM[4] and M_EM[2]>M_EM[5]) hEM2toHad1M->Fill(M_EM[2] / M_HAD[1]);
			if ( M_EM[3]>=M_EM[1] and M_EM[3]>=M_EM[2] and M_EM[3]>=M_EM[4] and M_EM[3]>M_EM[5]) hEM3toHad1M->Fill(M_EM[3] / M_HAD[1]);
			if ( M_EM[4]>=M_EM[1] and M_EM[4]>=M_EM[2] and M_EM[4]>=M_EM[3] and M_EM[4]>M_EM[5]) hEM4toHad1M->Fill(M_EM[4] / M_HAD[1]);
			if ( M_EM[5]>=M_EM[1] and M_EM[5]>=M_EM[2] and M_EM[5]>=M_EM[3] and M_EM[5]>M_EM[4]) hEM5toHad1M->Fill(M_EM[5] / M_HAD[1]);

			if ( M_EM[1]>=M_EM[2] and M_EM[1]>=M_EM[3] and M_EM[1]>=M_EM[4] and M_EM[1]>M_EM[5]) hEM1toHadAM->Fill(M_EM[1] / M_NpeakTHad);
			if ( M_EM[2]>=M_EM[1] and M_EM[2]>=M_EM[3] and M_EM[2]>=M_EM[4] and M_EM[2]>M_EM[5]) hEM2toHadAM->Fill(M_EM[2] / M_NpeakTHad);
			if ( M_EM[3]>=M_EM[1] and M_EM[3]>=M_EM[2] and M_EM[3]>=M_EM[4] and M_EM[3]>M_EM[5]) hEM3toHadAM->Fill(M_EM[3] / M_NpeakTHad);
			if ( M_EM[4]>=M_EM[1] and M_EM[4]>=M_EM[2] and M_EM[4]>=M_EM[3] and M_EM[4]>M_EM[5]) hEM4toHadAM->Fill(M_EM[4] / M_NpeakTHad);
			if ( M_EM[5]>=M_EM[1] and M_EM[5]>=M_EM[2] and M_EM[5]>=M_EM[3] and M_EM[5]>M_EM[4]) hEM5toHadAM->Fill(M_EM[5] / M_NpeakTHad);

			for ( int i = 0; i < 16; i++ ) {
				hRPDM[i]->Fill(M_RPD[i]);
				hRPDM456[i]->Fill(M_RPD456[i]);
				hRPDM4[i]->Fill(lZDCM_RPD[i][4]);
				hRPDtoZDCM[i]->Fill( M_RPD[i] / M_NpeakT );
			}
			if ( M_HAD[1] > 2000. ) hEM1_1NM->Fill(M_EM[1]);
			if ( M_HAD[1] > 2000. ) hEM2_1NM->Fill(M_EM[2]);
			if ( M_HAD[1] > 2000. ) hEM3_1NM->Fill(M_EM[3]);
			if ( M_HAD[1] > 2000. ) hEM4_1NM->Fill(M_EM[4]);
			if ( M_HAD[1] > 2000. ) hEM5_1NM->Fill(M_EM[5]);

			hEM1NCount1000M->Fill(
					int( M_EM[1] > 1000 ) +
					int( M_EM[2] > 1000 ) +
					int( M_EM[3] > 1000 ) +
					int( M_EM[4] > 1000 ) +
					int( M_EM[5] > 1000 )
					);
			hEM1NCount200M->Fill(
					int( M_EM[1] > 200 ) +
					int( M_EM[2] > 200 ) +
					int( M_EM[3] > 200 ) +
					int( M_EM[4] > 200 ) +
					int( M_EM[5] > 200 )
					);
			hEM1NCount2000M->Fill(
					int( M_EM[1] > 2000 ) +
					int( M_EM[2] > 2000 ) +
					int( M_EM[3] > 2000 ) +
					int( M_EM[4] > 2000 ) +
					int( M_EM[5] > 2000 )
					);
		}

		idx++;
	}


	TFile * fsave = new TFile(s.c_str(), "recreate");
	hCentBinP->Write();
	hCentBinM->Write();
	auto fHad1 = fsave->mkdir("Had1");
	fHad1->cd();
	hEM1toHad1P->Write();
	hEM2toHad1P->Write();
	hEM3toHad1P->Write();
	hEM4toHad1P->Write();
	hEM5toHad1P->Write();
	hEM1toHad1M->Write();
	hEM2toHad1M->Write();
	hEM3toHad1M->Write();
	hEM4toHad1M->Write();
	hEM5toHad1M->Write();

	auto fHadA = fsave->mkdir("HadA");
	fHadA->cd();
	hEM1toHadAP->Write();
	hEM2toHadAP->Write();
	hEM3toHadAP->Write();
	hEM4toHadAP->Write();
	hEM5toHadAP->Write();
	hEM1toHadAM->Write();
	hEM2toHadAM->Write();
	hEM3toHadAM->Write();
	hEM4toHadAM->Write();
	hEM5toHadAM->Write();

	auto fRPD = fsave->mkdir("RPD");
	fRPD->cd();
	for ( int i = 0 ; i < 16; i++ ) {
		hRPDP[i]->Write();
		hRPDM[i]->Write();
	}
	auto fRPD4 = fsave->mkdir("RPD4");
	fRPD4->cd();
	for ( int i = 0 ; i < 16; i++ ) {
		hRPDP4[i]->Write();
		hRPDM4[i]->Write();
	}
	auto fRPD456 = fsave->mkdir("RPD456");
	fRPD456->cd();
	for ( int i = 0 ; i < 16; i++ ) {
		hRPDP456[i]->Write();
		hRPDM456[i]->Write();
	}
	auto fRPDtoZDC = fsave->mkdir("RPDtoZDC");
	fRPDtoZDC->cd();
	for ( int i = 0 ; i < 16; i++ ) {
		hRPDtoZDCP[i]->Write();
		hRPDtoZDCM[i]->Write();
	}
	auto fEM1N = fsave->mkdir("EM1N");
	fEM1N->cd();
	hEM1_1NP->Write();
	hEM2_1NP->Write();
	hEM3_1NP->Write();
	hEM4_1NP->Write();
	hEM5_1NP->Write();
	hEM1_1NM->Write();
	hEM2_1NM->Write();
	hEM3_1NM->Write();
	hEM4_1NM->Write();
	hEM5_1NM->Write();

	hEM1NCount200P->Write();
	hEM1NCount200M->Write();
	hEM1NCount1000P->Write();
	hEM1NCount1000M->Write();
	hEM1NCount2000P->Write();
	hEM1NCount2000M->Write();
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
