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

class QWDouble2VectorProducer : public edm::EDProducer {
public:
	explicit QWDouble2VectorProducer(const edm::ParameterSet&);
	~QWDouble2VectorProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	edm::InputTag	src_;
};

QWDouble2VectorProducer::QWDouble2VectorProducer(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src"))
{
	consumes<double>(src_);
	produces< std::vector<double> >();
}

QWDouble2VectorProducer::~QWDouble2VectorProducer() {
	return;
}

void QWDouble2VectorProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;

	Handle<double> db;
	iEvent.getByLabel(src_, db);

	std::vector<double> p;
	p.push_back(*db);
	//std::auto_ptr<std::vector<double> > pp(&p);
	iEvent.put(std::make_unique<std::vector<double> >(p));
}

DEFINE_FWK_MODULE(QWDouble2VectorProducer);
