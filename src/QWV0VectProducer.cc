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
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"

#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidateFwd.h"
#include "TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h"

#include "CommonTools/CandUtils/interface/CenterOfMassBooster.h"

#include "TFile.h"
#include "TH2.h"
#include "TMath.h"
#include "TVector3.h"
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

	typedef struct {
		double	pTmin_;
		double	pTmax_;
		double	Etamin_;
		double	Etamax_;
		double	Rapmin_;
		double	Rapmax_;
		double	Massmin_;
		double	Massmax_;

		double	Chi2min_;
		double	Chi2max_;

		double	Ndfmin_;
		double	Ndfmax_;

		double	Chi2oNdfmin_;
		double	Chi2oNdfmax_;

		double	Lxymin_;
		double	Lxymax_;

		double	Lxyzmin_;
		double	Lxyzmax_;

		double	ThetaXYmin_;
		double	ThetaXYmax_;

		double	ThetaXYZmin_;
		double	ThetaXYZmax_;

		double	DecaySigXYmin_;
		double	DecaySigXYmax_;

		double	DecaySigXYZmin_;
		double	DecaySigXYZmax_;

		double	DCAmin_;
		double	DCAmax_;

		double	VtxProbmin_;
		double	VtxProbmax_;

		double	AngleMin_;
		double	AngleMax_;
	} V0_cut;
	std::vector<QWV0VectProducer::V0_cut> cuts_;

	double dauEtaMin_;
	double dauEtaMax_;
	double dauNhitsMin_;
	double dauNhitsMax_;
	double dauPtMin_;
	double dauPtMax_;
	double dauPterrorMin_;
	double dauPterrorMax_;
	double dauDCASig_;

	double mis_ks_range_;
	double mis_la_range_;
	double mis_ph_range_;

};

