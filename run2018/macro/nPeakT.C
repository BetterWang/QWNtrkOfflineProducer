{
	TChain * trV = new TChain("trV");
	TChain * trADC = new TChain("trADC");
	TChain * trC = new TChain("trC");
	TChain * trP = new TChain("trP");
	TChain * trB = new TChain("trB");
//	TFile * f = new TFile("Npeak_Study.root", "recreate");


	trV->  Add("zdc_326790_MBCent*.root/zdcana/fC/trV");
	trADC->Add("zdc_326790_MBCent*.root/zdcana/ADC/trV");
	trC->  Add("zdc_326790_MBCent*.root/centTree/trV");
	trP->  Add("zdc_326790_MBCent*.root/QWfilter/trV");
	trB->  Add("zdc_326790_MBCent*.root/QWBXTree/trV");

	trV->AddFriend(trC);
	trV->AddFriend(trADC);
	trV->AddFriend(trP);
	trV->AddFriend(trB);

	trV->SetAlias("trMB", "pprimaryVertexFilter && phfCoincFilter2Th4 && pclusterCompatibilityFilter");

	TH1D * hHad  = new TH1D("hHad",  "hHad", 4000, 0, 400000);
	TH1D * hHadP = new TH1D("hHadP", "hHad",10000, 0,1000000);
	TH1D * hHadM = new TH1D("hHadM", "hHad",10000, 0,1000000);
	TH1D * hHadR = new TH1D("hHadR", "hHad", 4000, 0, 400000);
	TH1D * hHadB = new TH1D("hHadB", "hHad", 4000, 0, 400000);
	TH1D * hHadC = new TH1D("hHadC", "hHad", 4000, 0, 400000);
	TH1D * hHadG = new TH1D("hHadG", "hHad", 4000, 0, 400000);

	TH1D * hEM   = new TH1D("hEM",   "hEM", 102, -2000, 100000);
	TH1D * hEMR  = new TH1D("hEMR",  "hEM", 102, -2000, 100000);
	TH1D * hEMB  = new TH1D("hEMB",  "hEM", 102, -2000, 100000);
	TH1D * hEMC  = new TH1D("hEMC",  "hEM", 102, -2000, 100000);
	TH1D * hEMG  = new TH1D("hEMG",  "hEM", 102, -2000, 100000);

	TH1D * hRatio   = new TH1D("hRatio",  "hRatio", 200, 0, 2.);
	TH1D * hRatioR  = new TH1D("hRatioR", "hRatio", 200, 0, 2.);
	TH1D * hRatioB  = new TH1D("hRatioB", "hRatio", 200, 0, 2.);
	TH1D * hRatioC  = new TH1D("hRatioC", "hRatio", 200, 0, 2.);

	hHadR->SetLineColor(kRed);
	hHadM->SetLineColor(kRed);
	hHadB->SetLineColor(kBlue);
	hHadC->SetLineColor(kCyan);
	hHadG->SetLineColor(kGreen);

	hEMR->SetLineColor(kRed);
	hEMB->SetLineColor(kBlue);
	hEMC->SetLineColor(kCyan);
	hEMG->SetLineColor(kGreen+2);

	hRatioR->SetLineColor(kRed);
	hRatioB->SetLineColor(kBlue);
	hRatioC->SetLineColor(kCyan);

	TH2D * hHad2  = new TH2D("hHad2",  "hHad2", 1000, 0, 1000000, 1000, 0, 1000000);

	trV->SetAlias("M_HAD1", "hZDCM_HAD1[4]+hZDCM_HAD1[5]");
	trV->SetAlias("M_HAD2", "hZDCM_HAD2[4]+hZDCM_HAD2[5]");
	trV->SetAlias("M_HAD3", "hZDCM_HAD3[4]+hZDCM_HAD3[5]");
	trV->SetAlias("M_HAD4", "hZDCM_HAD4[4]+hZDCM_HAD4[5]");

	trV->SetAlias("M_EM1", "hZDCM_EM1[4]+hZDCM_EM1[5]");
	trV->SetAlias("M_EM2", "hZDCM_EM2[4]+hZDCM_EM2[5]");
	trV->SetAlias("M_EM3", "hZDCM_EM3[4]+hZDCM_EM3[5]");
	trV->SetAlias("M_EM4", "hZDCM_EM4[4]+hZDCM_EM4[5]");
	trV->SetAlias("M_EM5", "hZDCM_EM5[4]+hZDCM_EM5[5]");

	trV->SetAlias("P_HAD1", "hZDCP_HAD1[4]+hZDCP_HAD1[5]");
	trV->SetAlias("P_HAD2", "hZDCP_HAD2[4]+hZDCP_HAD2[5]");
	trV->SetAlias("P_HAD3", "hZDCP_HAD3[4]+hZDCP_HAD3[5]");
	trV->SetAlias("P_HAD4", "hZDCP_HAD4[4]+hZDCP_HAD4[5]");

	trV->SetAlias("P_EM1", "hZDCP_EM1[4]+hZDCP_EM1[5]");
	trV->SetAlias("P_EM2", "hZDCP_EM2[4]+hZDCP_EM2[5]");
	trV->SetAlias("P_EM3", "hZDCP_EM3[4]+hZDCP_EM3[5]");
	trV->SetAlias("P_EM4", "hZDCP_EM4[4]+hZDCP_EM4[5]");
	trV->SetAlias("P_EM5", "hZDCP_EM5[4]+hZDCP_EM5[5]");

	trV->SetAlias("P_RPD0", "hZDCP_RPD0[4]+hZDCP_RPD0[5]");
	trV->SetAlias("P_RPD1", "hZDCP_RPD1[4]+hZDCP_RPD1[5]");
	trV->SetAlias("P_RPD2", "hZDCP_RPD2[4]+hZDCP_RPD2[5]");
	trV->SetAlias("P_RPD3", "hZDCP_RPD3[4]+hZDCP_RPD3[5]");
	trV->SetAlias("P_RPD4", "hZDCP_RPD4[4]+hZDCP_RPD4[5]");
	trV->SetAlias("P_RPD5", "hZDCP_RPD5[4]+hZDCP_RPD5[5]");
	trV->SetAlias("P_RPD6", "hZDCP_RPD6[4]+hZDCP_RPD6[5]");
	trV->SetAlias("P_RPD7", "hZDCP_RPD7[4]+hZDCP_RPD7[5]");
	trV->SetAlias("P_RPD8", "hZDCP_RPD8[4]+hZDCP_RPD8[5]");
	trV->SetAlias("P_RPD9", "hZDCP_RPD9[4]+hZDCP_RPD9[5]");
	trV->SetAlias("P_RPD10", "hZDCP_RPD10[4]+hZDCP_RPD10[5]");
	trV->SetAlias("P_RPD11", "hZDCP_RPD11[4]+hZDCP_RPD11[5]");
	trV->SetAlias("P_RPD12", "hZDCP_RPD12[4]+hZDCP_RPD12[5]");
	trV->SetAlias("P_RPD13", "hZDCP_RPD13[4]+hZDCP_RPD13[5]");
	trV->SetAlias("P_RPD14", "hZDCP_RPD14[4]+hZDCP_RPD14[5]");
	trV->SetAlias("P_RPD15", "hZDCP_RPD15[4]+hZDCP_RPD15[5]");
	// 6700

	TCut P_select = "(hZDCP_HAD1[4]==Max$(hZDCP_HAD1) || hZDCP_HAD1[5]==Max$(hZDCP_HAD1)) && (hZDCP_HAD2[4]==Max$(hZDCP_HAD2) || hZDCP_HAD2[5]==Max$(hZDCP_HAD2))";
	TCut M_select = "(hZDCM_HAD1[4]==Max$(hZDCM_HAD1) || hZDCM_HAD1[5]==Max$(hZDCM_HAD1)) && (hZDCM_HAD2[4]==Max$(hZDCM_HAD2) || hZDCM_HAD2[5]==Max$(hZDCM_HAD2))";
	trV->SetAlias("P_NpeakT", "0.52*P_HAD4+0.46*P_HAD3 + 0.61*P_HAD2 + P_HAD1 + 0.1*(P_EM1+P_EM2+P_EM3+P_EM4+P_EM5)");
	trV->SetAlias("P_NpeakHad", "0.52*P_HAD4+0.46*P_HAD3 + 0.61*P_HAD2 + P_HAD1");

	trV->SetAlias("M_NpeakT", "(0.58*M_HAD4+0.46*M_HAD3 + 0.72*M_HAD2 + M_HAD1 + 0.1*(M_EM1+M_EM2+M_EM3+M_EM4+M_EM5))");
	trV->SetAlias("M_NpeakHad", "(0.58*M_HAD4+0.46*M_HAD3 + 0.72*M_HAD2 + M_HAD1)");


	TCut P_EMON = "(P_EM1>1000) || (P_EM2>1000) || (P_EM3>1000) || (P_EM4>1000) || (P_EM5>1000)";
//	trV->SetAlias("P_NpeakHad", "0.52*P_HAD4+0.46*P_HAD3 + 0.61*P_HAD2 + P_HAD1");
	trV->SetAlias("P_NpeakHad12", "0.61*P_HAD2 + P_HAD1");

	TCut P1nT = "P_NpeakT>2000 && P_NpeakT<12000";
	TCut P2nT = "P_NpeakT>12000 && P_NpeakT<18000";
	TCut M1nT = "M_NpeakT>5000 && M_NpeakT<18000";
	TCut M2nT = "M_NpeakT>18000 && M_NpeakT<30000";

	trV->SetAlias("P_EMSum", "0.15*P_EM1+0.15*P_EM2+0.15*P_EM3+0.17*P_EM4+0.13*P_EM5");

	trV->SetAlias("P_NpeakN", "0.52*P_HAD4+0.46*P_HAD3 + 0.61*P_HAD2 + P_HAD1 + (0.15*P_EM1+0.15*P_EM2+0.15*P_EM3+0.17*P_EM4+0.13*P_EM5)");

	TCut P0nN = "P_NpeakN<4000";
	TCut P1nN = "P_NpeakN>4000 && P_NpeakN<11200";
	TCut P2nN = "P_NpeakN>11200 && P_NpeakN<18450";
	TCut P3nN = "P_NpeakN>18450 && P_NpeakN<25700";

	trV->SetAlias("M_EMSum", "0.10*M_EM1+0.10*M_EM2+0.11*M_EM3+0.11*M_EM4+0.15*M_EM5");
	trV->SetAlias("M_NpeakN", "0.58*M_HAD4+0.46*M_HAD3 + 0.72*M_HAD2 + M_HAD1 + 0.10*M_EM1+0.10*M_EM2+0.11*M_EM3+0.11*M_EM4+0.15*M_EM5");

	TCut M0nN = "M_NpeakN<5000";
	TCut M1nN = "M_NpeakN>5000 && M_NpeakN<17000";
	TCut M2nN = "M_NpeakN>17000 && M_NpeakN<28500";
	TCut M3nN = "M_NpeakN>28500 && M_NpeakN<39500";

	trV->SetAlias("P_rec", "(P_NpeakN-480.)/2.7171315");
	trV->SetAlias("M_rec", "(M_NpeakN-480.)/4.1115538");

//	trV->Draw("P_NpeakN>>hHadP", "trMB", "goff");
//	trV->Draw("M_NpeakN>>hHadM", "trMB", "goff");
//	trV->Draw("P_rec>>hHadR", "trMB", "goff");
//	trV->Draw("M_rec>>hHadB", "trMB", "goff");


}
