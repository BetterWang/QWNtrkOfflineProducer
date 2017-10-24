#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "TrackingTools/TransientTrack/interface/TransientTrack.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/VolumeBasedEngine/interface/VolumeBasedMagneticField.h"


#include "TFile.h"
#include "TH2.h"
#include "TMath.h"

using namespace std;
class QWVertexProducer : public edm::EDProducer {
public:
	explicit QWVertexProducer(const edm::ParameterSet&);
	~QWVertexProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///
	edm::InputTag	vertexSrc_;

};

QWVertexProducer::QWVertexProducer(const edm::ParameterSet& pset) :
	vertexSrc_(pset.getUntrackedParameter<edm::InputTag>("vertexSrc"))
{
	consumes<reco::VertexCollection>(vertexSrc_);

	produces<int>("size");
	produces<std::vector<double> >("tracks");
	produces<std::vector<double> >("vx");
	produces<std::vector<double> >("vy");
	produces<std::vector<double> >("vz");
}

QWVertexProducer::~QWVertexProducer()
{
	return;
}

void QWVertexProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;

	auto ptracks = std::make_unique<std::vector<double> >();
	auto pvx = std::make_unique<std::vector<double> >();
	auto pvy = std::make_unique<std::vector<double> >();
	auto pvz = std::make_unique<std::vector<double> >();

	Handle<VertexCollection> vertexCollection;
	iEvent.getByLabel(vertexSrc_, vertexCollection);
	VertexCollection recoVertices = *vertexCollection;
	sort(recoVertices.begin(), recoVertices.end(), [](const reco::Vertex &a, const reco::Vertex &b){
			return a.tracksSize() > b.tracksSize();
			});
	for ( auto it = recoVertices.begin(); it != recoVertices.end(); ++it ) {
		ptracks->push_back(it->tracksSize());
		pvx->push_back(it->x());
		pvy->push_back(it->y());
		pvz->push_back(it->z());
	}

	int sz = recoVertices.size();


	iEvent.put(std::make_unique<int>(sz), std::string("size"));
	iEvent.put(std::move(ptracks), std::string("tracks"));
	iEvent.put(std::move(pvx), std::string("vx"));
	iEvent.put(std::move(pvy), std::string("vy"));
	iEvent.put(std::move(pvz), std::string("vz"));
}



DEFINE_FWK_MODULE(QWVertexProducer);
