
void ZDCEM1N_Ana()
{
	TFile * f = new TFile("ZDC1N_run327560.root");

	TH1D * hEM1toHadAP = (TH1D*) f->Get("HadA/hEM1toHadAP");
	TH1D * hEM2toHadAP = (TH1D*) f->Get("HadA/hEM2toHadAP");
	TH1D * hEM3toHadAP = (TH1D*) f->Get("HadA/hEM3toHadAP");
	TH1D * hEM4toHadAP = (TH1D*) f->Get("HadA/hEM4toHadAP");
	TH1D * hEM5toHadAP = (TH1D*) f->Get("HadA/hEM5toHadAP");

	TH1D * hEM1toHadAM = (TH1D*) f->Get("HadA/hEM1toHadAM");
	TH1D * hEM2toHadAM = (TH1D*) f->Get("HadA/hEM2toHadAM");
	TH1D * hEM3toHadAM = (TH1D*) f->Get("HadA/hEM3toHadAM");
	TH1D * hEM4toHadAM = (TH1D*) f->Get("HadA/hEM4toHadAM");
	TH1D * hEM5toHadAM = (TH1D*) f->Get("HadA/hEM5toHadAM");

	TH1D * hratio[10] = {
		hEM1toHadAP,
		hEM2toHadAP,
		hEM3toHadAP,
		hEM4toHadAP,
		hEM5toHadAP,
		hEM1toHadAM,
		hEM2toHadAM,
		hEM3toHadAM,
		hEM4toHadAM,
		hEM5toHadAM
	};
	TCanvas c;
	for ( int i = 0; i < 10; i++ ) {
		TF1 * f1 = new TF1("f1", "gaus", 0.02, 0.05);
		hratio[i]->GetXaxis()->SetRangeUser(0., 0.2);
		hratio[i]->Fit(f1, "Q", "", 0.02, 0.07);
		cout << hratio[i]->GetName() << "\tmean = " << f1->GetParameter(0) << "\tsigma = " << f1->GetParameter(1) << endl;

		hratio[i]->Draw();
		c.SaveAs(Form("%s.pdf", hratio[i]->GetName()));
//		hratio[i]->Rebin(5);
//		int bin = hratio[i]->GetMaximumBin();
//		double r = ( hratio[i]->GetBinCenter(bin-2) * hratio[i]->GetBinContent(bin-2) + 
//			hratio[i]->GetBinCenter(bin-1) * hratio[i]->GetBinContent(bin-1) +
//			hratio[i]->GetBinCenter(bin-0) * hratio[i]->GetBinContent(bin-0) +
//			hratio[i]->GetBinCenter(bin+1) * hratio[i]->GetBinContent(bin+1) +
//			hratio[i]->GetBinCenter(bin+2) * hratio[i]->GetBinContent(bin+2) ) / (
//			hratio[i]->GetBinContent(bin-2) +
//			hratio[i]->GetBinContent(bin-1) +
//			hratio[i]->GetBinContent(bin-0) +
//			hratio[i]->GetBinContent(bin+1) +
//			hratio[i]->GetBinContent(bin+2) );
//		cout << hratio[i]->GetName() << "\tbin = " << bin << "\t r = " << r << endl;
	}
}
