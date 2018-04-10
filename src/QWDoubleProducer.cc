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

class QWDoubleProducer : public edm::EDProducer {
public:
	explicit QWDoubleProducer(const edm::ParameterSet&);
	~QWDoubleProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	int	src_;
};

QWDoubleProducer::QWDoubleProducer(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<double>("src"))
{
	produces<double>();
}

QWDoubleProducer::~QWDoubleProducer() {
	return;
}

void QWDoubleProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	std::unique_ptr<double> pint(new double(src_));
	iEvent.put(move(pint));
}

DEFINE_FWK_MODULE(QWDoubleProducer);
