#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HeavyIonEvent/interface/EvtPlane.h"
#include "iostream"

class QWEvtPlaneProducer : public edm::EDProducer {
public:
	explicit QWEvtPlaneProducer(const edm::ParameterSet&);
	~QWEvtPlaneProducer() {return;}
private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;

	edm::InputTag	src_;
	int	level_;
};

QWEvtPlaneProducer::QWEvtPlaneProducer(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src")),
	level_(pset.getUntrackedParameter<int>("level", 2))
{
	consumes<reco::EvtPlaneCollection>(src_);
	produces<std::vector<double> >("angle");
}

void QWEvtPlaneProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	edm::Handle<reco::EvtPlaneCollection> eps;
	iEvent.getByLabel( src_, eps );

	std::vector<double> vangle(eps->size());
	for ( auto ep : *eps ) {
		vangle.push_back(ep.angle(level_));
	}

	iEvent.put(std::make_unique<std::vector<double>>(vangle), std::string("angle"));

	return;
}

DEFINE_FWK_MODULE(QWEvtPlaneProducer);
