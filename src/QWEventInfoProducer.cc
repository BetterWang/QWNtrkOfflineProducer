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

#include "TFile.h"
#include "TH2.h"
#include "TMath.h"

using namespace std;
class QWEventInfoProducer : public edm::EDProducer {
public:
	explicit QWEventInfoProducer(const edm::ParameterSet&);
	~QWEventInfoProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///
};

QWEventInfoProducer::QWEventInfoProducer(const edm::ParameterSet& pset)
{
	produces< double >("RunId");
	produces< double >("EventId");
	produces< double >("Lumi");
	produces< double >("BX");
	produces< double >("Orbit");
}

QWEventInfoProducer::~QWEventInfoProducer()
{
	return;
}

void QWEventInfoProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;

	double runid = double(iEvent.run());
	double eventid = iEvent.id().event();
	double bx = iEvent.bunchCrossing();
	double lumi = iEvent.luminosityBlock();
	double orbit = iEvent.orbitNumber();

	iEvent.put(make_unique<double>(double(runid))  , std::string("RunId"));
	iEvent.put(make_unique<double>(double(eventid)), std::string("EventId"));
	iEvent.put(make_unique<double>(double(bx))     , std::string("BX"));
	iEvent.put(make_unique<double>(double(lumi))   , std::string("Lumi"));
	iEvent.put(make_unique<double>(double(orbit))  , std::string("Orbit"));
}


DEFINE_FWK_MODULE(QWEventInfoProducer);
