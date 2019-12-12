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
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

typedef ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> > SMatrixSym3D;
typedef ROOT::Math::SVector<double, 3> SVector3;

using namespace std;
class QWV0MVAVectProducer : public edm::EDProducer {
public:
	explicit QWV0MVAVectProducer(const edm::ParameterSet&);
	~QWV0MVAVectProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///

	edm::InputTag	V0Src_;
	edm::InputTag	vertexSrc_;
	edm::InputTag	dbCent_;
    std::string mvaXML_;
    double mvaCut_;
    double ptMin_;
    double ptMax_;
    double rapMin_;
    double rapMax_;

    Float_t pt_;
    Float_t rapidity_;
    Float_t vtxChi2_;
    Float_t cosThetaXYZ_;
    Float_t Lxyz_;
    Float_t vtxDecaySigXYZ_;
    Float_t DCA_;
    Float_t pTrkNHit_;
    Float_t pTrkPt_;
    Float_t pTrkPtError_;
    Float_t pTrkEta_;
    Float_t pTrkNPxLayer_;
    Float_t pTrkDCASigXY_;
    Float_t pTrkDCASigZ_;
    Float_t nTrkNHit_;
    Float_t nTrkPt_;
    Float_t nTrkPtError_;
    Float_t nTrkEta_;
    Float_t nTrkNPxLayer_;
    Float_t nTrkDCASigXY_;
    Float_t nTrkDCASigZ_;
    Float_t Cent_;

    Float_t mass_;
    Float_t eta_;
    Float_t phi_;
    Float_t pdgId_;

    TMVA::Reader *reader;
};

QWV0MVAVectProducer::QWV0MVAVectProducer(const edm::ParameterSet& pset) :
	V0Src_(pset.getUntrackedParameter<edm::InputTag>("V0Src")),
	vertexSrc_(pset.getUntrackedParameter<edm::InputTag>("vertexSrc")),
	dbCent_(pset.getUntrackedParameter<edm::InputTag>("dbCent")),
	mvaXML_(pset.getUntrackedParameter<std::string>("mvaXML")),
	mvaCut_(pset.getUntrackedParameter<double>("mvaCut")),
	ptMin_(pset.getUntrackedParameter<double>("ptMin", 0.2)),
	ptMax_(pset.getUntrackedParameter<double>("ptMax", 8.5)),
	rapMin_(pset.getUntrackedParameter<double>("rapMin", -2.5)),
	rapMax_(pset.getUntrackedParameter<double>("rapMax",  2.5))
{
    TMVA::Tools::Instance();
    reader = new TMVA::Reader( "!Color:!Silent" );

    reader->AddVariable( "pt",               &pt_             );
    reader->AddVariable( "rapidity",         &rapidity_       );
    reader->AddVariable( "vtxChi2",          &vtxChi2_        );
    reader->AddVariable( "cosThetaXYZ",      &cosThetaXYZ_    );
    reader->AddVariable( "Lxyz",             &Lxyz_           );
    reader->AddVariable( "vtxDecaySigXYZ",   &vtxDecaySigXYZ_ );
    reader->AddVariable( "DCA",              &DCA_            );
    reader->AddVariable( "pTrkNHit",         &pTrkNHit_       );
    reader->AddVariable( "pTrkPt",           &pTrkPt_         );
    reader->AddVariable( "pTrkPtError",      &pTrkPtError_    );
    reader->AddVariable( "pTrkEta",          &pTrkEta_        );
    reader->AddVariable( "pTrkNPxLayer",     &pTrkNPxLayer_   );
    reader->AddVariable( "pTrkDCASigXY",     &pTrkDCASigXY_   );
    reader->AddVariable( "pTrkDCASigZ",      &pTrkDCASigZ_    );
    reader->AddVariable( "nTrkNHit",         &nTrkNHit_       );
    reader->AddVariable( "nTrkPt",           &nTrkPt_         );
    reader->AddVariable( "nTrkPtError",      &nTrkPtError_    );
    reader->AddVariable( "nTrkEta",          &nTrkEta_        );
    reader->AddVariable( "nTrkNPxLayer",     &nTrkNPxLayer_   );
    reader->AddVariable( "nTrkDCASigXY",     &nTrkDCASigXY_   );
    reader->AddVariable( "nTrkDCASigZ",      &nTrkDCASigZ_    );
    reader->AddVariable( "Cent",             &Cent_           );

    reader->AddSpectator( "mass",   &mass_);
    reader->AddSpectator( "eta",    &eta_);
    reader->AddSpectator( "phi",    &phi_);
    reader->AddSpectator( "pdgId",  &pdgId_);

    reader->BookMVA( string("BDT")+V0Src_.instance(), mvaXML_ );

	consumes<reco::VertexCompositeCandidateCollection>(V0Src_);
	consumes<reco::VertexCollection>(vertexSrc_);
	consumes<double>(dbCent_);

	produces<reco::VertexCompositeCandidateCollection>(V0Src_.instance());
}

