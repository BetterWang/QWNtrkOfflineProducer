#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1D.h"

class QWHistDAnalyzer : public edm::EDAnalyzer {
public:
	explicit QWHistDAnalyzer(const edm::ParameterSet&);
	~QWHistDAnalyzer() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob() {};

	edm::InputTag   src_;
	TH1D * h;
};

QWHistDAnalyzer::QWHistDAnalyzer(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src"))
{
	consumes<double>(src_);

	int Nbins = pset.getUntrackedParameter<int>("Nbins");
	double start = pset.getUntrackedParameter<double>("start");
	double end = pset.getUntrackedParameter<double>("end");

	edm::Service<TFileService> fs;
	h = fs->make<TH1D>("h", "h", Nbins, start, end);
	h->Sumw2();
}

void
QWHistDAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle<double> t;
	iEvent.getByLabel(src_, t);
	double c = *(t.product());

	h->Fill(c);
	return;
}

DEFINE_FWK_MODULE(QWHistDAnalyzer);
