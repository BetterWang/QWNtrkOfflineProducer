#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
//#include "MagneticField/Engine/interface/MagneticField.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidateFwd.h"
#include "TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h"

#include "TFile.h"
#include "TH2.h"
#include "TMath.h"
#include <Math/SMatrix.h>
#include <Math/SVector.h>

typedef ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> > SMatrixSym3D;
typedef ROOT::Math::SVector<double, 3> SVector3;

using namespace std;
class QWV0VectProducer : public edm::EDProducer {
public:
	explicit QWV0VectProducer(const edm::ParameterSet&);
	~QWV0VectProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///

	edm::InputTag	vertexSrc_;
	edm::InputTag	trackSrc_;

	edm::InputTag	V0Src_;

	double	pTmin_;
	double	pTmax_;
	double	Etamin_;
	double	Etamax_;
	double	Massmin_;
	double	Massmax_;

	bool	bFlip_;
};

QWV0VectProducer::QWV0VectProducer(const edm::ParameterSet& pset) :
	vertexSrc_(pset.getParameter<edm::InputTag>("vertexSrc")),
	trackSrc_(pset.getParameter<edm::InputTag>("trackSrc")),
	V0Src_(pset.getParameter<edm::InputTag>("V0Src"))
{
	consumes<reco::TrackCollection>(trackSrc_);
	consumes<reco::VertexCollection>(vertexSrc_);
	consumes<reco::VertexCompositeCandidateCollection>(V0Src_);

	pTmin_ = pset.getParameter<double>("ptMin");
	pTmax_ = pset.getParameter<double>("ptMax");
	Etamin_ = pset.getParameter<double>("Etamin");
	Etamax_ = pset.getParameter<double>("Etamax");
	Massmin_ = pset.getParameter<double>("Massmin");
	Massmax_ = pset.getParameter<double>("Massmax");

	bFlip_ = pset.getUntrackedParameter<bool>("bFlip", false);

	produces<std::vector<double> >("phi");
	produces<std::vector<double> >("eta");
	produces<std::vector<double> >("pt");
	produces<std::vector<double> >("mass");

	produces<std::vector<double> >("vtxChi2");
	produces<std::vector<double> >("vtxNdf");
	produces<std::vector<double> >("vtxChi2oNdf");

	produces<std::vector<double> >("Lxy");
	produces<std::vector<double> >("Lxyz");

	produces<std::vector<double> >("cosThetaXY");
	produces<std::vector<double> >("cosThetaXYZ");

	produces<std::vector<double> >("vtxDecaySigXY");
	produces<std::vector<double> >("vtxDecaySigXYZ");

	produces<std::vector<double> >("DCA");
}

QWV0VectProducer::~QWV0VectProducer()
{
	return;
}

void QWV0VectProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;

	std::auto_ptr<std::vector<double> > pphi( new std::vector<double> );
	std::auto_ptr<std::vector<double> > peta( new std::vector<double> );
	std::auto_ptr<std::vector<double> > ppT( new std::vector<double> );
	std::auto_ptr<std::vector<double> > pmass( new std::vector<double> );

	std::auto_ptr<std::vector<double> > pvtxChi2( new std::vector<double> );
	std::auto_ptr<std::vector<double> > pvtxNdf( new std::vector<double> );
	std::auto_ptr<std::vector<double> > pvtxChi2oNdf( new std::vector<double> );

	std::auto_ptr<std::vector<double> > pLxy( new std::vector<double> );
	std::auto_ptr<std::vector<double> > pLxyz( new std::vector<double> );

	std::auto_ptr<std::vector<double> > pcosThetaXY( new std::vector<double> );
	std::auto_ptr<std::vector<double> > pcosThetaXYZ( new std::vector<double> );

	std::auto_ptr<std::vector<double> > pvtxDecaySigXY( new std::vector<double> );
	std::auto_ptr<std::vector<double> > pvtxDecaySigXYZ( new std::vector<double> );

	std::auto_ptr<std::vector<double> > pDCA( new std::vector<double> );

	edm::ESHandle<TransientTrackBuilder> theB;
	iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);

