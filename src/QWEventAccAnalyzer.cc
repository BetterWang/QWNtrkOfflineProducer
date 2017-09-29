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

class QWEventAccAnalyzer : public edm::EDAnalyzer {
public:
	explicit QWEventAccAnalyzer(const edm::ParameterSet&);
	~QWEventAccAnalyzer() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob() {};

	edm::InputTag   srcPhi_;
	edm::InputTag   srcEta_;
	edm::InputTag   srcPt_;
	edm::InputTag   srcWeight_;

	double minPt_;
	double maxPt_;

	TH2D * hc;
	TH2D * hw;
};

QWEventAccAnalyzer::QWEventAccAnalyzer(const edm::ParameterSet& pset) :
	srcPhi_(pset.getUntrackedParameter<edm::InputTag>("srcPhi")),
	srcEta_(pset.getUntrackedParameter<edm::InputTag>("srcEta")),
	srcPt_(pset.getUntrackedParameter<edm::InputTag>("srcPt")),
	srcWeight_(pset.getUntrackedParameter<edm::InputTag>("srcWeight"))
{
	consumes<std::vector<double> >(srcPhi_);
	consumes<std::vector<double> >(srcEta_);
	consumes<std::vector<double> >(srcPt_);
	consumes<std::vector<double> >(srcWeight_);

	int NbinsPhi = pset.getUntrackedParameter<int>("NbinsPhi", 24);
	int NbinsEta = pset.getUntrackedParameter<int>("NbinsEta", 24);
	double hstartPhi = pset.getUntrackedParameter<double>("hstartPhi", -3.14159265358979323846);
	double hendPhi = pset.getUntrackedParameter<double>("hendPhi", 3.14159265358979323846);
	double hstartEta = pset.getUntrackedParameter<double>("hstartEta", -2.4);
	double hendEta = pset.getUntrackedParameter<double>("hendEta", 2.4);

	minPt_ = pset.getUntrackedParameter<double>("minPt", std::numeric_limits<double>::min());
	maxPt_ = pset.getUntrackedParameter<double>("maxPt", std::numeric_limits<double>::max());

	edm::Service<TFileService> fs;
	hc = fs->make<TH2D>("hc", "hc", NbinsPhi, hstartPhi, hendPhi, NbinsEta, hstartEta, hendEta);
	hw = fs->make<TH2D>("hw", "hw", NbinsPhi, hstartPhi, hendPhi, NbinsEta, hstartEta, hendEta);
	hc->Sumw2();
	hw->Sumw2();
}

void
QWEventAccAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle<std::vector<double> > pPhi;
	Handle<std::vector<double> > pEta;
	Handle<std::vector<double> > pPt;
	Handle<std::vector<double> > pWeight;

	iEvent.getByLabel(srcPhi_, pPhi);
	iEvent.getByLabel(srcEta_, pEta);
	iEvent.getByLabel(srcPt_, pPt);
	iEvent.getByLabel(srcWeight_, pWeight);

	int szPt = pPt->size();
	int szPhi = pPhi->size();
	int szEta = pEta->size();
	int szWeight = pWeight->size();
	if ( szPt != szPhi or szPt != szEta or szPt != szWeight ) return;

	for ( int i = 0; i < szPt; i++ ) {
		if ( (*pPt)[i] < minPt_ or (*pPt)[i] >= maxPt_ ) continue;

		hc->Fill( (*pPhi)[i], (*pEta)[i] );
		hw->Fill( (*pPhi)[i], (*pEta)[i], (*pWeight)[i] );
	}
	return;
}

DEFINE_FWK_MODULE(QWEventAccAnalyzer);
