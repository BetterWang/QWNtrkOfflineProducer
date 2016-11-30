#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

using namespace std;
class QWZDCProducer : public edm::EDProducer {
public:
	explicit QWZDCProducer(const edm::ParameterSet&);
	~QWZDCProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///

	edm::InputTag	Src_;

};

QWZDCProducer::QWZDCProducer(const edm::ParameterSet& pset) :
	Src_(pset.getUntrackedParameter<edm::InputTag>("Src")),
{
	consumes<ZDCDigiCollection>(Src_);

	produces<std::vector<double> >("ADC");
	produces<std::vector<double> >("fC");
}

QWZDCProducer::~QWZDCProducer()
{
	return;
}

void QWZDCProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;

	std::auto_ptr<std::vector<double> > pADC( new std::vector<double> );
	std::auto_ptr<std::vector<double> > pfC( new std::vector<double> );


	Handle<ZDCDigiCollection> digi;
	iEvent.getByLabel(Src_, digi);

	for ( auto i = digi->begin(); i < digi->end(); i++ ) {
		cout << "i->id().zside() = " << i->id().zside() << " i->id()..section() = " << i->id()..section() << " i->id().channel() = " << i->id().channel() << endl;
	}

	iEvent.put(pADC, std::string("ADC"));
	iEvent.put(pfC, std::string("fC"));
}


///
bool
QWZDCProducer::TrackQuality_ppReco(const reco::TrackCollection::const_iterator& itTrack, const reco::VertexCollection& recoVertices)
{
        if ( itTrack->charge() == 0 ) return false;
        if ( !itTrack->quality(reco::TrackBase::highPurity) ) return false;
        if ( itTrack->ptError()/itTrack->pt() > pterrorpt_ ) return false;
	int primaryvtx = 0;
	math::XYZPoint v1( recoVertices[primaryvtx].position().x(), recoVertices[primaryvtx].position().y(), recoVertices[primaryvtx].position().z() );
	double vxError = recoVertices[primaryvtx].xError();
	double vyError = recoVertices[primaryvtx].yError();
	double vzError = recoVertices[primaryvtx].zError();
        double d0 = -1.* itTrack->dxy(v1);
        double derror=sqrt(itTrack->dxyError()*itTrack->dxyError()+vxError*vyError);
        if ( fabs( d0/derror ) > d0d0error_ ) {
                return false;
        }
        double dz=itTrack->dz(v1);
        double dzerror=sqrt(itTrack->dzError()*itTrack->dzError()+vzError*vzError);
        if ( fabs( dz/dzerror ) > dzdzerror_ ) {
                return false;
        }
        return true;
}

///
bool
QWZDCProducer::TrackQuality_HIReco(const reco::TrackCollection::const_iterator& itTrack, const reco::VertexCollection& recoVertices)
{
	if ( itTrack->charge() == 0 ) return false;
	if ( !itTrack->quality(reco::TrackBase::highPurity) ) return false;
	if ( itTrack->numberOfValidHits() < 11 ) return false;
	if ( itTrack->normalizedChi2() / itTrack->hitPattern().trackerLayersWithMeasurement() > 0.15 ) {
		return false;
	}
	if ( itTrack->ptError()/itTrack->pt() > pterrorpt_ ) {
		return false;
	}
	if (
		itTrack->originalAlgo() != 4 and
		itTrack->originalAlgo() != 5 and
		itTrack->originalAlgo() != 6 and
		itTrack->originalAlgo() != 7
	) {
		return false;
	}

	int primaryvtx = 0;
	math::XYZPoint v1( recoVertices[primaryvtx].position().x(), recoVertices[primaryvtx].position().y(), recoVertices[primaryvtx].position().z() );
	double vxError = recoVertices[primaryvtx].xError();
	double vyError = recoVertices[primaryvtx].yError();
	double vzError = recoVertices[primaryvtx].zError();
	double d0 = -1.* itTrack->dxy(v1);
	double derror=sqrt(itTrack->dxyError()*itTrack->dxyError()+vxError*vyError);
	if ( fabs( d0/derror ) > d0d0error_ ) {
		return false;
	}

	double dz=itTrack->dz(v1);
	double dzerror=sqrt(itTrack->dzError()*itTrack->dzError()+vzError*vzError);
	if ( fabs( dz/dzerror ) > dzdzerror_ ) {
		return false;
	}
	return true;
}

///
bool
QWZDCProducer::TrackQuality_Pixel(const reco::TrackCollection::const_iterator& itTrack, const reco::VertexCollection& recoVertices)
{
	if ( itTrack->charge() == 0 ) return false;
	if ( !itTrack->quality(reco::TrackBase::highPurity) ) return false;
	bool bPix = false;
	int nHits = itTrack->numberOfValidHits();
//	std::cout << __LINE__ << "\tnHits = " << nHits << std::endl;
	if ( itTrack->pt() < 2.4 and (nHits==3 or nHits==4 or nHits==5 or nHits==6) ) bPix = true;
	if ( not bPix ) {
		if ( nHits < 11 ) return false;
		if ( itTrack->normalizedChi2() / itTrack->hitPattern().trackerLayersWithMeasurement() > 0.15 ) {
			return false;
		}
		if ( itTrack->ptError()/itTrack->pt() > pterrorpt_ ) {
			return false;
		}
		if (
			itTrack->pt() > 2.4 and
			itTrack->originalAlgo() != 4 and
			itTrack->originalAlgo() != 5 and
			itTrack->originalAlgo() != 6 and
			itTrack->originalAlgo() != 7
		) {
			return false;
		}

		int primaryvtx = 0;
		math::XYZPoint v1( recoVertices[primaryvtx].position().x(), recoVertices[primaryvtx].position().y(), recoVertices[primaryvtx].position().z() );
		double vxError = recoVertices[primaryvtx].xError();
		double vyError = recoVertices[primaryvtx].yError();
		double vzError = recoVertices[primaryvtx].zError();
		double d0 = -1.* itTrack->dxy(v1);
		double derror=sqrt(itTrack->dxyError()*itTrack->dxyError()+vxError*vyError);
		if ( fabs( d0/derror ) > d0d0error_ ) {
			return false;
		}

		double dz=itTrack->dz(v1);
		double dzerror=sqrt(itTrack->dzError()*itTrack->dzError()+vzError*vzError);
		if ( fabs( dz/dzerror ) > dzdzerror_ ) {
			return false;
		}
	}
	return true;
}

DEFINE_FWK_MODULE(QWZDCProducer);