//	edm::Handle<reco::TrackCollection> theTrackHandle;
//	iEvent.getByLabel(trackSrc_, theTrackHandle);
//	if (!theTrackHandle->size()) return;
//	const reco::TrackCollection* theTrackCollection = theTrackHandle.product();


	edm::Handle< reco::VertexCollection > pvHandle;
	iEvent.getByLabel(vertexSrc_, pvHandle);
	reco::Vertex const * pv = nullptr;  
	size_t nPV = 0;
	int const pvNDOF_ = 4; // hardcoded
	if (pvHandle.isValid()) {
		pv = &pvHandle->front();
		//--- pv fake (the pv collection should have size==1 and the pv==beam spot)
		if (   pv->isFake() || pv->tracksSize()==0
			// definition of goodOfflinePrimaryVertex
			|| pv->ndof() < pvNDOF_ || pv->z() > 24.)  pv = nullptr;

		for (auto v : *pvHandle) {
			if (v.isFake()        ) continue;
			if (v.ndof() < pvNDOF_) continue;
			if (v.z() > 24.       ) continue; 
			++nPV;
		}
	}

	Handle<VertexCompositeCandidateCollection> V0s;
	iEvent.getByLabel(V0Src_, V0s);

	for ( auto & v0 : (*V0s) ) {
		float mass     = v0.mass();
		float pt       = v0.pt();
		float eta      = v0.eta();
		float phi      = v0.phi();
		float chi2oNDF = v0.vertexNormalizedChi2();
		GlobalPoint displacementFromPV = ( pv==nullptr ? GlobalPoint(-9.,-9.,-9.) : GlobalPoint( (pv->x() - v0.vx()), (pv->y() - v0.vy()), (pv->z() - v0.vz()) ) );
//		GlobalPoint momentum = GlobalPoint(v0.px(), v0.py(), v0.pz());
		float lxy      = ( pv==nullptr ? -9. : displacementFromPV.perp() );
		float lxyz     = ( pv==nullptr ? -9. : displacementFromPV.mag() );

		if ( pt > pTmax_ or pt < pTmin_ ) continue;
		if ( eta > Etamax_ or eta < Etamin_ ) continue;
		if ( mass > Massmax_ or mass < Massmin_ ) continue;


		pphi		->push_back( phi	);
		peta		->push_back( eta	);
		ppT		->push_back( pt		);
		pmass		->push_back( mass	);

		pvtxChi2	->push_back( v0.vertexChi2()	);
		pvtxNdf		->push_back( v0.vertexNdof()	);
		pvtxChi2oNdf	->push_back( chi2oNDF		);

		pLxy		->push_back( lxy	);
		pLxyz		->push_back( lxyz	);

		double	cosThetaXY  = (displacementFromPV.x()*v0.px() + displacementFromPV.y()*v0.py()) / ( displacementFromPV.mag2()*sqrt(v0.px()*v0.px() + v0.py()*v0.py()) );
		double	cosThetaXYZ = (displacementFromPV.x()*v0.px() + displacementFromPV.y()*v0.py() + displacementFromPV.z()*v0.pz()) / ( displacementFromPV.mag()*sqrt(v0.px()*v0.px() + v0.py()*v0.py() + v0.pz()*v0.pz()) );

		pcosThetaXY	->push_back( cosThetaXY		);
		pcosThetaXYZ	->push_back( cosThetaXYZ	);

		SMatrixSym3D totalCov = pv->covariance() + v0.vertexCovariance();
		SVector3 distVecXY(displacementFromPV.x(), displacementFromPV.y(), 0.);
		double distMagXY = ROOT::Math::Mag(distVecXY);
		double sigmaDistMagXY = sqrt(ROOT::Math::Similarity(totalCov, distVecXY)) / distMagXY;
		double vtxDecaySigXY = distMagXY/sigmaDistMagXY;

		SVector3 distVecXYZ(displacementFromPV.x(), displacementFromPV.y(), displacementFromPV.z());
		double distMagXYZ = ROOT::Math::Mag(distVecXYZ);
		double sigmaDistMagXYZ = sqrt(ROOT::Math::Similarity(totalCov, distVecXYZ)) / distMagXYZ;
		double vtxDecaySigXYZ = distMagXYZ/sigmaDistMagXYZ;

		pvtxDecaySigXY	->push_back( vtxDecaySigXY	);
		pvtxDecaySigXYZ	->push_back( vtxDecaySigXYZ	);

		double dca = -999.;
		if ( v0.numberOfDaughters() == 2 ) {
			auto tt0 = (theB->build(v0.daughter(0)->bestTrack())).initialFreeState();
			auto tt1 = (theB->build(v0.daughter(1)->bestTrack())).initialFreeState();

			ClosestApproachInRPhi cApp;
			cApp.calculate(tt0, tt1);
      			if (cApp.status()) {
				dca = std::abs(cApp.distance());
			}
		}
		pDCA	->push_back( dca	);
	}


	iEvent.put(pphi, std::string("phi"));
	iEvent.put(peta, std::string("eta"));
	iEvent.put(ppT, std::string("pt"));
	iEvent.put(pmass, std::string("mass"));

	iEvent.put(pvtxChi2, std::string("vtxChi2"));
	iEvent.put(pvtxNdf, std::string("vtxNdf"));
	iEvent.put(pvtxChi2oNdf, std::string("vtxChi2oNdf"));

	iEvent.put(pLxy, std::string("Lxy"));
	iEvent.put(pLxyz, std::string("Lxyz"));

	iEvent.put(pcosThetaXY, std::string("cosThetaXY"));
	iEvent.put(pcosThetaXYZ, std::string("cosThetaXYZ"));

	iEvent.put(pvtxDecaySigXY, std::string("vtxDecaySigXY"));
	iEvent.put(pvtxDecaySigXYZ, std::string("vtxDecaySigXYZ"));

	iEvent.put(pDCA, std::string("DCA"));
}


DEFINE_FWK_MODULE(QWV0VectProducer);
