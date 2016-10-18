#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1D.h"

class QWVectorAnalyzer : public edm::EDAnalyzer {
public:
	explicit QWVectorAnalyzer(const edm::ParameterSet&);
	~QWVectorAnalyzer() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob() {};

	edm::InputTag   src_;
	TH1D * h;
	TH1D * hc;
};

QWVectorAnalyzer::QWVectorAnalyzer(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src"))
{
	consumes<std::vector<double> >(src_);
	int hNbins = pset.getUntrackedParameter<int>("hNbins");
	double hstart = pset.getUntrackedParameter<double>("hstart");
	double hend = pset.getUntrackedParameter<double>("hend");

	int cNbins = pset.getUntrackedParameter<int>("cNbins");
	double cstart = pset.getUntrackedParameter<double>("cstart");
	double cend = pset.getUntrackedParameter<double>("cend");

	edm::Service<TFileService> fs;
	h = fs->make<TH1D>("h", "h", hNbins, hstart, hend);
	h->Sumw2();
	hc = fs->make<TH1D>("hc", "hc", cNbins, cstart, cend);
	hc->Sumw2();
}

void
QWVectorAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle<std::vector<double> > t;
	iEvent.getByLabel(src_, t);
	int sz = t->size();
	h->Fill(sz);
	for ( int i = 0; i < sz; i++ ) {
		hc->Fill((*t)[i]);
	}
	return;
}

DEFINE_FWK_MODULE(QWVectorAnalyzer);
