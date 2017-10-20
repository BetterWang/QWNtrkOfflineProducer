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

class QWNtrkOfflineProducer : public edm::EDProducer {
public:
	explicit QWNtrkOfflineProducer(const edm::ParameterSet&);
	~QWNtrkOfflineProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	edm::InputTag	vertexSrc_;
	edm::InputTag	trackSrc_;
};

QWNtrkOfflineProducer::QWNtrkOfflineProducer(const edm::ParameterSet& pset) :
	vertexSrc_(pset.getUntrackedParameter<edm::InputTag>("vertexSrc")),
	trackSrc_(pset.getUntrackedParameter<edm::InputTag>("trackSrc"))
{
	consumes<reco::TrackCollection>(trackSrc_);
	consumes<reco::VertexCollection>(vertexSrc_);
	produces<int>();
}

QWNtrkOfflineProducer::~QWNtrkOfflineProducer() {
	return;
}

void QWNtrkOfflineProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;
	int Noff = 0;

	Handle<VertexCollection> vertexCollection;
	iEvent.getByLabel(vertexSrc_, vertexCollection);
	VertexCollection recoVertices = *vertexCollection;

	sort(recoVertices.begin(), recoVertices.end(), [](const reco::Vertex &a, const reco::Vertex &b){
			return a.tracksSize() > b.tracksSize();
			});

	unsigned int primaryvtx = 0;
	for ( primaryvtx = 0; primaryvtx < recoVertices.size(); primaryvtx++ ) {
		if ( (!recoVertices[primaryvtx].isFake())
			and fabs(recoVertices[primaryvtx].z()) <= 25.
			and recoVertices[primaryvtx].position().Rho() <= 2.0
			and recoVertices[primaryvtx].tracksSize() >=2 ) break;
	}
	if ( primaryvtx == recoVertices.size() ) {
		iEvent.put(std::make_unique<int>(-1));
		return;
	}

	math::XYZPoint v1( recoVertices[primaryvtx].position().x(), recoVertices[primaryvtx].position().y(), recoVertices[primaryvtx].position().z() );
	double vxError = recoVertices[primaryvtx].xError();
	double vyError = recoVertices[primaryvtx].yError();
	double vzError = recoVertices[primaryvtx].zError();

	Handle<TrackCollection> tracks;
	iEvent.getByLabel(trackSrc_,tracks);
	for(TrackCollection::const_iterator itTrack = tracks->begin();
		itTrack != tracks->end();
		++itTrack) {

		if ( !itTrack->quality(reco::TrackBase::highPurity) ) continue;
		if ( itTrack->charge() == 0 ) continue;
		if ( itTrack->pt() < 0.4 ) continue;

		double d0 = -1.* itTrack->dxy(v1);
		double derror=sqrt(itTrack->dxyError()*itTrack->dxyError()+vxError*vyError);
		double dz=itTrack->dz(v1);
		double dzerror=sqrt(itTrack->dzError()*itTrack->dzError()+vzError*vzError);
		if ( fabs(itTrack->eta()) > 2.4 ) continue;
		if ( fabs( dz/dzerror ) > 3. ) continue;
		if ( fabs( d0/derror ) > 3. ) continue;
		if ( itTrack->ptError()/itTrack->pt() > 0.1 ) continue;

		Noff++;
	}
	//std::unique_ptr<int> pNoff(new int(Noff));
	iEvent.put(std::make_unique<int>(Noff));
}

DEFINE_FWK_MODULE(QWNtrkOfflineProducer);
