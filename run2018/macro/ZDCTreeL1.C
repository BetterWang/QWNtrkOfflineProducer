#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

void
ZDCTreeL1(string input, string s = "test.root")
{
	TChain * trV = new TChain("trV");
	trV->Add("ZDC2018/ZDCZBL1Tree/*.root/zdcana/ADC/trV");
	TChain * trL1 = new TChain("trV");
	trL1->Add("ZDC2018/ZDCZBL1Tree/*.root/QWL1/trV");

	trV->AddFriend(trL1);

	TH1D* hZDCPFire = new TH1D("hZDCPFire", "hZDCPFire", 2, 0, 2);

	Bool_t          bFinal[512][5];
	Bool_t          bInitial[512][5];
	Bool_t          bInterm[512][5];

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

	trL1->SetBranchAddress("bFinal", bFinal);
	trL1->SetBranchAddress("bInitial", bInitial);
	trL1->SetBranchAddress("bInterm", bInterm);

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
				hP1NEM[i]->Fill(P_EM[i+1]);
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
				hPAllEM[i]->Fill(P_EM[i+1]);
			}
			for ( int i = 0; i < 4; i++ ) {
				hPAllHAD[i]->Fill(P_HAD[i+1]);
			}
			for ( int i = 0; i < 16; i++ ) {
				hPAllRPD[i]->Fill(P_RPD[i]);
			}
		}
		for ( int i = 0; i < 5; i++ ) {
			hPIncEM[i]->Fill(P_EM[i+1]);
		}
		for ( int i = 0; i < 4; i++ ) {
			hPIncHAD[i]->Fill(P_HAD[i+1]);
		}
		for ( int i = 0; i < 16; i++ ) {
			hPIncRPD[i]->Fill(P_RPD[i]);
		}

		if ( M_select and bM1nT ) {
			for ( int i = 0; i < 5; i++ ) {
				hM1NEM[i]->Fill(M_EM[i+1]);
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
				hMAllEM[i]->Fill(M_EM[i+1]);
			}
			for ( int i = 0; i < 4; i++ ) {
				hMAllHAD[i]->Fill(M_HAD[i+1]);
			}
			for ( int i = 0; i < 16; i++ ) {
				hMAllRPD[i]->Fill(M_RPD[i]);
			}
		}
		for ( int i = 0; i < 5; i++ ) {
			hMIncEM[i]->Fill(M_EM[i+1]);
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

