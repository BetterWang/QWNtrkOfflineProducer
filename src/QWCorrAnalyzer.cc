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

class QWCorrAnalyzer : public edm::EDAnalyzer {
public:
	explicit QWCorrAnalyzer(const edm::ParameterSet&);
	~QWCorrAnalyzer() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob() {};

	edm::InputTag   srcX_;
	edm::InputTag   srcY_;

	TH2D * hc;
};

QWCorrAnalyzer::QWCorrAnalyzer(const edm::ParameterSet& pset) :
	srcX_(pset.getUntrackedParameter<edm::InputTag>("srcX")),
	srcY_(pset.getUntrackedParameter<edm::InputTag>("srcY"))
{
	consumes<double>(srcX_);
	consumes<double>(srcY_);

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
QWCorrAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle<double> pX;
	Handle<double> pY;

	iEvent.getByLabel(srcX_, pX);
	iEvent.getByLabel(srcY_, pY);

	hc->Fill(*pX, *pY);
	return;
}

DEFINE_FWK_MODULE(QWCorrAnalyzer);
