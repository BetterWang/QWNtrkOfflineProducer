{
	TChain * trV = new TChain("trV");
	TChain * trADC = new TChain("trADC");
	TChain * trC = new TChain("trC");
//	TFile * f = new TFile("Npeak_Study.root", "recreate");


	trV->  Add("zdc_326790_MBCent*.root/zdcana/fC/trV");
	trADC->Add("zdc_326790_MBCent*.root/zdcana/ADC/trV");
	trC->  Add("zdc_326790_MBCent*.root/centTree/trV");

	trV->AddFriend(trC);
	trV->AddFriend(trADC);

	TH1D * hHad  = new TH1D("hHad",  "hHad", 1000, 0, 1000000);
	TH1D * hHadR = new TH1D("hHadR", "hHad", 1000, 0, 1000000);
	TH1D * hHadB = new TH1D("hHadB", "hHad", 1000, 0, 1000000);
	TH1D * hHadC = new TH1D("hHadC", "hHad", 1000, 0, 1000000);

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
	hHadB->SetLineColor(kBlue);
	hHadC->SetLineColor(kCyan);

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
	/*
	trV->SetAlias("M_HAD1", "hZDCM_HAD1[4]+hZDCM_HAD1[5]+hZDCM_HAD1[6]");
	trV->SetAlias("M_HAD2", "hZDCM_HAD2[4]+hZDCM_HAD2[5]+hZDCM_HAD2[6]");
	trV->SetAlias("M_HAD3", "hZDCM_HAD3[4]+hZDCM_HAD3[5]+hZDCM_HAD3[6]");
	trV->SetAlias("M_HAD4", "hZDCM_HAD4[4]+hZDCM_HAD4[5]+hZDCM_HAD4[6]");

	trV->SetAlias("M_EM1", "hZDCM_EM1[4]+hZDCM_EM1[5]+hZDCM_EM1[6]");
	trV->SetAlias("M_EM2", "hZDCM_EM2[4]+hZDCM_EM2[5]+hZDCM_EM2[6]");
	trV->SetAlias("M_EM3", "hZDCM_EM3[4]+hZDCM_EM3[5]+hZDCM_EM3[6]");
	trV->SetAlias("M_EM4", "hZDCM_EM4[4]+hZDCM_EM4[5]+hZDCM_EM4[6]");
	trV->SetAlias("M_EM5", "hZDCM_EM5[4]+hZDCM_EM5[5]+hZDCM_EM5[6]");

	trV->SetAlias("P_HAD1", "hZDCP_HAD1[4]+hZDCP_HAD1[5]+hZDCP_HAD1[6]");
	trV->SetAlias("P_HAD2", "hZDCP_HAD2[4]+hZDCP_HAD2[5]+hZDCP_HAD2[6]");
	trV->SetAlias("P_HAD3", "hZDCP_HAD3[4]+hZDCP_HAD3[5]+hZDCP_HAD3[6]");
	trV->SetAlias("P_HAD4", "hZDCP_HAD4[4]+hZDCP_HAD4[5]+hZDCP_HAD4[6]");

	trV->SetAlias("P_EM1", "hZDCP_EM1[4]+hZDCP_EM1[5]+hZDCP_EM1[6]");
	trV->SetAlias("P_EM2", "hZDCP_EM2[4]+hZDCP_EM2[5]+hZDCP_EM2[6]");
	trV->SetAlias("P_EM3", "hZDCP_EM3[4]+hZDCP_EM3[5]+hZDCP_EM3[6]");
	trV->SetAlias("P_EM4", "hZDCP_EM4[4]+hZDCP_EM4[5]+hZDCP_EM4[6]");
	trV->SetAlias("P_EM5", "hZDCP_EM5[4]+hZDCP_EM5[5]+hZDCP_EM5[6]");
	*/

	// 1. 0.61, 0.3, 0.26
	// n-peak - M side
//	trV->SetAlias("M_NpeakOrg", "0.1*(M_EM1+M_EM2+M_EM3+M_EM4+M_EM5)+0.65*M_HAD4+0.6*M_HAD3+0.6*M_HAD2+M_HAD1");

//	trV->SetAlias("M_Npeak_NOEM", "0.65*(hZDCM_HAD4[4]+hZDCM_HAD4[5]+hZDCM_HAD4[6])+0.6*(hZDCM_HAD3[4]+hZDCM_HAD3[5]+hZDCM_HAD3[6])+0.6*(hZDCM_HAD2[4]+hZDCM_HAD2[5]+hZDCM_HAD2[6])+(hZDCM_HAD1[4]+hZDCM_HAD1[5]+hZDCM_HAD1[6])");
//	TCut M_select = "(hZDCM_HAD1[4]>500 || hZDCM_HAD1[5]>500 ) && (hZDCM_HAD2[4]>500 || hZDCM_HAD2[5]>500 ) && (hZDCM_HAD1[4]==Max$(hZDCM_HAD1) || hZDCM_HAD1[5]==Max$(hZDCM_HAD1)) && (hZDCM_HAD2[4]==Max$(hZDCM_HAD2) || hZDCM_HAD2[5]==Max$(hZDCM_HAD2))";

//	trV->Draw("M_NpeakOrg>>hHad", M_select);

//	TCut Mcut1n = "M_NpeakOrg<40000";
//	TCut cutMEM1_peak = "hZDCM_EM1[4]==Max$(hZDCM_EM1) || hZDCM_EM1[5]==Max$(hZDCM_EM1)";
//	TCut cutMEM2_peak = "hZDCM_EM2[4]==Max$(hZDCM_EM2) || hZDCM_EM2[5]==Max$(hZDCM_EM2)";
//	TCut cutMEM3_peak = "hZDCM_EM3[4]==Max$(hZDCM_EM3) || hZDCM_EM3[5]==Max$(hZDCM_EM3)";
//	TCut cutMEM4_peak = "hZDCM_EM4[4]==Max$(hZDCM_EM4) || hZDCM_EM4[5]==Max$(hZDCM_EM4)";
//	TCut cutMEM5_peak = "hZDCM_EM5[4]==Max$(hZDCM_EM5) || hZDCM_EM5[5]==Max$(hZDCM_EM5)";

//	trV->Draw("M_EM1>>hHad", Mcut1n); // mean 3079   , 0.78
//	trV->Draw("M_EM2>>hHad", Mcut1n); // mean 2960   , 0.75
//	trV->Draw("M_EM3>>hHad", Mcut1n); // mean 3949   , 1.
//	trV->Draw("M_EM4>>hHad", Mcut1n); // mean 5113   , 1.29
//	trV->Draw("M_EM5>>hHad", Mcut1n); // mean 8759   , 2.22
//	trV->Draw("M_EM3/M_HAD1>>(1000,1,10)", M_select);
//	trV->Draw("M_Npeak_NOEM>>hHad", M_select);

//	trV->SetAlias("M_NpeakOpt", "M_Npeak_NOEM + 0.1*(M_EM1/0.78 + M_EM2/0.75 + M_EM3 + M_EM4/1.29 + M_EM5/2.2)");
//	trV->Draw("M_NpeakOpt>>hHad", M_select);

	// P side
//	trV->SetAlias("P_Npeak_NOEM", "1.3*P_HAD4+1.2*P_HAD3+1.22*P_HAD2+P_HAD1");
//	trV->SetAlias("P_NpeakOrg", "0.1*(P_EM1+P_EM2+P_EM3+P_EM4+P_EM5)+P_Npeak_NOEM");
//	TCut P_select = "(hZDCP_HAD1[4]>500 || hZDCP_HAD1[5]>500 || hZDCP_HAD1[6]>500) && (hZDCP_HAD2[4]>500 || hZDCP_HAD2[5]>500 || hZDCP_HAD2[6]>500) && (hZDCP_HAD1[4]==Max$(hZDCP_HAD1) || hZDCP_HAD1[5]==Max$(hZDCP_HAD1)) && (hZDCP_HAD2[4]==Max$(hZDCP_HAD2) || hZDCP_HAD2[5]==Max$(hZDCP_HAD2))";
//	TCut P_select = "(hZDCP_HAD1[4]>500 || hZDCP_HAD1[5]>500 ) && (hZDCP_HAD2[4]>500 || hZDCP_HAD2[5]>500 ) && (hZDCP_HAD1[4]==Max$(hZDCP_HAD1) || hZDCP_HAD1[5]==Max$(hZDCP_HAD1)) && (hZDCP_HAD2[4]==Max$(hZDCP_HAD2) || hZDCP_HAD2[5]==Max$(hZDCP_HAD2))";

//	TCut Pcut1n = "P_NpeakOrg<32000";

//	trV->Draw("P_EM1>>hHad",  Pcut1n); // mean 1192   , 0.42
//	trV->Draw("P_EM2>>hHad",  Pcut1n); // mean 1906   , 0.67
//	trV->Draw("P_EM3>>hHad",  Pcut1n); // mean 2832   , 1.
//	trV->Draw("P_EM4>>hHad",  Pcut1n); // mean 4012   , 1.42
//	trV->Draw("P_EM5>>hHad",  Pcut1n); // mean 3247   , 1.15

//	trV->SetAlias("P_NpeakOpt", "0.1*1.27*(P_EM1/0.42+P_EM2/0.67+P_EM3+P_EM4/1.42+P_EM5/1.15)+P_Npeak_NOEM");

//	trV->Draw("P_NpeakOrg>>hHad", P_select);
//	trV->Draw("P_NpeakOpt>>hHad", P_select);
//	trV->Draw("P_EM3/P_HAD1>>(1000,1,10)", P_select);

//	trV->Draw("M_NpeakOpt-M_Npeak_NOEM:M_NpeakOpt", M_select);


	/* Draw stuff */
//	trV->Draw("P_HAD2/P_HAD1>>(1000, 0, 10)", P_select);
//	trV->Draw("P_HAD3/P_HAD1>>(1000, 0, 10)", P_select);
//	trV->Draw("P_HAD4/P_HAD1>>(1000, 0, 10)", P_select);
//
//	trV->Draw("M_HAD2/M_HAD1>>(1000, 0, 10)", M_select);
//	trV->Draw("M_HAD3/M_HAD1>>(1000, 0, 10)", M_select);
//	trV->Draw("M_HAD4/M_HAD1>>(1000, 0, 10)", M_select);
//
//	trV->Draw("P_HAD4/M_HAD1>>(1000, 0, 10)", M_select&&P_select);

//	trV->Draw("0.81*P_HAD4+0.67*P_HAD3 + 0.81*P_HAD2 + P_HAD1 + 0.1*(P_EM1+P_EM2+P_EM3+P_EM4+P_EM5)", P_select);

///	trV->SetAlias("P_NpeakT", "0.52*P_HAD4+0.46*P_HAD3 + 0.61*P_HAD2 + P_HAD1 + 0.1*(P_EM1+P_EM2+P_EM3+P_EM4+P_EM5)");
///	trV->SetAlias("M_NpeakT", "(0.58*M_HAD4+0.46*M_HAD3 + 0.81*M_HAD2 + M_HAD1 + 0.1*(M_EM1+M_EM2+M_EM3+M_EM4+M_EM5))");
///
///	TCut M1nT = "M_NpeakT>6000 && M_NpeakT<18000";
///	TCut P1nT = "P_NpeakT<11000";

//	trV->Draw("P_EM1>>hEM", P_select&&P1nT&&"P_EM1>1000");
//	trV->Draw("P_EM2>>hEM", P_select&&P1nT&&"P_EM2>1000");
//	trV->Draw("P_EM3>>hEM", P_select&&P1nT&&"P_EM3>1000");
//	trV->Draw("P_EM4>>hEM", P_select&&P1nT&&"P_EM4>1000");
//	trV->Draw("P_EM5>>hEM", P_select&&P1nT&&"P_EM5>1000");

//	trV->Draw("M_EM1>>hEM", M_select&&M1nT&&"M_EM1>1000");
//	trV->Draw("M_EM2>>hEM", M_select&&M1nT&&"M_EM2>1000");
//	trV->Draw("M_EM3>>hEM", M_select&&M1nT&&"M_EM3>1000");
//	trV->Draw("M_EM4>>hEM", M_select&&M1nT&&"M_EM4>1000");
//	trV->Draw("M_EM5>>hEM", M_select&&M1nT&&"M_EM5>1000");

//	trV->Draw("P_HAD1>>hEM", P_select&&P1nT&&"P_HAD1>1000");
//	trV->Draw("P_HAD2>>hEM", P_select&&P1nT&&"P_HAD2>1000");
//	trV->Draw("P_HAD3>>hEM", P_select&&P1nT&&"P_HAD3>1000");
//	trV->Draw("P_HAD4>>hEM", P_select&&P1nT&&"P_HAD4>1000");
//
//	trV->Draw("M_HAD1>>hEM", M_select&&M1nT&&"M_HAD1>1000");
//	trV->Draw("M_HAD2>>hEM", M_select&&M1nT&&"M_HAD2>1000");
//	trV->Draw("M_HAD3>>hEM", M_select&&M1nT&&"M_HAD3>1000");
//	trV->Draw("M_HAD4>>hEM", M_select&&M1nT&&"M_HAD4>1000");
//
//	trV->Draw("P_EM3>>hEM",  P_select&&P1nT, "Goff");
//	trV->Draw("P_EM5/2.37>>hEMR", P_select&&P1nT, "Goff");
//	trV->Draw("M_EM3>>hEMB", M_select&&P1nT, "Goff");
//	trV->Draw("M_EM5/2.67>>hEMC", M_select&&P1nT, "Goff");

//	trV->SetAlias("P_NpeakH", "0.52*P_HAD4+0.46*P_HAD3 + 0.61*P_HAD2 + P_HAD1 ");
//	trV->SetAlias("M_NpeakH", "(0.58*M_HAD4+0.46*M_HAD3 + 0.81*M_HAD2 + M_HAD1 )");
//
//	TCut P1nH = "P_NpeakH<11000";
//	TCut M1nH = "M_NpeakH>5000 && M_NpeakT<19000";

//	trV->Draw("M_HAD1/M_EM2>>(1000, 0, 10)", M1nT);

//	trV->SetAlias("P_NpeakOpt", "0.52*P_HAD4+0.46*P_HAD3 + 0.61*P_HAD2 + P_HAD1 + 0.1*(1.1*P_EM1+1.1*P_EM2+0.8*P_EM3+1.0*P_EM4+0.6*P_EM5)");
//	trV->SetAlias("M_NpeakOpt", "0.6*(0.58*M_HAD4+0.46*M_HAD3 + 0.81*M_HAD2 + M_HAD1 + 0.1*(1.2*M_EM1+1.2*M_EM2+0.8*M_EM3+1.0*M_EM4+1.1*M_EM5))");

//	trV->Draw("P_NpeakOpt>>hHad", P_select);
//	trV->Draw("M_NpeakOpt>>hHad", M_select);

	// 6700

	TCut P_select = "(hZDCP_HAD1[4]==Max$(hZDCP_HAD1) || hZDCP_HAD1[5]==Max$(hZDCP_HAD1)) && (hZDCP_HAD2[4]==Max$(hZDCP_HAD2) || hZDCP_HAD2[5]==Max$(hZDCP_HAD2))";
	TCut M_select = "(hZDCM_HAD1[4]==Max$(hZDCM_HAD1) || hZDCM_HAD1[5]==Max$(hZDCM_HAD1)) && (hZDCM_HAD2[4]==Max$(hZDCM_HAD2) || hZDCM_HAD2[5]==Max$(hZDCM_HAD2))";
	trV->SetAlias("P_NpeakT", "0.52*P_HAD4+0.46*P_HAD3 + 0.61*P_HAD2 + P_HAD1 + 0.1*(P_EM1+P_EM2+P_EM3+P_EM4+P_EM5)");
	trV->SetAlias("M_NpeakT", "(0.58*M_HAD4+0.46*M_HAD3 + 0.72*M_HAD2 + M_HAD1 + 0.1*(M_EM1+M_EM2+M_EM3+M_EM4+M_EM5))");

	TCut P1nT = "P_NpeakT>2000 && P_NpeakT<12000";
	TCut P2nT = "P_NpeakT>12000 && P_NpeakT<18000";
	TCut M1nT = "M_NpeakT>5000 && M_NpeakT<18000";
	TCut M2nT = "M_NpeakT>18000 && M_NpeakT<30000";

//	trV->Draw("P_NpeakT>>hHad", P_select);
//	trV->Draw("M_NpeakT>>hHadR", M_select);
//
//	TH1D * hP1 = new TH1D("hP1", "hP1", 1000,0,20000);
//	TH1D * hP2 = new TH1D("hP2", "hP2", 1000,0,20000);
//	TH1D * hM1 = new TH1D("hM1", "hM1", 1000,0,20000);
//	TH1D * hM2 = new TH1D("hM2", "hM2", 1000,0,20000);
//
//	TH1D * h2P1 = new TH1D("h2P1", "h2P1", 1000,0,20000);
//	TH1D * h2P2 = new TH1D("h2P2", "h2P2", 1000,0,20000);
//	TH1D * h2M1 = new TH1D("h2M1", "h2M1", 1000,0,20000);
//	TH1D * h2M2 = new TH1D("h2M2", "h2M2", 1000,0,20000);

//	trV->Draw("hZDCP_HAD1[4]>>hP1(1000,0,20000)", P_select&&P1nT);
//	trV->Draw("hZDCP_HAD2[4]>>hP2(1000,0,20000)", P_select&&P1nT);
//
//	trV->Draw("hZDCM_HAD1[4]>>hM1(1000,0,20000)", M_select&&M1nT);
//	trV->Draw("hZDCM_HAD2[4]>>hM2(1000,0,20000)", M_select&&M1nT);
//
//	trV->Draw("hZDCP_HAD1[4]>>h2P1(1000,0,20000)", P_select&&P1nT);
//	trV->Draw("hZDCP_HAD2[4]>>h2P2(1000,0,20000)", P_select&&P1nT);
//
//	trV->Draw("hZDCM_HAD1[4]>>h2M1(1000,0,20000)", M_select&&M1nT);
//	trV->Draw("hZDCM_HAD2[4]>>h2M2(1000,0,20000)", M_select&&M1nT);
//
//	TFile * fsave = new TFile("test.root", "recreate");
//	hHad->Write();
//	hHadR->Write();
//	hP1->Write();
//	hP2->Write();
//	hM1->Write();
//	hM2->Write();
//
//	h2P1->Write();
//	h2P2->Write();
//	h2M1->Write();
//	h2M2->Write();

//	trV->Draw("P_NpeakT>>hHad", P_select, "Goff");
//	hHad->Write("P_NpeakT");
//	trV->Draw("M_NpeakT>>hHad", M_select, "Goff");
//	hHad->Write("M_NpeakT");
//
//	trV->Draw("P_EM1>>hEM", P_select&&P1nT, "Goff");
//	hEM->Write("hP_EM1");
//
//	trV->Draw("P_EM2>>hEM", P_select&&P1nT, "Goff");
//	hEM->Write("hP_EM2");
//
//	trV->Draw("P_EM3>>hEM", P_select&&P1nT, "Goff");
//	hEM->Write("hP_EM3");
//
//	trV->Draw("P_EM4>>hEM", P_select&&P1nT, "Goff");
//	hEM->Write("hP_EM4");
//
//	trV->Draw("P_EM5>>hEM", P_select&&P1nT, "Goff");
//	hEM->Write("hP_EM5");
//
//	trV->Draw("M_EM1>>hEM", M_select&&M1nT, "Goff");
//	hEM->Write("hM_EM1");
//
//	trV->Draw("M_EM2>>hEM", M_select&&M1nT, "Goff");
//	hEM->Write("hM_EM2");
//
//	trV->Draw("M_EM3>>hEM", M_select&&M1nT, "Goff");
//	hEM->Write("hM_EM3");
//
//	trV->Draw("M_EM4>>hEM", M_select&&M1nT, "Goff");
//	hEM->Write("hM_EM4");
//
//	trV->Draw("M_EM5>>hEM", M_select&&M1nT, "Goff");
//	hEM->Write("hM_EM5");
}
