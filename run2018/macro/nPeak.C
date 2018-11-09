{
	TChain * trV = new TChain("trV");
	/*
	trV->AddFile("zdc_326237_normedMB1Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326237_normedMB2Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326237_normedMB3Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326237_normedMB4Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326237_normedMB5Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326237_normedMB6Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326237_normedMB7Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326237_normedMB8Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326237_normedMB9Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326237_normedMB0Tree.root/zdcana/fC/trV");

	trV->AddFile("zdc_326238_normedMB0Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326238_normedMB1Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326238_normedMB2Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326238_normedMB3Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326238_normedMB4Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326238_normedMB5Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326238_normedMB6Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326238_normedMB7Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326238_normedMB8Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326238_normedMB9Tree.root/zdcana/fC/trV");
	*/
	trV->AddFile("zdc_326381_normedMB0Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326381_normedMB1Tree.root/zdcana/fC/trV");

	trV->AddFile("zdc_326382_normedMB0Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326382_normedMB1Tree.root/zdcana/fC/trV");

	trV->AddFile("zdc_326383_normedMB0Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326383_normedMB1Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326383_normedMB2Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326383_normedMB3Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326383_normedMB4Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326383_normedMB5Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326383_normedMB6Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326383_normedMB7Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326383_normedMB8Tree.root/zdcana/fC/trV");
	trV->AddFile("zdc_326383_normedMB9Tree.root/zdcana/fC/trV");


	TH1D * hHad = new TH1D("hHad", "hHad", 1500,0, 1500000);


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

	// 1. 0.61, 0.3, 0.26
	// n-peak - M side
	trV->SetAlias("M_NpeakOrg", "0.1*(M_EM1+M_EM2+M_EM3+M_EM4+M_EM5)+0.65*M_HAD4+0.6*M_HAD3+0.6*M_HAD2+M_HAD1");

	trV->SetAlias("M_Npeak_NOEM", "0.65*(hZDCM_HAD4[4]+hZDCM_HAD4[5]+hZDCM_HAD4[6])+0.6*(hZDCM_HAD3[4]+hZDCM_HAD3[5]+hZDCM_HAD3[6])+0.6*(hZDCM_HAD2[4]+hZDCM_HAD2[5]+hZDCM_HAD2[6])+(hZDCM_HAD1[4]+hZDCM_HAD1[5]+hZDCM_HAD1[6])");
	TCut M_select = "(hZDCM_HAD1[4]>500 || hZDCM_HAD1[5]>500 || hZDCM_HAD1[6]>500) && (hZDCM_HAD2[4]>500 || hZDCM_HAD2[5]>500 || hZDCM_HAD2[6]>500) && (hZDCM_HAD1[4]==Max$(hZDCM_HAD1) || hZDCM_HAD1[5]==Max$(hZDCM_HAD1)) && (hZDCM_HAD2[4]==Max$(hZDCM_HAD2) || hZDCM_HAD2[5]==Max$(hZDCM_HAD2))";

//	trV->Draw("M_NpeakOrg>>hHad", M_select);

	TCut Mcut1n = "M_NpeakOrg<40000";
	TCut cutMEM1_peak = "hZDCM_EM1[4]==Max$(hZDCM_EM1) || hZDCM_EM1[5]==Max$(hZDCM_EM1)";
	TCut cutMEM2_peak = "hZDCM_EM2[4]==Max$(hZDCM_EM2) || hZDCM_EM2[5]==Max$(hZDCM_EM2)";
	TCut cutMEM3_peak = "hZDCM_EM3[4]==Max$(hZDCM_EM3) || hZDCM_EM3[5]==Max$(hZDCM_EM3)";
	TCut cutMEM4_peak = "hZDCM_EM4[4]==Max$(hZDCM_EM4) || hZDCM_EM4[5]==Max$(hZDCM_EM4)";
	TCut cutMEM5_peak = "hZDCM_EM5[4]==Max$(hZDCM_EM5) || hZDCM_EM5[5]==Max$(hZDCM_EM5)";

//	trV->Draw("M_EM1>>hHad", Mcut1n); // mean 3079   , 0.78
//	trV->Draw("M_EM2>>hHad", Mcut1n); // mean 2960   , 0.75
//	trV->Draw("M_EM3>>hHad", Mcut1n); // mean 3949   , 1.
//	trV->Draw("M_EM4>>hHad", Mcut1n); // mean 5113   , 1.29
//	trV->Draw("M_EM5>>hHad", Mcut1n); // mean 8759   , 2.22
//	trV->Draw("M_EM3/M_HAD1>>(1000,1,10)", M_select);
//	trV->Draw("M_Npeak_NOEM>>hHad", M_select);

	trV->SetAlias("M_NpeakOpt", "M_Npeak_NOEM + 0.1*(M_EM1/0.78 + M_EM2/0.75 + M_EM3 + M_EM4/1.29 + M_EM5/2.2)");
//	trV->Draw("M_NpeakOpt>>hHad", M_select);

	// P side
	trV->SetAlias("P_Npeak_NOEM", "1.3*P_HAD4+1.2*P_HAD3+1.22*P_HAD2+P_HAD1");
	trV->SetAlias("P_NpeakOrg", "0.1*(P_EM1+P_EM2+P_EM3+P_EM4+P_EM5)+P_Npeak_NOEM");
	TCut P_select = "(hZDCP_HAD1[4]>500 || hZDCP_HAD1[5]>500 || hZDCP_HAD1[6]>500) && (hZDCP_HAD2[4]>500 || hZDCP_HAD2[5]>500 || hZDCP_HAD2[6]>500) && (hZDCP_HAD1[4]==Max$(hZDCP_HAD1) || hZDCP_HAD1[5]==Max$(hZDCP_HAD1)) && (hZDCP_HAD2[4]==Max$(hZDCP_HAD2) || hZDCP_HAD2[5]==Max$(hZDCP_HAD2))";

	TCut Pcut1n = "P_NpeakOrg<32000";

//	trV->Draw("P_EM1>>hHad",  Pcut1n); // mean 1192   , 0.42
//	trV->Draw("P_EM2>>hHad",  Pcut1n); // mean 1906   , 0.67
//	trV->Draw("P_EM3>>hHad",  Pcut1n); // mean 2832   , 1.
//	trV->Draw("P_EM4>>hHad",  Pcut1n); // mean 4012   , 1.42
//	trV->Draw("P_EM5>>hHad",  Pcut1n); // mean 3247   , 1.15

	trV->SetAlias("P_NpeakOpt", "0.1*1.27*(P_EM1/0.42+P_EM2/0.67+P_EM3+P_EM4/1.42+P_EM5/1.15)+P_Npeak_NOEM");

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
//	trV->Draw("M_HAD1/M_EM1>>(1000, 0, 10)", M1nT);

//	trV->Draw("0.81*P_HAD4+0.67*P_HAD3 + 0.81*P_HAD2 + P_HAD1 + 0.1*(P_EM1+P_EM2+P_EM3+P_EM4+P_EM5)", P_select);
//	trV->SetAlias("P_NpeakT", "0.81*P_HAD4+0.67*P_HAD3 + 0.81*P_HAD2 + P_HAD1 + 0.1*(P_EM1+P_EM2+P_EM3+P_EM4+P_EM5)");
//	trV->SetAlias("M_NpeakT", "0.87*M_HAD4+0.67*M_HAD3 + 0.62*M_HAD2 + M_HAD1 + 0.1*(M_EM1+M_EM2+M_EM3+M_EM4+M_EM5)");
//
//	TCut M1nT = "M_NpeakT>10000 && M_NpeakT<29000";
//	TCut P1nT = "P_NpeakT>8000 && P_NpeakT<18000";

	trV->SetAlias("M_NpeakOpt", "0.87*M_HAD4+0.67*M_HAD3 + 0.62*M_HAD2 + M_HAD1 + 0.1*(M_EM1+M_EM2+M_EM3+0.8*M_EM4+0.6*M_EM5)");

	trV->SetAlias("P_NpeakOpt", "1.5*(0.81*P_HAD4+0.67*P_HAD3 + 0.81*P_HAD2 + P_HAD1 + 0.1*(P_EM1+P_EM2+0.7*P_EM3+0.8*P_EM4+0.6*P_EM5))");




}
