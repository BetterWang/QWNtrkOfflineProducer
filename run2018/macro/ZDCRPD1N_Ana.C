
void ZDCRPD1N_Ana()
{
	TFile * f = new TFile("ZDC1N_run327560.root");

	TH1D * hRPD1NP[16];
	TH1D * hRPD1NM[16];

	for ( int i = 0; i < 16; i++ ) {
		hRPD1NP[i] = (TH1D*) f->Get(Form("RPD/hRPDP_%i", i));
		hRPD1NM[i] = (TH1D*) f->Get(Form("RPD/hRPDM_%i", i));
	}

	TCanvas c;
	c.SetLogy();
	for ( int i = 0; i < 16; i++ ) {
		hRPD1NP[i]->Rebin(10);
		hRPD1NP[i]->GetXaxis()->SetRangeUser(0, 5000);
		hRPD1NM[i]->Rebin(10);
		hRPD1NM[i]->GetXaxis()->SetRangeUser(0, 5000);

		hRPD1NP[i]->Draw();
		c.SaveAs(Form("RPD1NP_%i.pdf", i));
		hRPD1NM[i]->Draw();
		c.SaveAs(Form("RPD1NM_%i.pdf", i));
	}
}
