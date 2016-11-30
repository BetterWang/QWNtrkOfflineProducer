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

	std::auto_ptr<double> prunid(new double(runid));
	std::auto_ptr<double> peventid(new double(eventid));
	std::auto_ptr<double> pbx(new double(bx));
	std::auto_ptr<double> plumi(new double(lumi));
	std::auto_ptr<double> porbit(new double(orbit));

	iEvent.put(prunid, std::string("RunId"));
	iEvent.put(peventid, std::string("EventId"));
	iEvent.put(pbx, std::string("BX"));
	iEvent.put(plumi, std::string("Lumi"));
	iEvent.put(porbit, std::string("Orbit"));
}


DEFINE_FWK_MODULE(QWEventInfoProducer);