QWV0VectProducer::QWV0VectProducer(const edm::ParameterSet& pset) :
	vertexSrc_(pset.getUntrackedParameter<edm::InputTag>("vertexSrc")),
	trackSrc_(pset.getUntrackedParameter<edm::InputTag>("trackSrc")),
	V0Src_(pset.getUntrackedParameter<edm::InputTag>("V0Src"))
{

	auto pcuts = pset.getUntrackedParameter< std::vector< edm::ParameterSet > >("cuts");
	for ( auto pcut : pcuts ) {
		QWV0VectProducer::V0_cut cut;

		cut.pTmin_ = pcut.getUntrackedParameter<double>("ptMin", 0.);
		cut.pTmax_ = pcut.getUntrackedParameter<double>("ptMax", 100.);
		cut.Etamin_ = pcut.getUntrackedParameter<double>("Etamin", -2.4);
		cut.Etamax_ = pcut.getUntrackedParameter<double>("Etamax", 2.4);
		cut.Rapmin_ = pcut.getUntrackedParameter<double>("Rapmin", -3.0);
		cut.Rapmax_ = pcut.getUntrackedParameter<double>("Rapmax", 3.0);
		cut.Massmin_ = pcut.getUntrackedParameter<double>("Massmin", 0.);
		cut.Massmax_ = pcut.getUntrackedParameter<double>("Massmax", 1000);

		cut.Chi2min_ = pcut.getUntrackedParameter<double>("Chi2Min", -std::numeric_limits<double>::max());
		cut.Chi2max_ = pcut.getUntrackedParameter<double>("Chi2Max",  std::numeric_limits<double>::max());

		cut.Ndfmin_ = pcut.getUntrackedParameter<double>("NdfMin", -std::numeric_limits<double>::max());
		cut.Ndfmax_ = pcut.getUntrackedParameter<double>("NdfMax",  std::numeric_limits<double>::max());

		cut.Chi2oNdfmin_ = pcut.getUntrackedParameter<double>("Chi2oNdfMin", -std::numeric_limits<double>::max());
		cut.Chi2oNdfmax_ = pcut.getUntrackedParameter<double>("Chi2oNdfMax",  std::numeric_limits<double>::max());

		cut.Lxymin_ = pcut.getUntrackedParameter<double>("LxyMin", -std::numeric_limits<double>::max());
		cut.Lxymax_ = pcut.getUntrackedParameter<double>("LxyMax",  std::numeric_limits<double>::max());

		cut.Lxyzmin_ = pcut.getUntrackedParameter<double>("LxyzMin", -std::numeric_limits<double>::max());
		cut.Lxyzmax_ = pcut.getUntrackedParameter<double>("LxyzMax",  std::numeric_limits<double>::max());

		cut.ThetaXYmin_ = pcut.getUntrackedParameter<double>("ThetaXYMin", -std::numeric_limits<double>::max());
		cut.ThetaXYmax_ = pcut.getUntrackedParameter<double>("ThetaXYMax",  std::numeric_limits<double>::max());

		cut.ThetaXYZmin_ = pcut.getUntrackedParameter<double>("ThetaXYZMin", -std::numeric_limits<double>::max());
		cut.ThetaXYZmax_ = pcut.getUntrackedParameter<double>("ThetaXYZMax",  std::numeric_limits<double>::max());

		cut.DecaySigXYmin_ = pcut.getUntrackedParameter<double>("DecayXYMin", -std::numeric_limits<double>::max());
		cut.DecaySigXYmax_ = pcut.getUntrackedParameter<double>("DecayXYMax",  std::numeric_limits<double>::max());

		cut.DecaySigXYZmin_ = pcut.getUntrackedParameter<double>("DecayXYZMin", -std::numeric_limits<double>::max());
		cut.DecaySigXYZmax_ = pcut.getUntrackedParameter<double>("DecayXYZMax",  std::numeric_limits<double>::max());

		cut.DCAmin_ = pcut.getUntrackedParameter<double>("DCAMin", -std::numeric_limits<double>::max());
		cut.DCAmax_ = pcut.getUntrackedParameter<double>("DCAMax",  std::numeric_limits<double>::max());


		cut.VtxProbmin_ = pcut.getUntrackedParameter<double>("VtxProbmin", -std::numeric_limits<double>::max());
		cut.VtxProbmax_ = pcut.getUntrackedParameter<double>("VtxProbmax",  std::numeric_limits<double>::max());

		cut.AngleMin_ = pcut.getUntrackedParameter<double>("AngleMin", -std::numeric_limits<double>::max());
		cut.AngleMax_ = pcut.getUntrackedParameter<double>("AngleMax",  std::numeric_limits<double>::max());

		cuts_.push_back(cut);
	}
	auto dcuts = pset.getUntrackedParameter< edm::ParameterSet >("daughter_cuts");
	dauEtaMin_ = dcuts.getUntrackedParameter<double>("dauEtaMin", -std::numeric_limits<double>::max());
	dauEtaMax_ = dcuts.getUntrackedParameter<double>("dauEtaMax",  std::numeric_limits<double>::max());
	dauNhitsMin_ = dcuts.getUntrackedParameter<double>("dauNhitsMin", -std::numeric_limits<double>::max());
	dauNhitsMax_ = dcuts.getUntrackedParameter<double>("dauNhitsMax",  std::numeric_limits<double>::max());
	dauPtMin_ = dcuts.getUntrackedParameter<double>("dauPtMin", -std::numeric_limits<double>::max());
	dauPtMax_ = dcuts.getUntrackedParameter<double>("dauPtMax",  std::numeric_limits<double>::max());
	dauPterrorMin_ = dcuts.getUntrackedParameter<double>("dauPterrorMin", -std::numeric_limits<double>::max());
	dauPterrorMax_ = dcuts.getUntrackedParameter<double>("dauPterrorMax",  std::numeric_limits<double>::max());
	dauDCASig_ = dcuts.getUntrackedParameter<double>("dauDCASig", 0.0);

	mis_ks_range_ = pset.getUntrackedParameter<double>("mis_ks_range", 0.020);
	mis_la_range_ = pset.getUntrackedParameter<double>("mis_la_range", 0.010);
	mis_ph_range_ = pset.getUntrackedParameter<double>("mis_ph_range", 0.015);

	consumes<reco::TrackCollection>(trackSrc_);
	consumes<reco::VertexCollection>(vertexSrc_);
	consumes<reco::VertexCompositeCandidateCollection>(V0Src_);

	produces<std::vector<double> >("phi");
	produces<std::vector<double> >("eta");
	produces<std::vector<double> >("rapidity");
	produces<std::vector<double> >("pt");
	produces<std::vector<double> >("mass");
	produces<std::vector<double> >("weight");

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

	produces<std::vector<double> >("Refs");

	produces<std::vector<double> >("pdgId");
	produces<std::vector<double> >("pPhiCM");
	produces<std::vector<double> >("nPhiCM");

	produces<std::vector<double> >("pPxCM");
	produces<std::vector<double> >("pPyCM");
	produces<std::vector<double> >("pPzCM");
	produces<std::vector<double> >("nPxCM");
	produces<std::vector<double> >("nPyCM");
	produces<std::vector<double> >("nPzCM");

	produces<std::vector<double> >("pTrkQuality");
	produces<std::vector<double> >("pTrkNHit");
	produces<std::vector<double> >("pTrkPx");
	produces<std::vector<double> >("pTrkPy");
	produces<std::vector<double> >("pTrkPz");
	produces<std::vector<double> >("pTrkPt");
	produces<std::vector<double> >("pTrkPhi");
	produces<std::vector<double> >("pTrkPtError");
	produces<std::vector<double> >("pTrkEta");
	produces<std::vector<double> >("pTrkNPxLayer");
	produces<std::vector<double> >("pTrkDCASigXY");
	produces<std::vector<double> >("pTrkDCASigZ");

	produces<std::vector<double> >("nTrkQuality");
	produces<std::vector<double> >("nTrkNHit");
	produces<std::vector<double> >("nTrkPx");
	produces<std::vector<double> >("nTrkPy");
	produces<std::vector<double> >("nTrkPz");
	produces<std::vector<double> >("nTrkPt");
	produces<std::vector<double> >("nTrkPhi");
	produces<std::vector<double> >("nTrkPtError");
	produces<std::vector<double> >("nTrkEta");
	produces<std::vector<double> >("nTrkNPxLayer");
	produces<std::vector<double> >("nTrkDCASigXY");
	produces<std::vector<double> >("nTrkDCASigZ");

	produces<reco::VertexCompositeCandidateCollection>();
}