QWV0MVAVectProducer::~QWV0MVAVectProducer()
{
	return;
}

void QWV0MVAVectProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;

	std::unique_ptr< VertexCompositeCandidateCollection > pV0( new VertexCompositeCandidateCollection );

	edm::ESHandle<TransientTrackBuilder> theB;
	iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);


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
		iEvent.put(std::move(pV0), V0Src_.instance());
		return;
	}

	reco::Vertex const * pv = &recoVertices[primaryvtx];

	Handle<VertexCompositeCandidateCollection> V0s;
	iEvent.getByLabel(V0Src_, V0s);
	edm::Handle<double> pCent;
	iEvent.getByLabel(dbCent_, pCent);
    Cent_ = Float_t(*pCent);

	for ( auto v0 : (*V0s) ) {
		mass_     = v0.mass();
		pt_       = v0.pt();
		eta_      = v0.eta();
		rapidity_ = v0.rapidity();
		phi_      = v0.phi();
        pdgId_    = v0.pdgId();
		vtxChi2_ = v0.vertexChi2();

        if ( (pt_ > ptMax_) or (pt_ < ptMin_) or (rapidity_ > rapMax_) or (rapidity_ < rapMin_) ) continue;

		GlobalPoint displacementFromPV ( (pv->x() - v0.vx()), (pv->y() - v0.vy()), (pv->z() - v0.vz()) ) ;
		Lxyz_    = ( displacementFromPV.mag() );

        cosThetaXYZ_ = -(displacementFromPV.x()*v0.px() + displacementFromPV.y()*v0.py() + displacementFromPV.z()*v0.pz()) / ( displacementFromPV.mag()*sqrt(v0.px()*v0.px() + v0.py()*v0.py() + v0.pz()*v0.pz()) );

		SMatrixSym3D totalCov = pv->covariance() + v0.vertexCovariance();

		SVector3 distVecXYZ(displacementFromPV.x(), displacementFromPV.y(), displacementFromPV.z());
		double distMagXYZ = ROOT::Math::Mag(distVecXYZ);
		double sigmaDistMagXYZ = sqrt(ROOT::Math::Similarity(totalCov, distVecXYZ)) / distMagXYZ;
		vtxDecaySigXYZ_ = distMagXYZ/sigmaDistMagXYZ;

		DCA_ = -999.;
        double dauLongImpactSig1 = -999.;
        double dauTransImpactSig1 = -999.;
        double dauLongImpactSig2 = -999.;
        double dauTransImpactSig2 = -999.;
		if ( v0.numberOfDaughters() != 2 ) continue;

			auto bT0 = v0.daughter(0)->bestTrack();
			auto bT1 = v0.daughter(1)->bestTrack();

			if ( bT0 and bT1 ) {
				auto tt0 = theB->build(bT0).initialFreeState();
				auto tt1 = theB->build(bT1).initialFreeState();

				ClosestApproachInRPhi cApp;
				cApp.calculate(tt0, tt1);
				if (cApp.status()) {
					DCA_ = std::abs(cApp.distance());
				}
			}


			auto d1 = v0.daughter(0)->get<reco::TrackRef>();
			auto d2 = v0.daughter(1)->get<reco::TrackRef>();

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

            const double mis_ks_range_ = 0.020;
            const double mis_la_range_ = 0.010;
            const double mis_ph_range_ = 0.015;
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

			if ( v0.daughter(0)->charge() > 0 ) {
                auto trk = dynamic_cast<const reco::RecoChargedCandidate*>(v0.daughter(0))->track();
                pTrkNHit_     = trk->numberOfValidHits();
                pTrkPt_       = trk->pt();
                pTrkPtError_  = trk->ptError();
                pTrkEta_      = trk->eta();
                pTrkNPxLayer_ = trk->hitPattern().pixelLayersWithMeasurement();
                pTrkDCASigXY_ = dauTransImpactSig1;
                pTrkDCASigZ_  = dauLongImpactSig1;
			} else {
                auto trk = dynamic_cast<const reco::RecoChargedCandidate*>(v0.daughter(0))->track();
                nTrkNHit_     = trk->numberOfValidHits();
                nTrkPt_       = (trk->pt());
                nTrkPtError_  = (trk->ptError());
                nTrkEta_      = (trk->eta());
                nTrkNPxLayer_ = (trk->hitPattern().pixelLayersWithMeasurement());
                nTrkDCASigXY_ = (dauTransImpactSig1);
                nTrkDCASigZ_  = (dauLongImpactSig1);
			}
			if ( v0.daughter(1)->charge() > 0 ) {
                auto trk = dynamic_cast<const reco::RecoChargedCandidate*>(v0.daughter(1))->track();
                pTrkNHit_     = trk->numberOfValidHits();
                pTrkPt_       = trk->pt();
                pTrkPtError_  = trk->ptError();
                pTrkEta_      = trk->eta();
                pTrkNPxLayer_ = trk->hitPattern().pixelLayersWithMeasurement();
                pTrkDCASigXY_ = dauTransImpactSig2;
                pTrkDCASigZ_  = dauLongImpactSig2;
            } else {
                auto trk = dynamic_cast<const reco::RecoChargedCandidate*>(v0.daughter(1))->track();
                nTrkNHit_     = trk->numberOfValidHits();
                nTrkPt_       = (trk->pt());
                nTrkPtError_  = (trk->ptError());
                nTrkEta_      = (trk->eta());
                nTrkNPxLayer_ = (trk->hitPattern().pixelLayersWithMeasurement());
                nTrkDCASigXY_ = (dauTransImpactSig2);
                nTrkDCASigZ_  = (dauLongImpactSig2);
			}

            double mva = reader->EvaluateMVA(string("BDT")+V0Src_.instance());
//            std::cout    << " pt_             = " << pt_             << std::endl;
//            std::cout    << " rapidity_       = " << rapidity_       << std::endl;
//            std::cout    << " vtxChi2_        = " << vtxChi2_        << std::endl;
//            std::cout    << " cosThetaXYZ_    = " << cosThetaXYZ_    << std::endl;
//            std::cout    << " Lxyz_           = " << Lxyz_           << std::endl;
//            std::cout    << " vtxDecaySigXYZ_ = " << vtxDecaySigXYZ_ << std::endl;
//            std::cout    << " DCA_            = " << DCA_            << std::endl;
//            std::cout    << " pTrkNHit_       = " << pTrkNHit_       << std::endl;
//            std::cout    << " pTrkPt_         = " << pTrkPt_         << std::endl;
//            std::cout    << " pTrkPtError_    = " << pTrkPtError_    << std::endl;
//            std::cout    << " pTrkEta_        = " << pTrkEta_        << std::endl;
//            std::cout    << " pTrkNPxLayer_   = " << pTrkNPxLayer_   << std::endl;
//            std::cout    << " pTrkDCASigXY_   = " << pTrkDCASigXY_   << std::endl;
//            std::cout    << " pTrkDCASigZ_    = " << pTrkDCASigZ_    << std::endl;
//            std::cout    << " nTrkNHit_       = " << nTrkNHit_       << std::endl;
//            std::cout    << " nTrkPt_         = " << nTrkPt_         << std::endl;
//            std::cout    << " nTrkPtError_    = " << nTrkPtError_    << std::endl;
//            std::cout    << " nTrkEta_        = " << nTrkEta_        << std::endl;
//            std::cout    << " nTrkNPxLayer_   = " << nTrkNPxLayer_   << std::endl;
//            std::cout    << " nTrkDCASigXY_   = " << nTrkDCASigXY_   << std::endl;
//            std::cout    << " nTrkDCASigZ_    = " << nTrkDCASigZ_    << std::endl;

            if ( mva > mvaCut_ ) {
//            std::cout    << " Cent_           = " << Cent_           << std::endl;
//            std::cout    << " mva             = " << mva             << std::endl;
//            std::cout    << " mass_           = " << mass_           << std::endl;
                pV0	->push_back( v0 );
            }
	}

	iEvent.put(std::move(pV0), V0Src_.instance());
}


DEFINE_FWK_MODULE(QWV0MVAVectProducer);
