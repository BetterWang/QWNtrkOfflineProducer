#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1D.h"
#include <iostream>

class QWBXAnalyzer : public edm::EDAnalyzer {
public:
	explicit QWBXAnalyzer(const edm::ParameterSet&);
	~QWBXAnalyzer() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob() {};

	edm::InputTag   BX_;
	edm::InputTag   fC_;
	int		NS_;
	int		TS_;
	int		nChannel_;
	int		nSig_;
	TH1D *		hc;
	TH1D *		hh;
};

QWBXAnalyzer::QWBXAnalyzer(const edm::ParameterSet& pset) :
	BX_(pset.getUntrackedParameter<edm::InputTag>("BX")),
	fC_(pset.getUntrackedParameter<edm::InputTag>("fC")),
	NS_(pset.getUntrackedParameter<int>("NS")),
	TS_(pset.getUntrackedParameter<int>("TS")),
	nChannel_(pset.getUntrackedParameter<int>("nChannel")),
	nSig_(pset.getUntrackedParameter<int>("nSig"))
{
	consumes<double>(BX_);
	consumes<std::vector<double>>(fC_);

	edm::Service<TFileService> fs;
	hc = fs->make<TH1D>("hc", "hc", 4000, 0, 4000);
	hh = fs->make<TH1D>("hh", "hh", 4000, 0, 4000);
	hc->Sumw2();
	hh->Sumw2();
}

void
QWBXAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle<double> hBX;
	Handle<std::vector<double>> hfC;

	iEvent.getByLabel(BX_, hBX);
	iEvent.getByLabel(fC_, hfC);

	if ( TS_ < 0 ) {
		for ( int ts = 0; ts < NS_; ts++ ) {
			double q = (*hfC)[ int(nChannel_+ts) ];
			q = q>0?q:0;
			hc->Fill( (*hBX)+ts-nSig_, q );
			hh->Fill( (*hBX)+ts-nSig_ );
		}
	} else {
		double q = (*hfC)[ int(nChannel_+TS_) ];
		q = q>0?q:0;
		hc->Fill( (*hBX)+TS_, q );
		hh->Fill( (*hBX)+TS_ );
	}
	return;
}

DEFINE_FWK_MODULE(QWBXAnalyzer);
