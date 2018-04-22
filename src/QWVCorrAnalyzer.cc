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

class QWVCorrAnalyzer : public edm::EDAnalyzer {
public:
	explicit QWVCorrAnalyzer(const edm::ParameterSet&);
	~QWVCorrAnalyzer() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob() {};

	edm::InputTag   srcX_;
	edm::InputTag   srcY_;
	edm::InputTag   srcWeight_;

	bool		bWeight_;

	TH2D * hc;
};

QWVCorrAnalyzer::QWVCorrAnalyzer(const edm::ParameterSet& pset) :
	srcX_(pset.getUntrackedParameter<edm::InputTag>("srcX")),
	srcY_(pset.getUntrackedParameter<edm::InputTag>("srcY")),
	srcWeight_(pset.getUntrackedParameter<edm::InputTag>("srcWeight", edm::InputTag("NA")))
{
	consumes<std::vector<double> >(srcX_);
	consumes<std::vector<double> >(srcY_);
	bWeight_ = false;
	if ( srcWeight_.label() != std::string("NA") ) {
		consumes<std::vector<double> >(srcWeight_);
		bWeight_ = true;
	}

	int NbinsX = pset.getUntrackedParameter<int>("NbinsX");
	int NbinsY = pset.getUntrackedParameter<int>("NbinsY");
	double hstartX = pset.getUntrackedParameter<double>("hstartX");
	double hendX = pset.getUntrackedParameter<double>("hendX");
	double hstartY = pset.getUntrackedParameter<double>("hstartY");
	double hendY = pset.getUntrackedParameter<double>("hendY");

	edm::Service<TFileService> fs;
	hc = fs->make<TH2D>("hc", "hc", NbinsX, hstartX, hendX, NbinsY, hstartY, hendY);
	hc->Sumw2();
}

void
QWVCorrAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle<std::vector<double> > pX;
	Handle<std::vector<double> > pY;
	Handle<std::vector<double> > pWeight;

	iEvent.getByLabel(srcX_, pX);
	iEvent.getByLabel(srcY_, pY);
	if ( bWeight_ ) {
		iEvent.getByLabel(srcWeight_, pWeight);
	}

	for ( unsigned int i = 0; i < pX->size(); i++ ) {
		if ( bWeight_ ) {
			hc->Fill( (*pX)[i], (*pY)[i], (*pWeight)[i] );
		} else {
			hc->Fill( (*pX)[i], (*pY)[i] );
		}
	}
	return;
}

DEFINE_FWK_MODULE(QWVCorrAnalyzer);
