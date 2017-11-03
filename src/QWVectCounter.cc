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

class QWVectCounter : public edm::EDProducer {
public:
	explicit QWVectCounter(const edm::ParameterSet&);
	~QWVectCounter();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	edm::InputTag	src_;
};

QWVectCounter::QWVectCounter(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src"))
{
	consumes<std::vector<double> >(src_);
	produces<int>();
}

QWVectCounter::~QWVectCounter() {
	return;
}

void QWVectCounter::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;

	Handle<std::vector<double> > vect;
	iEvent.getByLabel(src_, vect);
	int Noff = int(vect->size());
	iEvent.put(std::make_unique<int>(Noff));
}

DEFINE_FWK_MODULE(QWVectCounter);
