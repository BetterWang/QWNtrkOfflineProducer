#include <TH1.h>
#include <TProfile.h>
#include <TH2.h>
#include <TTree.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <string>
#include <algorithm>
#include "RPDEP.h"

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
	TChain * trA = new TChain("trA");
//	loadTree(trV, trC, input);

	trV->Add( (input + "/zdcana/fC/trV").c_str() );
	trC->Add( (input + "/QWHFTree/trV").c_str() );
	trA->Add( (input + "/centTree/trV").c_str() );
	trV->AddFriend(trC);
	trV->AddFriend(trA);

	Double_t centBin;
	Double_t argp_HFQ2;
	Double_t argm_HFQ2;
	trC->SetBranchAddress("_dbCentBin", &centBin);
	trC->SetBranchAddress("argp_HFQ2", &argp_HFQ2);
	trC->SetBranchAddress("argm_HFQ2", &argm_HFQ2);

    Double_t etHFtowerSumPlus;
    Double_t etHFtowerSumMinus;
	trA->SetBranchAddress("etHFtowerSumPlus", &etHFtowerSumPlus);
	trA->SetBranchAddress("etHFtowerSumMinus", &etHFtowerSumMinus);

	TH1D* hCentBin = new TH1D("hCentBin", "hCentBin", 200, 0, 200);

	TH2D* hEMtoHF2xP[10];
	TH2D* hEMtoHF2xM[10];
	TH2D* hEMPtoEMM[10];
	for ( int c = 0; c < 10; c++ ) {
		hEMtoHF2xP[c] = new TH2D(Form("hEMtoHF2xP_%i", c), "hEMtoHF2xP;EM_P;HF2EP_P", 100, -4, 4, 100, -3.1416, 3.1416);
		hEMtoHF2xM[c] = new TH2D(Form("hEMtoHF2xM_%i", c), "hEMtoHF2xM;EM_P;HF2EP_M", 100, -4, 4, 100, -3.1416, 3.1416);
		hEMPtoEMM[c]  = new TH2D(Form("hEMPtoEMM_%i", c), "hEMPtoEMM;EM_P;EM_M", 100, -4, 4, 100, -4, 4);
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
			hP1NEM[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 100000);
			t = string("hM1NEM")+to_string(i+1)+"_"+to_string(c);
			hM1NEM[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 100000);

			t = string("hPAllEM")+to_string(i+1)+"_"+to_string(c);
			hPAllEM[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 100000);
			t = string("hMAllEM")+to_string(i+1)+"_"+to_string(c);
			hMAllEM[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 100000);

			t = string("hPIncEM")+to_string(i+1)+"_"+to_string(c);
			hPIncEM[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 100000);
			t = string("hMIncEM")+to_string(i+1)+"_"+to_string(c);
			hMIncEM[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 100000);
		}
	}

    TH1D* hPRawEM[10][5];
    TH1D* hMRawEM[10][5];
    TH1D* hPRawHad[10][4];
    TH1D* hMRawHad[10][4];
    TH1D* hPRawRPD[10][16];
    TH1D* hMRawRPD[10][16];

    TH1D* hTS4PRawEM[10][5];
    TH1D* hTS4MRawEM[10][5];
    TH1D* hTS4PRawHad[10][4];
    TH1D* hTS4MRawHad[10][4];
    TH1D* hTS4PRawRPD[10][16];
    TH1D* hTS4MRawRPD[10][16];
    TH1D* hTS456PRawRPD[10][16];
    TH1D* hTS456MRawRPD[10][16];

    for ( int c = 0; c < 10; c++ ) {
        for ( int i = 0; i < 5; i++ ) {
            t = string("hPRawEM")+to_string(i+1)+"_"+to_string(c);
            hPRawEM[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 50000);
            t = string("hMRawEM")+to_string(i+1)+"_"+to_string(c);
            hMRawEM[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 50000);

            t = string("hTS4PRawEM")+to_string(i+1)+"_"+to_string(c);
            hTS4PRawEM[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 50000);
            t = string("hTS4MRawEM")+to_string(i+1)+"_"+to_string(c);
            hTS4MRawEM[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 50000);
        }
        for ( int i = 0; i < 4; i++ ) {
            t = string("hPRawHad")+to_string(i+1)+"_"+to_string(c);
            hPRawHad[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 50000);
            t = string("hMRawHad")+to_string(i+1)+"_"+to_string(c);
            hMRawHad[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 50000);

            t = string("hTS4PRawHad")+to_string(i+1)+"_"+to_string(c);
            hTS4PRawHad[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 50000);
            t = string("hTS4MRawHad")+to_string(i+1)+"_"+to_string(c);
            hTS4MRawHad[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 50000);
        }
        for ( int i = 0; i < 16; i++ ) {
            t = string("hPRawRPD")+to_string(i)+"_"+to_string(c);
            hPRawRPD[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 50000);
            t = string("hMRawRPD")+to_string(i)+"_"+to_string(c);
            hMRawRPD[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 50000);

            t = string("hTS4PRawRPD")+to_string(i)+"_"+to_string(c);
            hTS4PRawRPD[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 50000);
            t = string("hTS4MRawRPD")+to_string(i)+"_"+to_string(c);
            hTS4MRawRPD[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 50000);

            t = string("hTS456PRawRPD")+to_string(i)+"_"+to_string(c);
            hTS456PRawRPD[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 50000);
            t = string("hTS456MRawRPD")+to_string(i)+"_"+to_string(c);
            hTS456MRawRPD[c][i] = new TH1D( t.c_str(), t.c_str(), 10000, 0, 50000);
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


    // HF correlation
    TH2D * ZDCHFP[10];
    TH2D * ZDCHFM[10];
    TH2D * RPDHFP4[10];
    TH2D * RPDHFM4[10];
    TH2D * RPDHFP45[10];
    TH2D * RPDHFM45[10];
    TH2D * RPDHFP456[10];
    TH2D * RPDHFM456[10];
    for ( int c = 0; c < 10; c++ ) {
        ZDCHFP[c] = new TH2D(Form("ZDCHFP_%i", c), Form("ZDCHFP_%i", c),           300, 0, 3000, 200, 0, 1000000);
        ZDCHFM[c] = new TH2D(Form("ZDCHFM_%i", c), Form("ZDCHFM_%i", c),           300, 0, 3000, 200, 0, 1000000);
        RPDHFP4[c] = new TH2D(Form("RPDHFP4_%i", c), Form("RPDHFP4_%i", c),        300, 0, 3000, 200, 0, 1000000);
        RPDHFM4[c] = new TH2D(Form("RPDHFM4_%i", c), Form("RPDHFM4_%i", c),        300, 0, 3000, 200, 0, 1000000);
        RPDHFP45[c] = new TH2D(Form("RPDHFP45_%i", c), Form("RPDHFP45_%i", c),     300, 0, 3000, 200, 0, 1000000);
        RPDHFM45[c] = new TH2D(Form("RPDHFM45_%i", c), Form("RPDHFM45_%i", c),     300, 0, 3000, 200, 0, 1000000);
        RPDHFP456[c] = new TH2D(Form("RPDHFP456_%i", c), Form("RPDHFP456_%i", c),  300, 0, 3000, 200, 0, 1000000);
        RPDHFM456[c] = new TH2D(Form("RPDHFM456_%i", c), Form("RPDHFM456_%i", c),  300, 0, 3000, 200, 0, 1000000);
    }
    TH2D * ZDCHFAllP = new TH2D("ZDCHFAllP", "ZDCHFAllP",           300, 0, 3000, 200, 0, 1000000 );
    TH2D * ZDCHFAllM = new TH2D("ZDCHFAllM", "ZDCHFAllM",           300, 0, 3000, 200, 0, 1000000 );
    TH2D * RPDHFAllP4 = new TH2D("RPDHFAllP4", "RPDHFAllP4",        300, 0, 3000, 200, 0, 1000000 );
    TH2D * RPDHFAllM4 = new TH2D("RPDHFAllM4", "RPDHFAllM4",        300, 0, 3000, 200, 0, 1000000 );
    TH2D * RPDHFAllP45 = new TH2D("RPDHFAllP45", "RPDHFAllP45",     300, 0, 3000, 200, 0, 1000000 );
    TH2D * RPDHFAllM45 = new TH2D("RPDHFAllM45", "RPDHFAllM45",     300, 0, 3000, 200, 0, 1000000 );
    TH2D * RPDHFAllP456 = new TH2D("RPDHFAllP456", "RPDHFAllP456",  300, 0, 3000, 200, 0, 1000000 );
    TH2D * RPDHFAllM456 = new TH2D("RPDHFAllM456", "RPDHFAllM456",  300, 0, 3000, 200, 0, 1000000 );

    TH2D * RPDiHFP4[16];
    TH2D * RPDiHFP45[16];
    TH2D * RPDiHFP456[16];
    TH2D * RPDiHFM4[16];
    TH2D * RPDiHFM45[16];
    TH2D * RPDiHFM456[16];

    TProfile * PRPD4Prof[16];
    TProfile * PRPD45Prof[16];
    TProfile * PRPD456Prof[16];

    TProfile * PRPD4ProfCent[16];
    TProfile * PRPD45ProfCent[16];
    TProfile * PRPD456ProfCent[16];
    TProfile * PRPD4ProfCentS[16];
    TProfile * PRPD45ProfCentS[16];
    TProfile * PRPD456ProfCentS[16];

    TProfile * MRPD4Prof[16];
    TProfile * MRPD45Prof[16];
    TProfile * MRPD456Prof[16];

    TProfile * MRPD4ProfCent[16];
    TProfile * MRPD45ProfCent[16];
    TProfile * MRPD456ProfCent[16];
    TProfile * MRPD4ProfCentS[16];
    TProfile * MRPD45ProfCentS[16];
    TProfile * MRPD456ProfCentS[16];

    for ( int i = 0; i < 16; i++ ) {
        RPDiHFP4[i]   = new TH2D(Form("RPDiHFP4_%i", i),   Form("RPDiHFP4_%i", i),   300, 0, 3000, 200, 0, 100000 );
        RPDiHFP45[i]  = new TH2D(Form("RPDiHFP45_%i", i),  Form("RPDiHFP45_%i", i),  300, 0, 3000, 200, 0, 100000 );
        RPDiHFP456[i] = new TH2D(Form("RPDiHFP456_%i", i), Form("RPDiHFP456_%i", i), 300, 0, 3000, 200, 0, 100000 );
        RPDiHFM4[i]   = new TH2D(Form("RPDiHFM4_%i", i),   Form("RPDiHFM4_%i", i),   300, 0, 3000, 200, 0, 100000 );
        RPDiHFM45[i]  = new TH2D(Form("RPDiHFM45_%i", i),  Form("RPDiHFM45_%i", i),  300, 0, 3000, 200, 0, 100000 );
        RPDiHFM456[i] = new TH2D(Form("RPDiHFM456_%i", i), Form("RPDiHFM456_%i", i), 300, 0, 3000, 200, 0, 100000 );

        PRPD4Prof[i]       = new TProfile(Form("PRPD4Prof_%i", i),       Form("PRPD4Prof_%i", i),       300, 0, 3000);
        PRPD45Prof[i]      = new TProfile(Form("PRPD45Prof_%i", i),      Form("PRPD45Prof_%i", i),      300, 0, 3000);
        PRPD456Prof[i]     = new TProfile(Form("PRPD456Prof_%i", i),     Form("PRPD456Prof_%i", i),     300, 0, 3000);

        PRPD4ProfCent[i]   = new TProfile(Form("PRPD4ProfCent_%i", i),   Form("PRPD4ProfCent_%i", i),   200, 0, 200);
        PRPD45ProfCent[i]  = new TProfile(Form("PRPD45ProfCent_%i", i),  Form("PRPD45ProfCent_%i", i),  200, 0, 200);
        PRPD456ProfCent[i] = new TProfile(Form("PRPD456ProfCent_%i", i), Form("PRPD456ProfCent_%i", i), 200, 0, 200);

        PRPD4ProfCentS[i]   = new TProfile(Form("PRPD4ProfCentS_%i", i),   Form("PRPD4ProfCentS_%i", i),   200, 0, 200, 200, 10000000);
        PRPD45ProfCentS[i]  = new TProfile(Form("PRPD45ProfCentS_%i", i),  Form("PRPD45ProfCentS_%i", i),  200, 0, 200, 200, 10000000);
        PRPD456ProfCentS[i] = new TProfile(Form("PRPD456ProfCentS_%i", i), Form("PRPD456ProfCentS_%i", i), 200, 0, 200, 200, 10000000);

        MRPD4Prof[i]       = new TProfile(Form("MRPD4Prof_%i", i),       Form("MRPD4Prof_%i", i),       300, 0, 3000);
        MRPD45Prof[i]      = new TProfile(Form("MRPD45Prof_%i", i),      Form("MRPD45Prof_%i", i),      300, 0, 3000);
        MRPD456Prof[i]     = new TProfile(Form("MRPD456Prof_%i", i),     Form("MRPD456Prof_%i", i),     300, 0, 3000);

        MRPD4ProfCent[i]   = new TProfile(Form("MRPD4ProfCent_%i", i),   Form("MRPD4ProfCent_%i", i),   200, 0, 200);
        MRPD45ProfCent[i]  = new TProfile(Form("MRPD45ProfCent_%i", i),  Form("MRPD45ProfCent_%i", i),  200, 0, 200);
        MRPD456ProfCent[i] = new TProfile(Form("MRPD456ProfCent_%i", i), Form("MRPD456ProfCent_%i", i), 200, 0, 200);

        MRPD4ProfCentS[i]   = new TProfile(Form("MRPD4ProfCentS_%i", i),   Form("MRPD4ProfCentS_%i", i),   200, 0, 200, 200, 10000000);
        MRPD45ProfCentS[i]  = new TProfile(Form("MRPD45ProfCentS_%i", i),  Form("MRPD45ProfCentS_%i", i),  200, 0, 200, 200, 10000000);
        MRPD456ProfCentS[i] = new TProfile(Form("MRPD456ProfCentS_%i", i), Form("MRPD456ProfCentS_%i", i), 200, 0, 200, 200, 10000000);
    }

    TH2D * hRPDvsRPDLowP[16][16][10] = {};
    TH2D * hRPDvsRPDLowM[16][16][10] = {};
    TH2D * hRPDvsRPDHighP[16][16][10] = {};
    TH2D * hRPDvsRPDHighM[16][16][10] = {};
    for ( int c = 0; c < 10; c++ ) {
        for ( int i = 0; i < 16; i++ ) {
            for ( int j = i+1; j < 16; j++ ) {
                hRPDvsRPDLowP[j][i][c] = new TH2D(Form("hRPDvsRPDLowP_%i_%i_%i", j, i, c), Form(";PRPD%i;PRPD%i", j, i), 200, 0, 2000, 200, 0, 2000);
                hRPDvsRPDLowM[j][i][c] = new TH2D(Form("hRPDvsRPDLowM_%i_%i_%i", j, i, c), Form(";MRPD%i;MRPD%i", j, i), 200, 0, 2000, 200, 0, 2000);

                hRPDvsRPDHighP[j][i][c] = new TH2D(Form("hRPDvsRPDHighP_%i_%i_%i", j, i, c), Form(";PRPD%i;PRPD%i", j, i), 200, 0, 100000, 200, 0, 100000);
                hRPDvsRPDHighM[j][i][c] = new TH2D(Form("hRPDvsRPDHighM_%i_%i_%i", j, i, c), Form(";MRPD%i;MRPD%i", j, i), 200, 0, 100000, 200, 0, 100000);
            }
        }
    }

    TH2D * hRPDvsRPD1NP[16][16] = {};
    TH2D * hRPDvsRPD1NM[16][16] = {};
    for ( int i = 0; i < 16; i++ ) {
        for ( int j = i+1; j < 16; j++ ) {
                hRPDvsRPD1NP[j][i] = new TH2D(Form("hRPDvsRPD1NP_%i_%i", j, i), Form(";PRPD%i;PRPD%i", j, i), 200, 0, 20000, 200, 0, 20000);
                hRPDvsRPD1NM[j][i] = new TH2D(Form("hRPDvsRPD1NM_%i_%i", j, i), Form(";PRPD%i;PRPD%i", j, i), 200, 0, 20000, 200, 0, 20000);
        }
    }

    TH2D * hRPDEP1vsHFEP2P[10];
    TH2D * hRPDEP1vsHFEP2M[10];
    TH2D * hRPDEP[10];
    for ( int c = 0; c < 10; c++ ) {
        hRPDEP1vsHFEP2P[c] = new TH2D(Form("hRPDEP1vsHFEP2P_%i",c), Form("hRPDEP1vsHFEP2P_%i;RPDEP1P;HFEP2P", c), 100, -3.1416, 3.1416, 100, -3.1416, 3.1416 );
        hRPDEP1vsHFEP2M[c] = new TH2D(Form("hRPDEP1vsHFEP2M_%i",c), Form("hRPDEP1vsHFEP2M_%i;RPDEP1M;HFEP2M", c), 100, -3.1416, 3.1416, 100, -3.1416, 3.1416 );
        hRPDEP[c] = new TH2D(Form("hRPDEP_%i",c), Form("hRPDEP_%i;RPDEP1M;RPDEP1P", c), 100, -3.1416, 3.1416, 100, -3.1416, 3.1416 );
    }

    TH2D * hRPDvsEMP[5][16];
    TH2D * hRPDvsEMM[5][16];
    for ( int i = 0; i < 16; i++ ) {
        for ( int j = 0; j < 5; j++ ) {
            hRPDvsEMP[j][i] = new TH2D(Form("hRPDvsEMP_%i_%i", j, i), Form("hRPDvsEMP;EMP%i;RPDP%i", j+1, i), 200, 0, 200000, 200, 0, 100000);
            hRPDvsEMM[j][i] = new TH2D(Form("hRPDvsEMM_%i_%i", j, i), Form("hRPDvsEMM;EMM%i;RPDM%i", j+1, i), 200, 0, 200000, 200, 0, 100000);
        }
    }

    TH2D * hRPD2x2PXY[10];
    TH2D * hRPD2x2MXY[10];

    TH2D * hRPD2x2PMX[10];
    TH2D * hRPD2x2PMY[10];

    TH1D * hRPD2x2PEP[10];
    TH1D * hRPD2x2MEP[10];

    TH1D * hHF2EPP[10];
    TH1D * hHF2EPM[10];
    for ( int c = 0; c < 10; c++ ) {
        hRPD2x2PXY[c] = new TH2D( Form("hRPD2x2PXY_%i", c), Form(";RPD2x2PX_%i;RPD2x2PY_%i", c, c), 100, -3., 3., 100, -3, 3.);
        hRPD2x2MXY[c] = new TH2D( Form("hRPD2x2MXY_%i", c), Form(";RPD2x2MX_%i;RPD2x2MY_%i", c, c), 100, -3., 3., 100, -3, 3.);

        hRPD2x2PMX[c] = new TH2D( Form("hRPD2x2PMX_%i", c), Form(";RPD2x2PX_%i;RPD2x2MX_%i", c, c), 100, -3., 3., 100, -3, 3.);
        hRPD2x2PMY[c] = new TH2D( Form("hRPD2x2PMY_%i", c), Form(";RPD2x2PY_%i;RPD2x2MY_%i", c, c), 100, -3., 3., 100, -3, 3.);

        hRPD2x2PEP[c] = new TH1D( Form("hRPD2x2PEP_%i", c), Form(";RPD2x2PEP_%i;", c), 100, -3.1416, 3.1416);
        hRPD2x2MEP[c] = new TH1D( Form("hRPD2x2MEP_%i", c), Form(";RPD2x2MEP_%i;", c), 100, -3.1416, 3.1416);

        hHF2EPP[c] = new TH1D( Form("hHF2EPP_%i", c), Form(";HF2EPP_%i;", c), 100, -3.1416, 3.1416);
        hHF2EPM[c] = new TH1D( Form("hHF2EPM_%i", c), Form(";HF2EPM_%i;", c), 100, -3.1416, 3.1416);

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
		//cout << " --> idx = " << idx << endl;
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
		double P_RPDTS4[16], M_RPDTS4[16];
		double P_RPDTS456[16], M_RPDTS456[16];
		for ( int i = 0; i < 16; i++ ) {
			P_RPD[i] = lZDCP_RPD[i][4] + lZDCP_RPD[i][5];
			M_RPD[i] = lZDCM_RPD[i][4] + lZDCM_RPD[i][5];
			P_RPDTS4[i] = lZDCP_RPD[i][4];
			M_RPDTS4[i] = lZDCM_RPD[i][4];
			P_RPDTS456[i] = lZDCP_RPD[i][4] + lZDCP_RPD[i][5] + lZDCP_RPD[i][6];
			M_RPDTS456[i] = lZDCM_RPD[i][4] + lZDCM_RPD[i][5] + lZDCP_RPD[i][6];
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

        double P_RPD4 = 0;
        double M_RPD4 = 0;
        double P_RPD45 = 0;
        double M_RPD45 = 0;
        double P_RPD456 = 0;
        double M_RPD456 = 0;
        for ( int i = 0; i < 16; i++ ) {
            P_RPD4 += P_RPDTS4[i];
            M_RPD4 += M_RPDTS4[i];
            P_RPD45 += P_RPD[i];
            M_RPD45 += M_RPD[i];
            P_RPD456 += P_RPDTS456[i];
            M_RPD456 += M_RPDTS456[i];
        }


		hNpeakPAll[c]->Fill(P_NpeakT);
		hNpeakMAll[c]->Fill(M_NpeakT);
		hNpeakPMAll[c]->Fill(P_NpeakT, M_NpeakT);

		if ( P_select ) hNpeakP[c]->Fill(P_NpeakT);
		if ( M_select ) hNpeakM[c]->Fill(M_NpeakT);

        hPRawEM[c][0]->Fill(P_EM[1]);
        hPRawEM[c][1]->Fill(P_EM[2]);
        hPRawEM[c][2]->Fill(P_EM[3]);
        hPRawEM[c][3]->Fill(P_EM[4]);
        hPRawEM[c][4]->Fill(P_EM[5]);
        hMRawEM[c][0]->Fill(M_EM[1]);
        hMRawEM[c][1]->Fill(M_EM[2]);
        hMRawEM[c][2]->Fill(M_EM[3]);
        hMRawEM[c][3]->Fill(M_EM[4]);
        hMRawEM[c][4]->Fill(M_EM[5]);

        hPRawHad[c][0]->Fill(P_HAD[1]);
        hPRawHad[c][1]->Fill(P_HAD[2]);
        hPRawHad[c][2]->Fill(P_HAD[3]);
        hPRawHad[c][3]->Fill(P_HAD[4]);
        hMRawHad[c][0]->Fill(M_HAD[1]);
        hMRawHad[c][1]->Fill(M_HAD[2]);
        hMRawHad[c][2]->Fill(M_HAD[3]);
        hMRawHad[c][3]->Fill(M_HAD[4]);

        hTS4PRawEM[c][0]->Fill(hZDCP_EM1[4]);
        hTS4PRawEM[c][1]->Fill(hZDCP_EM2[4]);
        hTS4PRawEM[c][2]->Fill(hZDCP_EM3[4]);
        hTS4PRawEM[c][3]->Fill(hZDCP_EM4[4]);
        hTS4PRawEM[c][4]->Fill(hZDCP_EM5[4]);
        hTS4MRawEM[c][0]->Fill(hZDCM_EM1[4]);
        hTS4MRawEM[c][1]->Fill(hZDCM_EM2[4]);
        hTS4MRawEM[c][2]->Fill(hZDCM_EM3[4]);
        hTS4MRawEM[c][3]->Fill(hZDCM_EM4[4]);
        hTS4MRawEM[c][4]->Fill(hZDCM_EM5[4]);

        hTS4PRawHad[c][0]->Fill(hZDCP_HAD1[4]);
        hTS4PRawHad[c][1]->Fill(hZDCP_HAD2[4]);
        hTS4PRawHad[c][2]->Fill(hZDCP_HAD3[4]);
        hTS4PRawHad[c][3]->Fill(hZDCP_HAD4[4]);
        hTS4MRawHad[c][0]->Fill(hZDCM_HAD1[4]);
        hTS4MRawHad[c][1]->Fill(hZDCM_HAD2[4]);
        hTS4MRawHad[c][2]->Fill(hZDCM_HAD3[4]);
        hTS4MRawHad[c][3]->Fill(hZDCM_HAD4[4]);

        for ( int i = 0; i < 16; i++ ) {
            hPRawRPD[c][i]->Fill(P_RPD[i]);
            hMRawRPD[c][i]->Fill(M_RPD[i]);
            hTS4PRawRPD[c][i]->Fill(P_RPDTS4[i]);
            hTS4MRawRPD[c][i]->Fill(M_RPDTS4[i]);
            hTS456PRawRPD[c][i]->Fill(P_RPDTS456[i]);
            hTS456MRawRPD[c][i]->Fill(M_RPDTS456[i]);
        }

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
            if ( centBin < 100 ) {
                for ( int i = 0; i < 16; i++ ) {
                    for ( int j = i+1; j < 16; j++ ) {
                        hRPDvsRPD1NP[j][i]->Fill(P_RPD[j], P_RPD[i]);
                    }
                }
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
            if ( centBin < 100 ) {
                for ( int i = 0; i < 16; i++ ) {
                    for ( int j = i+1; j < 16; j++ ) {
                        hRPDvsRPD1NM[j][i]->Fill(M_RPD[j], M_RPD[i]);
                    }
                }
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

        ZDCHFP[c]   ->Fill( etHFtowerSumPlus , P_NpeakT );
        ZDCHFM[c]   ->Fill( etHFtowerSumMinus, M_NpeakT );
        RPDHFP4[c]  ->Fill( etHFtowerSumPlus , P_RPD4   );
        RPDHFM4[c]  ->Fill( etHFtowerSumMinus, M_RPD4   );
        RPDHFP45[c] ->Fill( etHFtowerSumPlus , P_RPD45  );
        RPDHFM45[c] ->Fill( etHFtowerSumMinus, M_RPD45  );
        RPDHFP456[c]->Fill( etHFtowerSumPlus , P_RPD456 );
        RPDHFM456[c]->Fill( etHFtowerSumMinus, M_RPD456 );

        ZDCHFAllP   ->Fill( etHFtowerSumPlus , P_NpeakT );
        ZDCHFAllM   ->Fill( etHFtowerSumMinus, M_NpeakT );
        RPDHFAllP4  ->Fill( etHFtowerSumPlus , P_RPD4   );
        RPDHFAllM4  ->Fill( etHFtowerSumMinus, M_RPD4   );
        RPDHFAllP45 ->Fill( etHFtowerSumPlus , P_RPD45  );
        RPDHFAllM45 ->Fill( etHFtowerSumMinus, M_RPD45  );
        RPDHFAllP456->Fill( etHFtowerSumPlus , P_RPD456 );
        RPDHFAllM456->Fill( etHFtowerSumMinus, M_RPD456 );

        for ( int i = 0; i < 16; i++ ) {
            RPDiHFP4[i]  ->Fill( etHFtowerSumPlus, P_RPDTS4[i]  );
            RPDiHFP45[i] ->Fill( etHFtowerSumPlus, P_RPD[i]     );
            RPDiHFP456[i]->Fill( etHFtowerSumPlus, P_RPDTS456[i]);

            RPDiHFM4[i]  ->Fill( etHFtowerSumMinus, M_RPDTS4[i]  );
            RPDiHFM45[i] ->Fill( etHFtowerSumMinus, M_RPD[i]     );
            RPDiHFM456[i]->Fill( etHFtowerSumMinus, M_RPDTS456[i]);

            PRPD4Prof[i]  ->Fill( etHFtowerSumPlus, P_RPDTS4[i] );
            PRPD45Prof[i] ->Fill( etHFtowerSumPlus, P_RPD[i] );
            PRPD456Prof[i]->Fill( etHFtowerSumPlus, P_RPDTS456[i] );
            MRPD4Prof[i]  ->Fill( etHFtowerSumMinus, M_RPDTS4[i] );
            MRPD45Prof[i] ->Fill( etHFtowerSumMinus, M_RPD[i] );
            MRPD456Prof[i]->Fill( etHFtowerSumMinus, M_RPDTS456[i] );

            PRPD4ProfCent[i]  ->Fill( centBin, P_RPDTS4[i] );
            PRPD45ProfCent[i] ->Fill( centBin, P_RPD[i] );
            PRPD456ProfCent[i]->Fill( centBin, P_RPDTS456[i] );
            MRPD4ProfCent[i]  ->Fill( centBin, M_RPDTS4[i] );
            MRPD45ProfCent[i] ->Fill( centBin, M_RPD[i] );
            MRPD456ProfCent[i]->Fill( centBin, M_RPDTS456[i] );

            PRPD4ProfCentS[i]  ->Fill( centBin, P_RPDTS4[i] );
            PRPD45ProfCentS[i] ->Fill( centBin, P_RPD[i] );
            PRPD456ProfCentS[i]->Fill( centBin, P_RPDTS456[i] );
            MRPD4ProfCentS[i]  ->Fill( centBin, M_RPDTS4[i] );
            MRPD45ProfCentS[i] ->Fill( centBin, M_RPD[i] );
            MRPD456ProfCentS[i]->Fill( centBin, M_RPDTS456[i] );

            for ( int j = i+1; j < 16; j++ ) {
                hRPDvsRPDLowP[j][i][c]->Fill(P_RPD[j], P_RPD[i]);
                hRPDvsRPDLowM[j][i][c]->Fill(M_RPD[j], M_RPD[i]);

                hRPDvsRPDHighP[j][i][c]->Fill(P_RPD[j], P_RPD[i]);
                hRPDvsRPDHighM[j][i][c]->Fill(M_RPD[j], M_RPD[i]);
            }
        }

        for ( int i = 0; i < 16; i++ ) {
            for ( int j = 0; j < 5; j++ ) {
                hRPDvsEMP[j][i]->Fill(P_EM[j+1], P_RPD[i]);
                hRPDvsEMM[j][i]->Fill(M_EM[j+1], M_RPD[i]);
            }
        }

        double PX = 0.;
        double PY = 0.;
        double MX = 0.;
        double MY = 0.;
        double RPDEP_P = RPDEP2x2(P_RPD, PX, PY, centBin, true);
        double RPDEP_M = RPDEP2x2(M_RPD, MX, MY, centBin, false);

        hRPDEP1vsHFEP2P[c]->Fill( RPDEP_P, argp_HFQ2 );
        hRPDEP1vsHFEP2M[c]->Fill( RPDEP_M, argm_HFQ2 );
        hRPDEP[c]->Fill( RPDEP_M, RPDEP_P );

        hRPD2x2PXY[c]->Fill(PX, PY);
        hRPD2x2MXY[c]->Fill(MX, MY);

        hRPD2x2PMX[c]->Fill(PX, MX);
        hRPD2x2PMY[c]->Fill(PY, MY);

        hRPD2x2PEP[c]->Fill(RPDEP_P);
        hRPD2x2MEP[c]->Fill(RPDEP_M);

        hHF2EPP[c]->Fill(argp_HFQ2);
        hHF2EPM[c]->Fill(argm_HFQ2);

		idx++;
        //if ( idx > 10000000 ) break;
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
	auto fRAW = fsave->mkdir("RAW");
    fRAW->cd();
    for ( int c = 0; c < 10 ; c++ ) {
        for ( int i = 0; i < 5; i++ ) {
            hPRawEM[c][i]->Write();
            hMRawEM[c][i]->Write();
        }
        for ( int i = 0; i < 4; i++ ) {
            hPRawHad[c][i]->Write();
            hMRawHad[c][i]->Write();
        }
        for ( int i = 0; i < 16; i++ ) {
            hPRawRPD[c][i]->Write();
            hMRawRPD[c][i]->Write();
        }
    }
    auto fRAWTS4 = fsave->mkdir("RAWTS4");
    fRAWTS4->cd();
    for ( int c = 0; c < 10; c++ ) {
        for ( int i = 0; i < 5; i++ ) {
            hTS4PRawEM[c][i]->Write();
            hTS4MRawEM[c][i]->Write();
        }
        for ( int i = 0; i < 4; i++ ) {
            hTS4PRawHad[c][i]->Write();
            hTS4MRawHad[c][i]->Write();
        }
        for ( int i = 0; i < 16; i++ ) {
            hTS4PRawRPD[c][i]->Write();
            hTS4MRawRPD[c][i]->Write();
        }
    }
    auto fRAWTS456 = fsave->mkdir("RAWTS456");
    fRAWTS456->cd();
    for ( int c = 0; c < 10; c++ ) {
        for ( int i = 0; i < 16; i++ ) {
            hTS456PRawRPD[c][i]->Write();
            hTS456MRawRPD[c][i]->Write();
        }
    }
    auto fHFCorr4 = fsave->mkdir("HFCorr4");
    fHFCorr4->cd();
    for ( int c = 0; c < 10; c++ ) {
        RPDHFP4[c]->Write();
        RPDHFM4[c]->Write();
    }
    RPDHFAllP4->Write();
    RPDHFAllM4->Write();

    auto fHFCorr45 = fsave->mkdir("HFCorr45");
    fHFCorr45->cd();
    for ( int c = 0; c < 10; c++ ) {
        ZDCHFP[c]->Write();
        ZDCHFM[c]->Write();
        RPDHFP45[c]->Write();
        RPDHFM45[c]->Write();
    }
    ZDCHFAllP->Write();
    ZDCHFAllM->Write();
    RPDHFAllP45->Write();
    RPDHFAllM45->Write();

    auto fHFCorr456 = fsave->mkdir("HFCorr456");
    fHFCorr456->cd();
    for ( int c = 0; c < 10; c++ ) {
        RPDHFP456[c]->Write();
        RPDHFM456[c]->Write();
    }
    RPDHFAllP456->Write();
    RPDHFAllM456->Write();


    auto fRPDiHF4 = fsave->mkdir("RPDiHF4");
    fRPDiHF4->cd();
    for ( int i = 0; i < 16; i++ ) {
        RPDiHFP4[i]->Write();
        PRPD4Prof[i]->Write();
        PRPD4ProfCent[i]->Write();
        PRPD4ProfCentS[i]->Write();

        RPDiHFM4[i]->Write();
        MRPD4Prof[i]->Write();
        MRPD4ProfCent[i]->Write();
        MRPD4ProfCentS[i]->Write();
    }

    auto fRPDiHF45 = fsave->mkdir("RPDiHF45");
    fRPDiHF45->cd();
    for ( int i = 0; i < 16; i++ ) {
        RPDiHFP45[i]->Write();
        PRPD45Prof[i]->Write();
        PRPD45ProfCent[i]->Write();
        PRPD45ProfCentS[i]->Write();

        RPDiHFM45[i]->Write();
        MRPD45Prof[i]->Write();
        MRPD45ProfCent[i]->Write();
        MRPD45ProfCentS[i]->Write();
    }

    auto fRPDiHF456 = fsave->mkdir("RPDiHF456");
    fRPDiHF456->cd();
    for ( int i = 0; i < 16; i++ ) {
        RPDiHFP456[i]->Write();
        PRPD456Prof[i]->Write();
        PRPD456ProfCent[i]->Write();
        PRPD456ProfCentS[i]->Write();

        RPDiHFM456[i]->Write();
        MRPD456Prof[i]->Write();
        MRPD456ProfCent[i]->Write();
        MRPD456ProfCentS[i]->Write();
    }

    auto fRPDvsRPDLow45 = fsave->mkdir("RPDvsRPDLow45");
    fRPDvsRPDLow45->cd();
    for ( int c = 0; c < 10; c++ ) {
        for ( int i = 0; i < 16; i++ ) {
            for ( int j = i+1; j < 16; j++ ) {
                hRPDvsRPDLowP[j][i][c]->Write();
                hRPDvsRPDLowM[j][i][c]->Write();
            }
        }
    }

    auto fRPDvsRPDHigh45 = fsave->mkdir("RPDvsRPDHigh45");
    fRPDvsRPDHigh45->cd();
    for ( int c = 0; c < 10; c++ ) {
        for ( int i = 0; i < 16; i++ ) {
            for ( int j = i+1; j < 16; j++ ) {
                hRPDvsRPDHighP[j][i][c]->Write();
                hRPDvsRPDHighM[j][i][c]->Write();
            }
        }
    }

    auto fRPD1N45 = fsave->mkdir("RPD1N45");
    fRPD1N45->cd();
    for ( int i = 0; i < 16; i++ ) {
        for ( int j = i+1; j < 16; j++ ) {
            hRPDvsRPD1NP[j][i]->Write();
            hRPDvsRPD1NM[j][i]->Write();
        }
    }

    auto fEP = fsave->mkdir("EP");
    fEP->cd();
    for ( int c = 0; c < 10; c++ ) {
        hRPDEP1vsHFEP2P[c]->Write();
        hRPDEP1vsHFEP2M[c]->Write();
        hRPDEP[c]->Write();

        hRPD2x2PXY[c]->Write();
        hRPD2x2MXY[c]->Write();

        hRPD2x2PMX[c]->Write();
        hRPD2x2PMY[c]->Write();

        hRPD2x2PEP[c]->Write();
        hRPD2x2MEP[c]->Write();
        hHF2EPP[c]->Write();
        hHF2EPM[c]->Write();
    }

    auto fRPDvsEM = fsave->mkdir("RPDvsEM");
    fRPDvsEM->cd();
    for ( int i = 0; i < 16; i++ ) {
        for ( int j = 0; j < 5; j++ ) {
            hRPDvsEMP[j][i]->Write();
            hRPDvsEMM[j][i]->Write();
        }
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
