#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HeavyIonEvent/interface/Centrality.h"

#include "TFile.h"
#include "TH2.h"
#include "TMath.h"

using namespace std;
class QWCentralityProducer : public edm::EDProducer {
public:
	explicit QWCentralityProducer(const edm::ParameterSet&);
	~QWCentralityProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///
	edm::InputTag	src_;
};

QWCentralityProducer::QWCentralityProducer(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src"))
{
	consumes<reco::Centrality>(src_);

	produces< double >("EtHFtowerSum");
	produces< double >("EtHFtowerSumPlus");
	produces< double >("EtHFtowerSumMinus");
}

QWCentralityProducer::~QWCentralityProducer()
{
	return;
}

void QWCentralityProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;
	edm::Handle<reco::Centrality> cent;
	iEvent.getByLabel(src_, cent);

	double EtHFtowerSum = cent->EtHFtowerSum();
	double EtHFtowerSumPlus = cent->EtHFtowerSumPlus();
	double EtHFtowerSumMinus = cent->EtHFtowerSumMinus();

	iEvent.put(std::make_unique<double> (EtHFtowerSum), std::string("EtHFtowerSum"));
	iEvent.put(std::make_unique<double> (EtHFtowerSumPlus), std::string("EtHFtowerSumPlus"));
	iEvent.put(std::make_unique<double> (EtHFtowerSumMinus), std::string("EtHFtowerSumMinus"));
}


DEFINE_FWK_MODULE(QWCentralityProducer);
