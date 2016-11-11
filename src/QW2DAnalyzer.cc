#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"
#include <iostream>
#include <tuple>

class QW2DAnalyzer : public edm::EDAnalyzer {
public:
	explicit QW2DAnalyzer(const edm::ParameterSet&);
	~QW2DAnalyzer() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob() {};

	edm::InputTag   srcPhi_;
	edm::InputTag   srcEta_;
	edm::InputTag   srcW_;
	TH1D * h;
	TH2D * hc;
	TH2D * hm;
	bool	bWeight;

	std::vector<double>	mphi_;
	std::vector<double>	meta_;
	std::vector<double>	mw_;

	const double DphiMax = TMath::Pi();
	const double DphiMin = -TMath::Pi();
};

QW2DAnalyzer::QW2DAnalyzer(const edm::ParameterSet& pset) :
	srcPhi_(pset.getUntrackedParameter<edm::InputTag>("srcPhi")),
	srcEta_(pset.getUntrackedParameter<edm::InputTag>("srcEta")),
	srcW_(pset.getUntrackedParameter<edm::InputTag>("srcW", std::string("NA")))
{
	consumes<std::vector<double> >(srcPhi_);
	consumes<std::vector<double> >(srcEta_);
	bWeight = false;

	if ( srcW_.label() != std::string("NA") ) {
		consumes<std::vector<double> >(srcW_);
		bWeight = true;
	}
	int hNbins = pset.getUntrackedParameter<int>("hNbins");
	double hstart = pset.getUntrackedParameter<double>("hstart");
	double hend = pset.getUntrackedParameter<double>("hend");

	edm::Service<TFileService> fs;
	h = fs->make<TH1D>("h", "h", hNbins, hstart, hend);
	h->Sumw2();
	hc = fs->make<TH2D>("hc", "hc", 24, DphiMin, DphiMax, 48, -4.8, 4.8);
	hc->Sumw2();
	hm = fs->make<TH2D>("hm", "hm", 24, DphiMin, DphiMax, 48, -4.8, 4.8);
	hm->Sumw2();
}

void
QW2DAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle<std::vector<double> > phi;
	Handle<std::vector<double> > eta;
	Handle<std::vector<double> > w;

	iEvent.getByLabel(srcPhi_, phi);
	iEvent.getByLabel(srcEta_, eta);
	if ( bWeight ) {
		iEvent.getByLabel(srcW_, w);
	}

	int sz = phi->size();
	h->Fill(sz);
	for ( int i = 0; i < sz; i++ ) {
		for ( int j = 0; j < sz; j++ ) {
			if ( i == j ) continue;
			double Dphi = (*phi)[i] - (*phi)[j];
			double Deta = (*eta)[i] - (*eta)[j];
			while (Dphi > DphiMax) Dphi -= TMath::Pi()*2.;
			while (Dphi < DphiMin) Dphi += TMath::Pi()*2.;

			if ( bWeight ) {
				hc->Fill(Dphi, Deta, (*w)[i] * (*w)[j]);
			} else {
				hc->Fill(Dphi, Deta);
			}
		}
		// do mix

		for ( int j = 0; j < int(mphi_.size()); j++ ) {
			double Dphi = (*phi)[i] - (mphi_)[j];
			double Deta = (*eta)[i] - (meta_)[j];
			while (Dphi > DphiMax) Dphi -= TMath::Pi()*2.;
			while (Dphi < DphiMin) Dphi += TMath::Pi()*2.;

			if ( bWeight ) {
				hm->Fill(Dphi, Deta, (*w)[i] * (mw_)[j]);
			} else {
				hm->Fill(Dphi, Deta);
			}
		}
	}
	mphi_ = *phi;
	meta_ = *eta;
	mw_ = *w;
	return;
}

DEFINE_FWK_MODULE(QW2DAnalyzer);
