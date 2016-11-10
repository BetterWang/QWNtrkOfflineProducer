#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1D.h"
#include "TMath.h"
#include <iostream>

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
	bool	bWeight;


	const double DphiMax = 2*TMath::Pi() - 2.0;
	const double DphiMin = - 2.0;
};

QW2DAnalyzer::QW2DAnalyzer(const edm::ParameterSet& pset) :
	srcPhi_(pset.getUntrackedParameter<edm::InputTag>("srcPhi")),
	srcEta_(pset.getUntrackedParameter<edm::InputTag>("srcEta")),
	srcW_(pset.getUntrackedParameter<edm::InputTag>("srcW", std::string("NA")))
{
	consumes<std::vector<double> >(src_);
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

	int sz = t->size();
	h->Fill(sz);
	for ( int i = 0; i < sz; i++ ) {
		for ( int j = i+1; j < sz; j++ ) {
			double Dphi = (*phi)[i] - (*phi)[j];
			double Deta = (*eta)[i] - (*eta)[j];
			while (Dphi > DphiMax) Dphi -= TMath::Pi()*2.;
			while (Dphi < DphiMax) Dphi += TMath::Pi()*2.;
			if ( bWeight ) {
				hc->Fill(Dphi, Deta, (*w)[i] * (*w)[j]);
			} else {
				hc->Fill(Dphi, Deta);
			}
		}
	}
	return;
}

DEFINE_FWK_MODULE(QW2DAnalyzer);
