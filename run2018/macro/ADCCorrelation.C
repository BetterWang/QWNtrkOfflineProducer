#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <string>

using namespace std;

void ADCCorrelation(string input = "", string s = "test.root")
{
	TChain * trV = new TChain("trV");
	TChain * trC = new TChain("trC");

	trV->Add( (input + "/zdcana/ADC/trV").c_str() );
	trC->Add( (input + "/QWHFTree/trV").c_str() );
	trV->AddFriend(trC);

	Double_t centBin;
	trC->SetBranchAddress("_dbCentBin", &centBin);

    // Declaration of leaf types
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

    // List of branches
    TBranch        *b_hZDCM_EM1;   //!
    TBranch        *b_hZDCM_EM2;   //!
    TBranch        *b_hZDCM_EM3;   //!
    TBranch        *b_hZDCM_EM4;   //!
    TBranch        *b_hZDCM_EM5;   //!
    TBranch        *b_hZDCM_HAD1;   //!
    TBranch        *b_hZDCM_HAD2;   //!
    TBranch        *b_hZDCM_HAD3;   //!
    TBranch        *b_hZDCM_HAD4;   //!
    TBranch        *b_hZDCP_EM1;   //!
    TBranch        *b_hZDCP_EM2;   //!
    TBranch        *b_hZDCP_EM3;   //!
    TBranch        *b_hZDCP_EM4;   //!
    TBranch        *b_hZDCP_EM5;   //!
    TBranch        *b_hZDCP_HAD1;   //!
    TBranch        *b_hZDCP_HAD2;   //!
    TBranch        *b_hZDCP_HAD3;   //!
    TBranch        *b_hZDCP_HAD4;   //!
    TBranch        *b_hZDCM_RPD0;   //!
    TBranch        *b_hZDCM_RPD1;   //!
    TBranch        *b_hZDCM_RPD2;   //!
    TBranch        *b_hZDCM_RPD3;   //!
    TBranch        *b_hZDCM_RPD4;   //!
    TBranch        *b_hZDCM_RPD5;   //!
    TBranch        *b_hZDCM_RPD6;   //!
    TBranch        *b_hZDCM_RPD7;   //!
    TBranch        *b_hZDCM_RPD8;   //!
    TBranch        *b_hZDCM_RPD9;   //!
    TBranch        *b_hZDCM_RPD10;   //!
    TBranch        *b_hZDCM_RPD11;   //!
    TBranch        *b_hZDCM_RPD12;   //!
    TBranch        *b_hZDCM_RPD13;   //!
    TBranch        *b_hZDCM_RPD14;   //!
    TBranch        *b_hZDCM_RPD15;   //!
    TBranch        *b_hZDCP_RPD0;   //!
    TBranch        *b_hZDCP_RPD1;   //!
    TBranch        *b_hZDCP_RPD2;   //!
    TBranch        *b_hZDCP_RPD3;   //!
    TBranch        *b_hZDCP_RPD4;   //!
    TBranch        *b_hZDCP_RPD5;   //!
    TBranch        *b_hZDCP_RPD6;   //!
    TBranch        *b_hZDCP_RPD7;   //!
    TBranch        *b_hZDCP_RPD8;   //!
    TBranch        *b_hZDCP_RPD9;   //!
    TBranch        *b_hZDCP_RPD10;   //!
    TBranch        *b_hZDCP_RPD11;   //!
    TBranch        *b_hZDCP_RPD12;   //!
    TBranch        *b_hZDCP_RPD13;   //!
    TBranch        *b_hZDCP_RPD14;   //!
    TBranch        *b_hZDCP_RPD15;   //!

    trV->SetBranchAddress("hZDCM_EM1", hZDCM_EM1, &b_hZDCM_EM1);
    trV->SetBranchAddress("hZDCM_EM2", hZDCM_EM2, &b_hZDCM_EM2);
    trV->SetBranchAddress("hZDCM_EM3", hZDCM_EM3, &b_hZDCM_EM3);
    trV->SetBranchAddress("hZDCM_EM4", hZDCM_EM4, &b_hZDCM_EM4);
    trV->SetBranchAddress("hZDCM_EM5", hZDCM_EM5, &b_hZDCM_EM5);
    trV->SetBranchAddress("hZDCM_HAD1", hZDCM_HAD1, &b_hZDCM_HAD1);
    trV->SetBranchAddress("hZDCM_HAD2", hZDCM_HAD2, &b_hZDCM_HAD2);
    trV->SetBranchAddress("hZDCM_HAD3", hZDCM_HAD3, &b_hZDCM_HAD3);
    trV->SetBranchAddress("hZDCM_HAD4", hZDCM_HAD4, &b_hZDCM_HAD4);
    trV->SetBranchAddress("hZDCP_EM1", hZDCP_EM1, &b_hZDCP_EM1);
    trV->SetBranchAddress("hZDCP_EM2", hZDCP_EM2, &b_hZDCP_EM2);
    trV->SetBranchAddress("hZDCP_EM3", hZDCP_EM3, &b_hZDCP_EM3);
    trV->SetBranchAddress("hZDCP_EM4", hZDCP_EM4, &b_hZDCP_EM4);
    trV->SetBranchAddress("hZDCP_EM5", hZDCP_EM5, &b_hZDCP_EM5);
    trV->SetBranchAddress("hZDCP_HAD1", hZDCP_HAD1, &b_hZDCP_HAD1);
    trV->SetBranchAddress("hZDCP_HAD2", hZDCP_HAD2, &b_hZDCP_HAD2);
    trV->SetBranchAddress("hZDCP_HAD3", hZDCP_HAD3, &b_hZDCP_HAD3);
    trV->SetBranchAddress("hZDCP_HAD4", hZDCP_HAD4, &b_hZDCP_HAD4);
    trV->SetBranchAddress("hZDCM_RPD0", hZDCM_RPD0, &b_hZDCM_RPD0);
    trV->SetBranchAddress("hZDCM_RPD1", hZDCM_RPD1, &b_hZDCM_RPD1);
    trV->SetBranchAddress("hZDCM_RPD2", hZDCM_RPD2, &b_hZDCM_RPD2);
    trV->SetBranchAddress("hZDCM_RPD3", hZDCM_RPD3, &b_hZDCM_RPD3);
    trV->SetBranchAddress("hZDCM_RPD4", hZDCM_RPD4, &b_hZDCM_RPD4);
    trV->SetBranchAddress("hZDCM_RPD5", hZDCM_RPD5, &b_hZDCM_RPD5);
    trV->SetBranchAddress("hZDCM_RPD6", hZDCM_RPD6, &b_hZDCM_RPD6);
    trV->SetBranchAddress("hZDCM_RPD7", hZDCM_RPD7, &b_hZDCM_RPD7);
    trV->SetBranchAddress("hZDCM_RPD8", hZDCM_RPD8, &b_hZDCM_RPD8);
    trV->SetBranchAddress("hZDCM_RPD9", hZDCM_RPD9, &b_hZDCM_RPD9);
    trV->SetBranchAddress("hZDCM_RPD10", hZDCM_RPD10, &b_hZDCM_RPD10);
    trV->SetBranchAddress("hZDCM_RPD11", hZDCM_RPD11, &b_hZDCM_RPD11);
    trV->SetBranchAddress("hZDCM_RPD12", hZDCM_RPD12, &b_hZDCM_RPD12);
    trV->SetBranchAddress("hZDCM_RPD13", hZDCM_RPD13, &b_hZDCM_RPD13);
    trV->SetBranchAddress("hZDCM_RPD14", hZDCM_RPD14, &b_hZDCM_RPD14);
    trV->SetBranchAddress("hZDCM_RPD15", hZDCM_RPD15, &b_hZDCM_RPD15);
    trV->SetBranchAddress("hZDCP_RPD0", hZDCP_RPD0, &b_hZDCP_RPD0);
    trV->SetBranchAddress("hZDCP_RPD1", hZDCP_RPD1, &b_hZDCP_RPD1);
    trV->SetBranchAddress("hZDCP_RPD2", hZDCP_RPD2, &b_hZDCP_RPD2);
    trV->SetBranchAddress("hZDCP_RPD3", hZDCP_RPD3, &b_hZDCP_RPD3);
    trV->SetBranchAddress("hZDCP_RPD4", hZDCP_RPD4, &b_hZDCP_RPD4);
    trV->SetBranchAddress("hZDCP_RPD5", hZDCP_RPD5, &b_hZDCP_RPD5);
    trV->SetBranchAddress("hZDCP_RPD6", hZDCP_RPD6, &b_hZDCP_RPD6);
    trV->SetBranchAddress("hZDCP_RPD7", hZDCP_RPD7, &b_hZDCP_RPD7);
    trV->SetBranchAddress("hZDCP_RPD8", hZDCP_RPD8, &b_hZDCP_RPD8);
    trV->SetBranchAddress("hZDCP_RPD9", hZDCP_RPD9, &b_hZDCP_RPD9);
    trV->SetBranchAddress("hZDCP_RPD10", hZDCP_RPD10, &b_hZDCP_RPD10);
    trV->SetBranchAddress("hZDCP_RPD11", hZDCP_RPD11, &b_hZDCP_RPD11);
    trV->SetBranchAddress("hZDCP_RPD12", hZDCP_RPD12, &b_hZDCP_RPD12);
    trV->SetBranchAddress("hZDCP_RPD13", hZDCP_RPD13, &b_hZDCP_RPD13);
    trV->SetBranchAddress("hZDCP_RPD14", hZDCP_RPD14, &b_hZDCP_RPD14);
    trV->SetBranchAddress("hZDCP_RPD15", hZDCP_RPD15, &b_hZDCP_RPD15);


    TH2S * hHadRPDP[16][10];
    TH2S * hHadRPDM[16][10];

    TH2S * hEMRPDP[5][16][10];
    TH2S * hEMRPDM[5][16][10];

    TH2S * hRPDvsRPDP[16][16][10];
    TH2S * hRPDvsRPDM[16][16][10];

    for ( int c = 0; c < 10; c++ ) {
        for ( int i = 0; i < 16; i++ ) {
            hHadRPDP[i][c] = new TH2S(Form("hHadRPDP_%i_%i", i, c), Form(";PHad1;PRPD%i", i), 256, 0, 256, 256, 0, 256);
            hHadRPDM[i][c] = new TH2S(Form("hHadRPDM_%i_%i", i, c), Form(";MHad1;MRPD%i", i), 256, 0, 256, 256, 0, 256);

            for ( int j = 0; j < 5; j++ ) {
                hEMRPDP[j][i][c] = new TH2S(Form("hEMRPDP_%i_%i_%i", j, i , c), Form(";PEM%i;PRPD%i", j, i), 256, 0, 256, 256, 0, 256);
                hEMRPDM[j][i][c] = new TH2S(Form("hEMRPDM_%i_%i_%i", j, i , c), Form(";MEM%i;MRPD%i", j, i), 256, 0, 256, 256, 0, 256);
            }

            for ( int j = i+1; j < 16; j++ ) {
                hRPDvsRPDP[j][i][c] = new TH2S(Form("hRPDvsRPDP_%i_%i_%i", j, i, c), Form(";PRPD%i;PRPD%i", j, i), 256, 0, 256, 256, 0, 256);
                hRPDvsRPDM[j][i][c] = new TH2S(Form("hRPDvsRPDM_%i_%i_%i", j, i, c), Form(";MRPD%i;MRPD%i", j, i), 256, 0, 256, 256, 0, 256);
            }
        }
    }

    TH2S * hRPDPM[16][16][10];
    TH2S * hEMPM[5][5][10];

    for ( int c = 0; c < 10; c++ ) {
        for ( int i = 0; i < 16; i++ ) {
            for ( int j = 0; j < 16; j++ ) {
                hRPDPM[j][i][c] = new TH2S(Form("hRPDPM_%i_%i_%i", j, i, c), Form(";RPDP%i;RPDM%i", j, i), 256, 0, 256, 256, 0, 256);
            }
        }

        for ( int i = 0; i < 5; i++ ) {
            for ( int j = 0; j < 5; j++ ) {
                hEMPM[j][i][c] = new TH2S(Form("hEMPM_%i_%i_%i", j, i, c), Form(";EMP%i;EMM%i", j, i), 256, 0, 256, 256, 0, 256);
            }
        }
    }


	int idx = 0;
	while ( trV->GetEntry(idx) ) {
		if (idx%1000 == 0) cout << " --> idx = " << idx << endl;
		int c = int(centBin)/20;

        short P_HAD = short( hZDCP_HAD1[4] );
        short M_HAD = short( hZDCM_HAD1[4] );
        short P_EM[5] = { short(hZDCP_EM1[4]), short(hZDCP_EM2[4]), short(hZDCP_EM3[4]), short(hZDCP_EM4[4]), short(hZDCP_EM5[4]), };
        short M_EM[5] = { short(hZDCM_EM1[4]), short(hZDCM_EM2[4]), short(hZDCM_EM3[4]), short(hZDCM_EM4[4]), short(hZDCM_EM5[4]), };

        short P_RPD[16] = { short(hZDCP_RPD0[4]), short(hZDCP_RPD1[4]), short(hZDCP_RPD2[4]), short(hZDCP_RPD3[4]),
                            short(hZDCP_RPD4[4]), short(hZDCP_RPD5[4]), short(hZDCP_RPD6[4]), short(hZDCP_RPD7[4]),
                            short(hZDCP_RPD8[4]), short(hZDCP_RPD9[4]), short(hZDCP_RPD10[4]),short(hZDCP_RPD11[4]),
                            short(hZDCP_RPD12[4]),short(hZDCP_RPD13[4]),short(hZDCP_RPD14[4]),short(hZDCP_RPD15[4]) };

        short M_RPD[16] = { short(hZDCM_RPD0[4]), short(hZDCM_RPD1[4]), short(hZDCM_RPD2[4]), short(hZDCM_RPD3[4]),
                            short(hZDCM_RPD4[4]), short(hZDCM_RPD5[4]), short(hZDCM_RPD6[4]), short(hZDCM_RPD7[4]),
                            short(hZDCM_RPD8[4]), short(hZDCM_RPD9[4]), short(hZDCM_RPD10[4]),short(hZDCM_RPD11[4]),
                            short(hZDCM_RPD12[4]),short(hZDCM_RPD13[4]),short(hZDCM_RPD14[4]),short(hZDCM_RPD15[4]) };

        for ( int i = 0; i < 16; i++ ) {
            hHadRPDP[i][c]->Fill(P_HAD, P_RPD[i]);
            hHadRPDM[i][c]->Fill(M_HAD, M_RPD[i]);

            for ( int j = 0; j < 5; j++ ) {
                hEMRPDP[j][i][c]->Fill(P_EM[j], P_RPD[i]);
                hEMRPDM[j][i][c]->Fill(M_EM[j], M_RPD[i]);
            }

            for ( int j = i+1; j < 16; j++ ) {
                hRPDvsRPDP[j][i][c]->Fill(P_RPD[j], P_RPD[i]);
                hRPDvsRPDM[j][i][c]->Fill(M_RPD[j], M_RPD[i]);
            }

            for ( int j = 0; j < 16; j++ ) {
                hRPDPM[j][i][c]->Fill(P_RPD[j], M_RPD[i]);
            }
        }

        for ( int i = 0; i < 5; i++ ) {
            for ( int j = 0; j < 5; j++ ) {
                hEMPM[j][i][c]->Fill(P_EM[j], M_EM[i]);
            }
        }

        idx++;
        if ( idx > 5000000 ) break;
    }

	TFile * fsave = new TFile(s.c_str(), "recreate");

	auto fHad = fsave->mkdir("Had");
    fHad->cd();
    for ( int c = 0; c < 10; c++ ) {
        for ( int i = 0; i < 16; i++ ) {
            hHadRPDP[i][c]->Write();
            hHadRPDM[i][c]->Write();
        }
    }

	auto fEM = fsave->mkdir("EM");
    fEM->cd();
    for ( int c = 0; c < 10; c++ ) {
        for ( int i = 0; i < 16; i++ ) {
            for ( int j = 0; j < 5; j++ ) {
                hEMRPDP[j][i][c]->Write();
                hEMRPDM[j][i][c]->Write();
            }
        }
    }

	auto fRPD = fsave->mkdir("RPD");
    fRPD->cd();
    for ( int c = 0; c < 10; c++ ) {
        for ( int i = 0; i < 16; i++ ) {
            for ( int j = i+1; j < 16; j++ ) {
                hRPDvsRPDP[j][i][c]->Write();
                hRPDvsRPDM[j][i][c]->Write();
            }
        }
    }

    auto fEMPM = fsave->mkdir("EMPM");
    fEMPM->cd();
    for ( int c = 0; c < 10; c++ ) {
        for ( int i = 0; i < 5; i++ ) {
            for ( int j = 0; j < 5; j++ ) {
                hEMPM[j][i][c]->Write();
            }
        }
    }

    auto fRPDPM = fsave->mkdir("RPDPM");
    fRPDPM->cd();
    for ( int c = 0; c < 10; c++ ) {
        for ( int i = 0; i < 16; i++ ) {
            for ( int j = 0; j < 16; j++ ) {
                hRPDPM[j][i][c]->Write();
            }
        }
    }

}
