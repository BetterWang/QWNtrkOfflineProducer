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

class QWEtaPhiPtAnalyzer : public edm::EDAnalyzer {
public:
	explicit QWEtaPhiPtAnalyzer(const edm::ParameterSet&);
	~QWEtaPhiPtAnalyzer() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob() {};

	edm::InputTag   srcEta_;
	edm::InputTag   srcPhi_;
	edm::InputTag   srcPt_;
	edm::InputTag   srcWeight_;

    std::vector<double> ptbins_;

	bool		bWeight_;

    std::vector<TH2D*> hc;
};

QWEtaPhiPtAnalyzer::QWEtaPhiPtAnalyzer(const edm::ParameterSet& pset) :
	srcEta_(pset.getUntrackedParameter<edm::InputTag>("srcEta")),
	srcPhi_(pset.getUntrackedParameter<edm::InputTag>("srcPhi")),
	srcPt_(pset.getUntrackedParameter<edm::InputTag>("srcPt")),
	srcWeight_(pset.getUntrackedParameter<edm::InputTag>("srcWeight", edm::InputTag("NA")))
{
    ptbins_ = pset.getUntrackedParameter< std::vector<double> >("ptBin");
	consumes<std::vector<double> >(srcEta_);
	consumes<std::vector<double> >(srcPhi_);
	consumes<std::vector<double> >(srcPt_);

	bWeight_ = false;
	if ( srcWeight_.label() != std::string("NA") ) {
		consumes<std::vector<double> >(srcWeight_);
		bWeight_ = true;
	}

	int NbinsX = pset.getUntrackedParameter<int>("NbinsPhi");
	int NbinsY = pset.getUntrackedParameter<int>("NbinsEta");
	double hstartX = -3.14159265358979323846;
	double hendX = 3.14159265358979323846;
	double hstartY = pset.getUntrackedParameter<double>("hstartEta", -2.4);
	double hendY = pset.getUntrackedParameter<double>("hendEta", 2.4);

	edm::Service<TFileService> fs;
    for ( unsigned int i = 0; i < ptbins_.size()-1; i++ ) {
        hc.push_back( fs->make<TH2D>(Form("hc_%i", i), Form("%.1f - %.1f;#phi;#eta", ptbins_[i], ptbins_[i+1]), NbinsX, hstartX, hendX, NbinsY, hstartY, hendY ) );
    }
}

void
QWEtaPhiPtAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle<std::vector<double> > pPhi;
	Handle<std::vector<double> > pEta;
	Handle<std::vector<double> > pPt;
	Handle<std::vector<double> > pWeight;

	iEvent.getByLabel(srcPhi_, pPhi);
	iEvent.getByLabel(srcEta_, pEta);
	iEvent.getByLabel(srcPt_, pPt);
	if ( bWeight_ ) {
		iEvent.getByLabel(srcWeight_, pWeight);
	}

	for ( unsigned int i = 0; i < pPt->size(); i++ ) {
        int ipt = 0;
        while ( (*pPt)[i] > ptbins_[ipt+1] ) ipt++;

		if ( bWeight_ ) {
			hc[ipt]->Fill( (*pPhi)[i], (*pEta)[i], (*pWeight)[i] );
		} else {
			hc[ipt]->Fill( (*pPhi)[i], (*pEta)[i] );
		}
	}
	return;
}

DEFINE_FWK_MODULE(QWEtaPhiPtAnalyzer);
