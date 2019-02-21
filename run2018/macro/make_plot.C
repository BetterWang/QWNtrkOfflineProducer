
void make_plot(int c = 3, string tag = "All")
{
	TFile * f = new TFile("ZDC_run327560.root");
	TH1D * hPRPD[16];
	TH1D * hMRPD[16];
	TCanvas * c1 = new TCanvas("c1", "c1", 800, 600);
	c1->SetLogy();

	for ( int i = 0; i < 16; i++ ) {
		hPRPD[i] = (TH1D*) f->Get(Form("RPD/hP%sRPD%i_%i", tag.c_str(), i, c));
		hMRPD[i] = (TH1D*) f->Get(Form("RPD/hM%sRPD%i_%i", tag.c_str(), i, c));
		hPRPD[i]->Rebin(10);
		hMRPD[i]->Rebin(10);

		hPRPD[i]->Draw();
		c1->SaveAs(Form("hP%sRPD%i_%i.pdf", tag.c_str(), i, c));
		hMRPD[i]->Draw();
		c1->SaveAs(Form("hM%sRPD%i_%i.pdf", tag.c_str(), i, c));

		hPRPD[i]->SetLineColor(kRed);
		hMRPD[i]->SetLineColor(kBlue);
	}
	pair<int,int> p[16] = {
		make_pair( 7, 10), // 0
		make_pair( 3, 14), // 1
		make_pair(14,  3), // 2
		make_pair(10,  7), // 3
		make_pair( 4,  9), // 4
		make_pair( 0, 13), // 5
		make_pair(13,  0), // 6
		make_pair( 9,  4), // 7
		make_pair( 6,  8), // 8
		make_pair( 1, 12), // 9
		make_pair(12,  1), // 10
		make_pair( 8,  5), // 11
		make_pair( 5, 11), // 12
		make_pair( 2, 15), // 13
		make_pair(15,  2), // 14
		make_pair(11,  6)  // 15
	};
	pair<int,int> cord[16] = {
		make_pair( 0,  0), // 0
		make_pair( 1,  0), // 1
		make_pair( 2,  0), // 2
		make_pair( 3,  0), // 3
		make_pair( 0,  1), // 4
		make_pair( 1,  1), // 5
		make_pair( 2,  1), // 6
		make_pair( 3,  1), // 7
		make_pair( 0,  2), // 8
		make_pair( 1,  2), // 9
		make_pair( 2,  2), // 10
		make_pair( 3,  2), // 11
		make_pair( 0,  3), // 12
		make_pair( 1,  3), // 13
		make_pair( 2,  3), // 14
		make_pair( 3,  3)  // 15
	};

	for ( int i = 0; i < 16; i++ ) {
		hPRPD[p[i].first]->SetTitle(Form("Pside:%i (X=%i), Mside:%i (Y=%i)", p[i].first, cord[i].first, p[i].second, cord[i].second));
		hPRPD[p[i].first] ->Draw();
		hMRPD[p[i].second]->Draw("same");
		c1->SaveAs(Form("%sRPD_%i_%i_%i.pdf", tag.c_str(), cord[i].first, cord[i].second, c));
	}
}
