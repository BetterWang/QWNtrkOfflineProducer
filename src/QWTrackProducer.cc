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
class QWTrackProducer : public edm::EDProducer {
public:
	explicit QWTrackProducer(const edm::ParameterSet&);
	~QWTrackProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///

	edm::InputTag	vertexSrc_;
	edm::InputTag	trackSrc_;
	edm::InputTag	centralitySrc_;

	double  dzdzerror_;
	double  d0d0error_;
	double  pterrorpt_;

	double	pTmin_;
	double	pTmax_;
	double	Etamin_;
	double	Etamax_;
	int	minPxLayers_;

	bool	bEff_;
	bool	bFlip_;

};

QWTrackProducer::QWTrackProducer(const edm::ParameterSet& pset) :
	vertexSrc_(pset.getUntrackedParameter<edm::InputTag>("vertexSrc")),
	trackSrc_(pset.getUntrackedParameter<edm::InputTag>("trackSrc"))
{
	consumes<reco::TrackCollection>(trackSrc_);
	consumes<reco::VertexCollection>(vertexSrc_);

	produces<std::vector<double> >("phi");
	produces<std::vector<double> >("eta");
	produces<std::vector<double> >("pt");
	produces<std::vector<double> >("charge");

	produces<std::vector<double> >("pd0");
	produces<std::vector<double> >("pdz");
	produces<std::vector<double> >("pd0err");
	produces<std::vector<double> >("pdzerr");
	produces<std::vector<double> >("pterr");

	produces<std::vector<double> >("nhits");
	produces<std::vector<double> >("algo");
	produces<std::vector<double> >("nChi2");
}

QWTrackProducer::~QWTrackProducer()
{
	return;
}

void QWTrackProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;

	std::unique_ptr<std::vector<double> > pphi( new std::vector<double> );
	std::unique_ptr<std::vector<double> > peta( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppT( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pcharge( new std::vector<double> );

	std::unique_ptr<std::vector<double> > ppTerror( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pd0( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pdz( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pd0error( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pdzerror( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnhits( new std::vector<double> );
	std::unique_ptr<std::vector<double> > palgo( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnChi2( new std::vector<double> );

	Handle<VertexCollection> vertexCollection;
	iEvent.getByLabel(vertexSrc_, vertexCollection);
	VertexCollection recoVertices = *vertexCollection;
	sort(recoVertices.begin(), recoVertices.end(), [](const reco::Vertex &a, const reco::Vertex &b){
			return a.tracksSize() > b.tracksSize();
			});

	int primaryvtx = 0;
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
		pphi->push_back(itTrack->phi());
		ppT->push_back(itTrack->pt());
		peta->push_back(itTrack->eta());
		pcharge->push_back(itTrack->charge());

		ppTerror->push_back(itTrack->ptError());
		pd0->push_back( -1.* itTrack->dxy(v1) );
		pd0error->push_back(sqrt(itTrack->dxyError()*itTrack->dxyError()+vxError*vyError));
		pdz->push_back(itTrack->dz(v1));
		pdzerror->push_back(sqrt(itTrack->dzError()*itTrack->dzError()+vzError*vzError));
		pnhits->push_back(itTrack->numberOfValidHits());
		palgo->push_back(itTrack->algo());
		pnChi2->push_back(itTrack->normalizedChi2());
	}

	iEvent.put(move(pphi), std::string("phi"));
	iEvent.put(move(peta), std::string("eta"));
	iEvent.put(move(ppT), std::string("pt"));
	iEvent.put(move(pcharge), std::string("charge"));

	iEvent.put(move(ppTerror), std::string("pterr"));
	iEvent.put(move(pd0), std::string("pd0"));
	iEvent.put(move(pdz), std::string("pdz"));
	iEvent.put(move(pd0error), std::string("pd0err"));
	iEvent.put(move(pdzerror), std::string("pdzerr"));
	iEvent.put(move(pnhits), std::string("nhits"));
	iEvent.put(move(palgo), std::string("algo"));
	iEvent.put(move(pnChi2), std::string("nChi2"));
}


DEFINE_FWK_MODULE(QWTrackProducer);
