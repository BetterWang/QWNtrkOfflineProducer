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

class QWNG2DAnalyzer : public edm::EDAnalyzer {
public:
	explicit QWNG2DAnalyzer(const edm::ParameterSet&);
	~QWNG2DAnalyzer() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob() {};

	edm::InputTag   srcPhi_;
	edm::InputTag   srcPt_;
	edm::InputTag   srcEta_;
	edm::InputTag   srcW_;
	TH1D * h;
	TH2D * hc;
	TH2D * hm;
	bool	bWeight;

	std::vector<double>	mphi_;
	std::vector<double>	mpt_;
	std::vector<double>	meta_;
	std::vector<double>	mw_;

	double trigPtMin_;
	double trigPtMax_;
	double assoPtMin_;
	double assoPtMax_;

	const double DphiMax = 3*TMath::Pi()/2;
	const double DphiMin = -TMath::Pi()/2;
};

QWNG2DAnalyzer::QWNG2DAnalyzer(const edm::ParameterSet& pset) :
	srcPhi_(pset.getUntrackedParameter<edm::InputTag>("srcPhi")),
	srcPt_(pset.getUntrackedParameter<edm::InputTag>("srcPt")),
	srcEta_(pset.getUntrackedParameter<edm::InputTag>("srcEta")),
	srcW_(pset.getUntrackedParameter<edm::InputTag>("srcW", std::string("NA")))
{
	consumes<std::vector<double> >(srcPhi_);
	consumes<std::vector<double> >(srcPt_);
	consumes<std::vector<double> >(srcEta_);
	bWeight = false;

	if ( srcW_.label() != std::string("NA") ) {
		consumes<std::vector<double> >(srcW_);
		bWeight = true;
	}
	int hNbins = pset.getUntrackedParameter<int>("hNbins");
	double hstart = pset.getUntrackedParameter<double>("hstart");
	double hend = pset.getUntrackedParameter<double>("hend");

	trigPtMin_ = pset.getUntrackedParameter<double>("trigPtMin");
	trigPtMax_ = pset.getUntrackedParameter<double>("trigPtMax");
	assoPtMin_ = pset.getUntrackedParameter<double>("assoPtMin");
	assoPtMax_ = pset.getUntrackedParameter<double>("assoPtMax");

	edm::Service<TFileService> fs;
	h = fs->make<TH1D>("h", "h", hNbins, hstart, hend);
	h->Sumw2();
	hc = fs->make<TH2D>("hc", "hc", 24, DphiMin, DphiMax, 48, -4.8, 4.8);
	hc->Sumw2();
	hm = fs->make<TH2D>("hm", "hm", 24, DphiMin, DphiMax, 48, -4.8, 4.8);
	hm->Sumw2();
}

void
QWNG2DAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle<std::vector<double> > phi;
	Handle<std::vector<double> > pt;
	Handle<std::vector<double> > eta;
	Handle<std::vector<double> > w;

	iEvent.getByLabel(srcPhi_, phi);
	iEvent.getByLabel(srcPt_, pt);
	iEvent.getByLabel(srcEta_, eta);
	if ( bWeight ) {
		iEvent.getByLabel(srcW_, w);
	}

	int sz = phi->size();
	h->Fill(sz);
	for ( int i = 0; i < sz; i++ ) {
		if ( (*pt)[i] > trigPtMax_ or (*pt)[i] < trigPtMin_ ) continue;
		for ( int j = 0; j < sz; j++ ) {
			if ( i == j ) continue;
			if ( (*pt)[j] > assoPtMax_ or (*pt)[j] < assoPtMin_ ) continue;
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
			if ( (mpt_)[j] > assoPtMax_ or (mpt_)[j] < assoPtMin_ ) continue;
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
	mpt_ = *pt;
	meta_ = *eta;
	mw_ = *w;
	return;
}

DEFINE_FWK_MODULE(QWNG2DAnalyzer);
