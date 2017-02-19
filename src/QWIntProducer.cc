#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

class QWIntProducer : public edm::EDProducer {
public:
	explicit QWIntProducer(const edm::ParameterSet&);
	~QWIntProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	int	input_;
};

QWIntProducer::QWIntProducer(const edm::ParameterSet& pset) :
	input_(pset.getUntrackedParameter<int>("input"))
{
	produces<int>();
}

QWIntProducer::~QWIntProducer() {
	return;
}

void QWIntProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	std::auto_ptr<int> pint(new int(input_));
	iEvent.put(pint);
}

DEFINE_FWK_MODULE(QWIntProducer);