QWV0VectProducer::~QWV0VectProducer()
{
	return;
}

void QWV0VectProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;

	std::unique_ptr<std::vector<double> > pphi( new std::vector<double> );
	std::unique_ptr<std::vector<double> > peta( new std::vector<double> );
	std::unique_ptr<std::vector<double> > prapidity( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppT( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pmass( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pweight( new std::vector<double> );

	std::unique_ptr<std::vector<double> > pvtxChi2( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pvtxNdf( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pvtxChi2oNdf( new std::vector<double> );

	std::unique_ptr<std::vector<double> > pLxy( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pLxyz( new std::vector<double> );

	std::unique_ptr<std::vector<double> > pcosThetaXY( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pcosThetaXYZ( new std::vector<double> );

	std::unique_ptr<std::vector<double> > pvtxDecaySigXY( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pvtxDecaySigXYZ( new std::vector<double> );

	std::unique_ptr<std::vector<double> > pDCA( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pRefs( new std::vector<double> );

	std::unique_ptr<std::vector<double> > ppdgId( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppPhiCM( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnPhiCM( new std::vector<double> );

	std::unique_ptr<std::vector<double> > ppPxCM( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppPyCM( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppPzCM( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnPxCM( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnPyCM( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnPzCM( new std::vector<double> );

	std::unique_ptr<std::vector<double> > ppTrkQuality( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppTrkNHit( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppTrkPx( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppTrkPy( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppTrkPz( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppTrkPt( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppTrkPhi( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppTrkPtError( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppTrkEta( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppTrkNPxLayer( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppTrkDCASigXY( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppTrkDCASigZ( new std::vector<double> );

	std::unique_ptr<std::vector<double> > pnTrkQuality( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnTrkNHit( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnTrkPx( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnTrkPy( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnTrkPz( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnTrkPt( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnTrkPhi( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnTrkPtError( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnTrkEta( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnTrkNPxLayer( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnTrkDCASigXY( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnTrkDCASigZ( new std::vector<double> );

	std::unique_ptr< VertexCompositeCandidateCollection > pV0( new VertexCompositeCandidateCollection );

	edm::ESHandle<TransientTrackBuilder> theB;
	iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);

//	edm::Handle<reco::TrackCollection> theTrackHandle;
//	iEvent.getByLabel(trackSrc_, theTrackHandle);
//	if (!theTrackHandle->size()) return;
//	const reco::TrackCollection* theTrackCollection = theTrackHandle.product();


	edm::Handle< reco::VertexCollection > pvHandle;
	iEvent.getByLabel(vertexSrc_, pvHandle);
	VertexCollection recoVertices = *pvHandle;
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
		iEvent.put(std::move(pphi), std::string("phi"));
		iEvent.put(std::move(peta), std::string("eta"));
		iEvent.put(std::move(prapidity), std::string("rapidity"));
		iEvent.put(std::move(ppT), std::string("pt"));
		iEvent.put(std::move(pmass), std::string("mass"));
		iEvent.put(std::move(pweight), std::string("weight"));

		iEvent.put(std::move(pvtxChi2), std::string("vtxChi2"));
		iEvent.put(std::move(pvtxNdf), std::string("vtxNdf"));
		iEvent.put(std::move(pvtxChi2oNdf), std::string("vtxChi2oNdf"));

		iEvent.put(std::move(pLxy), std::string("Lxy"));
		iEvent.put(std::move(pLxyz), std::string("Lxyz"));

		iEvent.put(std::move(pcosThetaXY), std::string("cosThetaXY"));
		iEvent.put(std::move(pcosThetaXYZ), std::string("cosThetaXYZ"));

		iEvent.put(std::move(pvtxDecaySigXY), std::string("vtxDecaySigXY"));
		iEvent.put(std::move(pvtxDecaySigXYZ), std::string("vtxDecaySigXYZ"));

		iEvent.put(std::move(pDCA), std::string("DCA"));
		iEvent.put(std::move(pRefs), std::string("Refs"));

		iEvent.put(std::move(ppdgId), std::string("pdgId"));
		iEvent.put(std::move(ppPhiCM), std::string("pPhiCM"));
		iEvent.put(std::move(pnPhiCM), std::string("nPhiCM"));

		iEvent.put(std::move(ppPxCM), std::string("pPxCM"));
		iEvent.put(std::move(ppPyCM), std::string("pPyCM"));
		iEvent.put(std::move(ppPzCM), std::string("pPzCM"));
		iEvent.put(std::move(pnPxCM), std::string("nPxCM"));
		iEvent.put(std::move(pnPyCM), std::string("nPyCM"));
		iEvent.put(std::move(pnPzCM), std::string("nPzCM"));

		iEvent.put(std::move(ppTrkQuality),  std::string("pTrkQuality"));
		iEvent.put(std::move(ppTrkNHit),     std::string("pTrkNHit"));
		iEvent.put(std::move(ppTrkPx),       std::string("pTrkPx"));
		iEvent.put(std::move(ppTrkPy),       std::string("pTrkPy"));
		iEvent.put(std::move(ppTrkPz),       std::string("pTrkPz"));
		iEvent.put(std::move(ppTrkPt),       std::string("pTrkPt"));
		iEvent.put(std::move(ppTrkPhi),      std::string("pTrkPhi"));
		iEvent.put(std::move(ppTrkPtError),  std::string("pTrkPtError"));
		iEvent.put(std::move(ppTrkEta),      std::string("pTrkEta"));
		iEvent.put(std::move(ppTrkNPxLayer), std::string("pTrkNPxLayer"));
		iEvent.put(std::move(ppTrkDCASigXY), std::string("pTrkDCASigXY"));
		iEvent.put(std::move(ppTrkDCASigZ),  std::string("pTrkDCASigZ"));

		iEvent.put(std::move(pnTrkQuality),  std::string("nTrkQuality"));
		iEvent.put(std::move(pnTrkNHit),     std::string("nTrkNHit"));
		iEvent.put(std::move(pnTrkPx),       std::string("nTrkPx"));
		iEvent.put(std::move(pnTrkPy),       std::string("nTrkPy"));
		iEvent.put(std::move(pnTrkPz),       std::string("nTrkPz"));
		iEvent.put(std::move(pnTrkPt),       std::string("nTrkPt"));
		iEvent.put(std::move(pnTrkPhi),      std::string("nTrkPhi"));
		iEvent.put(std::move(pnTrkPtError),  std::string("nTrkPtError"));
		iEvent.put(std::move(pnTrkEta),      std::string("nTrkEta"));
		iEvent.put(std::move(pnTrkNPxLayer), std::string("nTrkNPxLayer"));
		iEvent.put(std::move(pnTrkDCASigXY), std::string("nTrkDCASigXY"));
		iEvent.put(std::move(pnTrkDCASigZ),  std::string("nTrkDCASigZ"));

		iEvent.put(std::move(pV0));
		return;
	}

	reco::Vertex const * pv = &recoVertices[primaryvtx];

	Handle<VertexCompositeCandidateCollection> V0s;
	iEvent.getByLabel(V0Src_, V0s);
	for ( auto v0 : (*V0s) ) {
		float mass     = v0.mass();
		float pt       = v0.pt();
		float eta      = v0.eta();
		float rapidity = v0.rapidity();
		float phi      = v0.phi();
		GlobalPoint displacementFromPV ( (pv->x() - v0.vx()), (pv->y() - v0.vy()), (pv->z() - v0.vz()) ) ;
		float lxy      = ( displacementFromPV.perp() );
		float lxyz     = ( displacementFromPV.mag() );

		double Chi2 = v0.vertexChi2();
		double Ndf  = v0.vertexNdof();
		double chi2oNDF = v0.vertexNormalizedChi2();
		double VtxProb = TMath::Prob(Chi2, Ndf);

		double	cosThetaXY  = -(displacementFromPV.x()*v0.px() + displacementFromPV.y()*v0.py()) / ( displacementFromPV.transverse()*sqrt(v0.px()*v0.px() + v0.py()*v0.py()) );
		double	cosThetaXYZ = -(displacementFromPV.x()*v0.px() + displacementFromPV.y()*v0.py() + displacementFromPV.z()*v0.pz()) / ( displacementFromPV.mag()*sqrt(v0.px()*v0.px() + v0.py()*v0.py() + v0.pz()*v0.pz()) );

		SMatrixSym3D totalCov = pv->covariance() + v0.vertexCovariance();
		SVector3 distVecXY(displacementFromPV.x(), displacementFromPV.y(), 0.);
		double distMagXY = ROOT::Math::Mag(distVecXY);
		double sigmaDistMagXY = sqrt(ROOT::Math::Similarity(totalCov, distVecXY)) / distMagXY;
		double vtxDecaySigXY = distMagXY/sigmaDistMagXY;

		SVector3 distVecXYZ(displacementFromPV.x(), displacementFromPV.y(), displacementFromPV.z());
		double distMagXYZ = ROOT::Math::Mag(distVecXYZ);
		double sigmaDistMagXYZ = sqrt(ROOT::Math::Similarity(totalCov, distVecXYZ)) / distMagXYZ;
		double vtxDecaySigXYZ = distMagXYZ/sigmaDistMagXYZ;
		double dca = -999.;
        double dauLongImpactSig1 = -999.;
        double dauTransImpactSig1 = -999.;
        double dauLongImpactSig2 = -999.;
        double dauTransImpactSig2 = -999.;
		if ( v0.numberOfDaughters() == 2 ) {
			auto bT0 = v0.daughter(0)->bestTrack();
			auto bT1 = v0.daughter(1)->bestTrack();

			if ( bT0 and bT1 ) {
				auto tt0 = theB->build(bT0).initialFreeState();
				auto tt1 = theB->build(bT1).initialFreeState();

				ClosestApproachInRPhi cApp;
				cApp.calculate(tt0, tt1);
				if (cApp.status()) {
					dca = std::abs(cApp.distance());
				}
			}


			auto d1 = v0.daughter(0)->get<reco::TrackRef>();
			auto d2 = v0.daughter(1)->get<reco::TrackRef>();

//			auto d1 = dynamic_cast<const RecoChargedCandidate*>(v0.daughter(0));
//			auto d2 = dynamic_cast<const RecoChargedCandidate*>(v0.daughter(1));

			if ( d1->eta() < dauEtaMin_ or d1->eta() > dauEtaMax_
				or d2->eta() < dauEtaMin_ or d2->eta() > dauEtaMax_ ) continue;
			if ( d1->numberOfValidHits() < dauNhitsMin_ or d1->numberOfValidHits() > dauNhitsMax_
				or d2->numberOfValidHits() < dauNhitsMin_ or d2->numberOfValidHits() > dauNhitsMax_ ) continue;
			if ( d1->pt() < dauPtMin_ or d1->pt() > dauPtMax_
				or d2->pt() < dauPtMin_ or d2->pt() > dauPtMax_ ) continue;
			if ( d1->ptError() / d1->pt() < dauPterrorMin_ or d1->ptError() / d1->pt() > dauPterrorMax_
				or d2->ptError() / d2->pt() < dauPterrorMin_ or d2->ptError() / d2->pt() > dauPterrorMax_ ) continue;

			double xVtxError = pv->xError();
			double yVtxError = pv->yError();
			double zVtxError = pv->zError();
            math::XYZPoint bestvtx(pv->x(),pv->y(),pv->z());

			double dzvtx1 = d1->dz(bestvtx);
			double dxyvtx1 = d1->dxy(bestvtx);
			double dzerror1 = sqrt(d1->dzError()*d1->dzError()+zVtxError*zVtxError);
			double dxyerror1 = sqrt(d1->d0Error()*d1->d0Error()+xVtxError*yVtxError);
			dauLongImpactSig1 = fabs(dzvtx1/dzerror1);
			dauTransImpactSig1 = fabs(dxyvtx1/dxyerror1);

			double dzvtx2 = d2->dz(bestvtx);
			double dxyvtx2 = d2->dxy(bestvtx);
			double dzerror2 = sqrt(d2->dzError()*d2->dzError()+zVtxError*zVtxError);
			double dxyerror2 = sqrt(d2->d0Error()*d2->d0Error()+xVtxError*yVtxError);
			dauLongImpactSig2 = fabs(dzvtx2/dzerror2);
			dauTransImpactSig2 = fabs(dxyvtx2/dxyerror2);

            if ( dauLongImpactSig1 < dauDCASig_ or dauTransImpactSig1 < dauDCASig_
              or dauLongImpactSig2 < dauDCASig_ or dauTransImpactSig2 < dauDCASig_ ) continue;


			double pxd1 = d1->px();
			double pyd1 = d1->py();
			double pzd1 = d1->pz();
			double pd1  = d1->p();
			double pxd2 = d2->px();
			double pyd2 = d2->py();
			double pzd2 = d2->pz();
			double pd2  = d2->p();

			TVector3 dauvec1(pxd1,pyd1,pzd1);
			TVector3 dauvec2(pxd2,pyd2,pzd2);
			TVector3 dauvecsum(dauvec1+dauvec2);

			if ( V0Src_.instance() == "Kshort" ) {
				double v0masspiproton1 = sqrt((sqrt(0.93827*0.93827+pd1*pd1)+sqrt(0.13957*0.13957+pd2*pd2))*(sqrt(0.93827*0.93827+pd1*pd1)+sqrt(0.13957*0.13957+pd2*pd2))-dauvecsum.Mag2());
				double v0masspiproton2 = sqrt((sqrt(0.13957*0.13957+pd1*pd1)+sqrt(0.93827*0.93827+pd2*pd2))*(sqrt(0.13957*0.13957+pd1*pd1)+sqrt(0.93827*0.93827+pd2*pd2))-dauvecsum.Mag2());
				if((v0masspiproton1>=(1.115683-mis_la_range_) && v0masspiproton1<=(1.115683+mis_la_range_)) || (v0masspiproton2>=(1.115683-mis_la_range_) && v0masspiproton2<=(1.115683+mis_la_range_)) ) continue;
			}
			if ( V0Src_.instance() == "Lambda" ) {
				double v0masspipi = sqrt((sqrt(0.13957*0.13957+pd1*pd1)+sqrt(0.13957*0.13957+pd2*pd2))*(sqrt(0.13957*0.13957+pd1*pd1)+sqrt(0.13957*0.13957+pd2*pd2))-dauvecsum.Mag2());
				double v0massee = sqrt((sqrt(0.000511*0.000511+pd1*pd1)+sqrt(0.000511*0.000511+pd2*pd2))*(sqrt(0.000511*0.000511+pd1*pd1)+sqrt(0.000511*0.000511+pd2*pd2))-dauvecsum.Mag2());
				if( (v0masspipi>=(0.497614-mis_ks_range_) && v0masspipi<=(0.497614+mis_ks_range_)) || v0massee <= mis_ph_range_ ) continue;
			}

		} else {
			continue;
		}

		TVector3 secvec(v0.px(), v0.py(), v0.pz());
		TVector3 ptosvec( -displacementFromPV.x(), -displacementFromPV.y(), -displacementFromPV.z() );
		double ang = secvec.Angle(ptosvec);

		for ( auto const cut : cuts_ ) {
			if ( pt > cut.pTmax_ or pt < cut.pTmin_ ) continue;
			if ( eta > cut.Etamax_ or eta < cut.Etamin_ ) continue;
			if ( rapidity > cut.Rapmax_ or rapidity < cut.Rapmin_ ) continue;
			if ( mass > cut.Massmax_ or mass < cut.Massmin_ ) continue;

			if ( Chi2 > cut.Chi2max_ or Chi2 < cut.Chi2min_ ) continue;
			if ( Ndf  > cut.Ndfmax_  or Ndf  < cut.Ndfmin_  ) continue;

			if ( chi2oNDF > cut.Chi2oNdfmax_ or chi2oNDF < cut.Chi2oNdfmin_ ) continue;

			if ( lxy > cut.Lxymax_  or lxy < cut.Lxymin_ ) continue;
			if ( lxyz> cut.Lxyzmax_ or lxyz< cut.Lxyzmin_) continue;

			if ( cosThetaXY > cut.ThetaXYmax_  or cosThetaXY < cut.ThetaXYmin_ ) continue;
			if ( cosThetaXYZ> cut.ThetaXYZmax_ or cosThetaXYZ< cut.ThetaXYZmin_) continue;

			if ( vtxDecaySigXY > cut.DecaySigXYmax_  or vtxDecaySigXY < cut.DecaySigXYmin_ ) continue;
			if ( vtxDecaySigXYZ> cut.DecaySigXYZmax_ or vtxDecaySigXYZ< cut.DecaySigXYZmin_ ) continue;

			if ( dca > cut.DCAmax_ or dca < cut.DCAmin_ ) continue;

			if ( VtxProb > cut.VtxProbmax_ or VtxProb < cut.VtxProbmin_ ) continue;

			if ( ang > cut.AngleMax_ or ang < cut.AngleMin_ ) continue;

			pphi		->push_back( phi	);
			peta		->push_back( eta	);
			prapidity	->push_back( rapidity	);
			ppT		->push_back( pt		);
			pmass		->push_back( mass	);
			pweight		->push_back( 1.0	);

			pvtxChi2	->push_back( Chi2	);
			pvtxNdf		->push_back( Ndf	);
			pvtxChi2oNdf	->push_back( chi2oNDF		);

			pLxy		->push_back( lxy	);
			pLxyz		->push_back( lxyz	);


			pcosThetaXY	->push_back( cosThetaXY		);
			pcosThetaXYZ	->push_back( cosThetaXYZ	);


			pvtxDecaySigXY	->push_back( vtxDecaySigXY	);
			pvtxDecaySigXYZ	->push_back( vtxDecaySigXYZ	);

			pDCA	->push_back( dca	);
			ppdgId	->push_back( v0.pdgId()	);

			pV0	->push_back( v0 );

			if ( v0.numberOfDaughters() == 2 ) {
				for ( unsigned int i = 0; i < 2; i++ ) {
					auto dau = dynamic_cast<const RecoChargedCandidate*>(v0.daughter(i));
					double idx = -1.;
					if ( dau ) idx = dau->track().index();
					pRefs->push_back( idx );
				}
			}

			if ( v0.daughter(0)->charge() > 0 ) {
                auto trk = dynamic_cast<const reco::RecoChargedCandidate*>(v0.daughter(0))->track();
                ppTrkQuality ->push_back(trk->qualityMask());
                ppTrkNHit    ->push_back(trk->numberOfValidHits());
                ppTrkPx      ->push_back(trk->px());
                ppTrkPy      ->push_back(trk->py());
                ppTrkPz      ->push_back(trk->pz());
                ppTrkPt      ->push_back(trk->pt());
                ppTrkPhi     ->push_back(trk->phi());
                ppTrkPtError ->push_back(trk->ptError());
                ppTrkEta     ->push_back(trk->eta());
                ppTrkNPxLayer->push_back(trk->hitPattern().pixelLayersWithMeasurement());
                ppTrkDCASigXY->push_back(dauTransImpactSig1);
                ppTrkDCASigZ ->push_back(dauLongImpactSig1);
			} else {
                auto trk = dynamic_cast<const reco::RecoChargedCandidate*>(v0.daughter(0))->track();
                pnTrkQuality ->push_back(trk->qualityMask());
                pnTrkNHit    ->push_back(trk->numberOfValidHits());
                pnTrkPx      ->push_back(trk->px());
                pnTrkPy      ->push_back(trk->py());
                pnTrkPz      ->push_back(trk->pz());
                pnTrkPt      ->push_back(trk->pt());
                pnTrkPhi     ->push_back(trk->phi());
                pnTrkPtError ->push_back(trk->ptError());
                pnTrkEta     ->push_back(trk->eta());
                pnTrkNPxLayer->push_back(trk->hitPattern().pixelLayersWithMeasurement());
                pnTrkDCASigXY->push_back(dauTransImpactSig1);
                pnTrkDCASigZ ->push_back(dauLongImpactSig1);
			}
			if ( v0.daughter(1)->charge() > 0 ) {
                auto trk = dynamic_cast<const reco::RecoChargedCandidate*>(v0.daughter(1))->track();
                ppTrkQuality ->push_back(trk->qualityMask());
                ppTrkNHit    ->push_back(trk->numberOfValidHits());
                ppTrkPx      ->push_back(trk->px());
                ppTrkPy      ->push_back(trk->py());
                ppTrkPz      ->push_back(trk->pz());
                ppTrkPt      ->push_back(trk->pt());
                ppTrkPhi     ->push_back(trk->phi());
                ppTrkPtError ->push_back(trk->ptError());
                ppTrkEta     ->push_back(trk->eta());
                ppTrkNPxLayer->push_back(trk->hitPattern().pixelLayersWithMeasurement());
                ppTrkDCASigXY->push_back(dauTransImpactSig2);
                ppTrkDCASigZ ->push_back(dauLongImpactSig2);
            } else {
                auto trk = dynamic_cast<const reco::RecoChargedCandidate*>(v0.daughter(1))->track();
                pnTrkQuality ->push_back(trk->qualityMask());
                pnTrkNHit    ->push_back(trk->numberOfValidHits());
                pnTrkPx      ->push_back(trk->px());
                pnTrkPy      ->push_back(trk->py());
                pnTrkPz      ->push_back(trk->pz());
                pnTrkPt      ->push_back(trk->pt());
                pnTrkPhi     ->push_back(trk->phi());
                pnTrkPtError ->push_back(trk->ptError());
                pnTrkEta     ->push_back(trk->eta());
                pnTrkNPxLayer->push_back(trk->hitPattern().pixelLayersWithMeasurement());
                pnTrkDCASigXY->push_back(dauTransImpactSig2);
                pnTrkDCASigZ ->push_back(dauLongImpactSig2);
			}


			// boostToCM
			//std::cout << " ---> " << __LINE__ << " v0.pdgId() = " << v0.pdgId() << " v0.p4() = " << v0.p4() << std::endl;
			//std::cout << " -> " << __LINE__ << " d0.p4() = " << v0.daughter(0)->p4() << std::endl;
			//std::cout << " -> " << __LINE__ << " d1.p4() = " << v0.daughter(1)->p4() << std::endl;
			CenterOfMassBooster b(dynamic_cast<reco::Candidate&>(v0));
			b.set(dynamic_cast<reco::Candidate&>(v0));
			if ( v0.daughter(0)->charge() > 0 ) {
				ppPhiCM->push_back(v0.daughter(0)->phi());
				ppPxCM->push_back(v0.daughter(0)->px());
				ppPyCM->push_back(v0.daughter(0)->py());
				ppPzCM->push_back(v0.daughter(0)->pz());
			} else {
				pnPhiCM->push_back(v0.daughter(0)->phi());
				pnPxCM->push_back(v0.daughter(0)->px());
				pnPyCM->push_back(v0.daughter(0)->py());
				pnPzCM->push_back(v0.daughter(0)->pz());
			}
			if ( v0.daughter(1)->charge() > 0 ) {
				ppPhiCM->push_back(v0.daughter(1)->phi());
				ppPxCM->push_back(v0.daughter(1)->px());
				ppPyCM->push_back(v0.daughter(1)->py());
				ppPzCM->push_back(v0.daughter(1)->pz());
			} else {
				pnPhiCM->push_back(v0.daughter(1)->phi());
				pnPxCM->push_back(v0.daughter(1)->px());
				pnPyCM->push_back(v0.daughter(1)->py());
				pnPzCM->push_back(v0.daughter(1)->pz());
			}
//			std::cout << " ---> " << __LINE__ << " v0.pdgId() = " << v0.pdgId() << " v0.p4() = " << v0.p4() << std::endl;
//			std::cout << " after boost -> " << __LINE__ << " d0.p4() = " << v0.daughter(0)->p4() << " d0charge = " << v0.daughter(0)->charge() << std::endl;
//			std::cout << " after boost -> " << __LINE__ << " d1.p4() = " << v0.daughter(1)->p4() << " d1charge = " << v0.daughter(1)->charge() << std::endl;
			//std::cout << " -> " << __LINE__ << " boost d0.p4() = " << v0.daughter(0)->p4() << std::endl;
			//std::cout << " -> " << __LINE__ << " boost d1.p4() = " << v0.daughter(1)->p4() << std::endl;
		}
	}


	iEvent.put(std::move(pphi), std::string("phi"));
	iEvent.put(std::move(peta), std::string("eta"));
	iEvent.put(std::move(prapidity), std::string("rapidity"));
	iEvent.put(std::move(ppT), std::string("pt"));
	iEvent.put(std::move(pmass), std::string("mass"));
	iEvent.put(std::move(pweight), std::string("weight"));

	iEvent.put(std::move(pvtxChi2), std::string("vtxChi2"));
	iEvent.put(std::move(pvtxNdf), std::string("vtxNdf"));
	iEvent.put(std::move(pvtxChi2oNdf), std::string("vtxChi2oNdf"));

	iEvent.put(std::move(pLxy), std::string("Lxy"));
	iEvent.put(std::move(pLxyz), std::string("Lxyz"));

	iEvent.put(std::move(pcosThetaXY), std::string("cosThetaXY"));
	iEvent.put(std::move(pcosThetaXYZ), std::string("cosThetaXYZ"));

	iEvent.put(std::move(pvtxDecaySigXY), std::string("vtxDecaySigXY"));
	iEvent.put(std::move(pvtxDecaySigXYZ), std::string("vtxDecaySigXYZ"));

	iEvent.put(std::move(pDCA), std::string("DCA"));
	iEvent.put(std::move(pRefs), std::string("Refs"));

	iEvent.put(std::move(ppdgId), std::string("pdgId"));
	iEvent.put(std::move(ppPhiCM), std::string("pPhiCM"));
	iEvent.put(std::move(pnPhiCM), std::string("nPhiCM"));

	iEvent.put(std::move(ppPxCM), std::string("pPxCM"));
	iEvent.put(std::move(ppPyCM), std::string("pPyCM"));
	iEvent.put(std::move(ppPzCM), std::string("pPzCM"));
	iEvent.put(std::move(pnPxCM), std::string("nPxCM"));
	iEvent.put(std::move(pnPyCM), std::string("nPyCM"));
	iEvent.put(std::move(pnPzCM), std::string("nPzCM"));

    iEvent.put(std::move(ppTrkQuality),  std::string("pTrkQuality"));
    iEvent.put(std::move(ppTrkNHit),     std::string("pTrkNHit"));
    iEvent.put(std::move(ppTrkPx),       std::string("pTrkPx"));
    iEvent.put(std::move(ppTrkPy),       std::string("pTrkPy"));
    iEvent.put(std::move(ppTrkPz),       std::string("pTrkPz"));
    iEvent.put(std::move(ppTrkPt),       std::string("pTrkPt"));
    iEvent.put(std::move(ppTrkPhi),      std::string("pTrkPhi"));
    iEvent.put(std::move(ppTrkPtError),  std::string("pTrkPtError"));
    iEvent.put(std::move(ppTrkEta),      std::string("pTrkEta"));
    iEvent.put(std::move(ppTrkNPxLayer), std::string("pTrkNPxLayer"));
    iEvent.put(std::move(ppTrkDCASigXY), std::string("pTrkDCASigXY"));
    iEvent.put(std::move(ppTrkDCASigZ),  std::string("pTrkDCASigZ"));

    iEvent.put(std::move(pnTrkQuality),  std::string("nTrkQuality"));
    iEvent.put(std::move(pnTrkNHit),     std::string("nTrkNHit"));
    iEvent.put(std::move(pnTrkPx),       std::string("nTrkPx"));
    iEvent.put(std::move(pnTrkPy),       std::string("nTrkPy"));
    iEvent.put(std::move(pnTrkPz),       std::string("nTrkPz"));
    iEvent.put(std::move(pnTrkPt),       std::string("nTrkPt"));
    iEvent.put(std::move(pnTrkPhi),      std::string("nTrkPhi"));
    iEvent.put(std::move(pnTrkPtError),  std::string("nTrkPtError"));
    iEvent.put(std::move(pnTrkEta),      std::string("nTrkEta"));
    iEvent.put(std::move(pnTrkNPxLayer), std::string("nTrkNPxLayer"));
    iEvent.put(std::move(pnTrkDCASigXY), std::string("nTrkDCASigXY"));
    iEvent.put(std::move(pnTrkDCASigZ),  std::string("nTrkDCASigZ"));

	iEvent.put(std::move(pV0));
}


DEFINE_FWK_MODULE(QWV0VectProducer);
