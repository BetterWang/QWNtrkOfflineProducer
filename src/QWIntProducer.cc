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
	int	src_;
};

QWIntProducer::QWIntProducer(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<int>("src"))
{
	produces<int>();
}

QWIntProducer::~QWIntProducer() {
	return;
}

void QWIntProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	std::unique_ptr<int> pint(new int(src_));
	iEvent.put(move(pint));
}

DEFINE_FWK_MODULE(QWIntProducer);
