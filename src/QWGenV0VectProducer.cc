#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/ESHandle.h"
//#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
//#include "TrackingTools/Records/interface/TransientTrackRecord.h"
//#include "MagneticField/Engine/interface/MagneticField.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

//#include "DataFormats/TrackReco/interface/Track.h"
//#include "DataFormats/TrackReco/interface/TrackFwd.h"
//#include "DataFormats/VertexReco/interface/Vertex.h"
//#include "DataFormats/VertexReco/interface/VertexFwd.h"
//#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"

//#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
//#include "DataFormats/Candidate/interface/VertexCompositeCandidateFwd.h"
//#include "TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"

#include "DataFormats/Math/interface/Vector3D.h"

#include <Math/SMatrix.h>
#include <Math/SVector.h>
#include <Math/VectorUtil.h>

typedef ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> > SMatrixSym3D;
typedef ROOT::Math::SVector<double, 3> SVector3;

using namespace std;
class QWGenV0VectProducer : public edm::EDProducer {
public:
	explicit QWGenV0VectProducer(const edm::ParameterSet&);
	~QWGenV0VectProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
    edm::EDGetTokenT<TrackingVertexCollection> trackingVertexCollection_Token_;
    int parent_pdgId;
    int daughter_pdgId1;
    int daughter_pdgId2;
	///


};

QWGenV0VectProducer::QWGenV0VectProducer(const edm::ParameterSet& pset) :
    trackingVertexCollection_Token_(consumes<TrackingVertexCollection>(pset.getUntrackedParameter<edm::InputTag>("trackingVertexCollection"))),
    parent_pdgId(pset.getUntrackedParameter<int>("parent_pdgId")),
    daughter_pdgId1(pset.getUntrackedParameter<int>("daughter_pdgId1")),
    daughter_pdgId2(pset.getUntrackedParameter<int>("daughter_pdgId2"))
{
	produces<std::vector<double> >("phi");
	produces<std::vector<double> >("eta");
	produces<std::vector<double> >("rapidity");
	produces<std::vector<double> >("pt");
	produces<std::vector<double> >("mass");

	produces<std::vector<double> >("pdgId");

	produces<std::vector<double> >("pPt");
	produces<std::vector<double> >("pEta");
	produces<std::vector<double> >("pPx");
	produces<std::vector<double> >("pPy");
	produces<std::vector<double> >("pPz");

	produces<std::vector<double> >("nPt");
	produces<std::vector<double> >("nEta");
	produces<std::vector<double> >("nPx");
	produces<std::vector<double> >("nPy");
	produces<std::vector<double> >("nPz");

	produces<std::vector<double> >("pPxCM");
	produces<std::vector<double> >("pPyCM");
	produces<std::vector<double> >("pPzCM");
	produces<std::vector<double> >("nPxCM");
	produces<std::vector<double> >("nPyCM");
	produces<std::vector<double> >("nPzCM");

}

QWGenV0VectProducer::~QWGenV0VectProducer()
{
	return;
}

void QWGenV0VectProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	std::unique_ptr<std::vector<double> > pphi( new std::vector<double> );
	std::unique_ptr<std::vector<double> > peta( new std::vector<double> );
	std::unique_ptr<std::vector<double> > prapidity( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppt( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pmass( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppdgId( new std::vector<double> );

	std::unique_ptr<std::vector<double> > ppPt( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppEta( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppPx( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppPy( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppPz( new std::vector<double> );

	std::unique_ptr<std::vector<double> > pnPt( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnEta( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnPx( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnPy( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnPz( new std::vector<double> );

	std::unique_ptr<std::vector<double> > ppPxCM( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppPyCM( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppPzCM( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnPxCM( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnPyCM( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnPzCM( new std::vector<double> );

    // Get Monte Carlo information
    edm::Handle<TrackingVertexCollection> TVCollectionH;
    iEvent.getByToken(trackingVertexCollection_Token_, TVCollectionH);

    for (auto const& gen_vertex : *TVCollectionH.product()) {
        if (gen_vertex.eventId().bunchCrossing() != 0) continue;  // Consider only in-time events
        if (gen_vertex.nDaughterTracks() != 2) continue;  // Keep only V0 vertices

        TrackingVertex::tp_iterator source = gen_vertex.sourceTracks_begin();
        if ( source == gen_vertex.sourceTracks_end() ) continue;
        if ( std::abs( (*source)->pdgId() ) != parent_pdgId ) continue; // Keep only the parent_pdgId

        int d0 = gen_vertex.daughterTracks().at(0)->pdgId();
        int d1 = gen_vertex.daughterTracks().at(1)->pdgId();
        auto tp0 = gen_vertex.daughterTracks().at(0);
        auto tp1 = gen_vertex.daughterTracks().at(1);

        if ( (std::abs(tp0->eta()) > 2.4) or (std::abs(tp1->eta()) > 2.4) ) continue; // daughters have to be in the detector

        if ( ((std::abs(d0) == daughter_pdgId1) and (std::abs(d1) == daughter_pdgId2)) or
             ((std::abs(d0) == daughter_pdgId2) and (std::abs(d1) == daughter_pdgId1)) ) {

            pphi->push_back( (*source)->phi() );
            peta->push_back( (*source)->eta() );
            prapidity->push_back( (*source)->rapidity() );
            ppt->push_back( (*source)->pt() );
            pmass->push_back( (*source)->mass() );
            ppdgId->push_back( (*source)->pdgId() );

            math::XYZVectorD boost( (*source)->boostToCM() );
            auto tp0p4 = tp0->p4();
            auto tp1p4 = tp1->p4();
            auto tp0p4CM = ROOT::Math::VectorUtil::boost( tp0p4, boost );
            auto tp1p4CM = ROOT::Math::VectorUtil::boost( tp1p4, boost );

            if ( (tp0->charge()>0) and (tp1->charge()<0) ) {
                ppPt->push_back( tp0->pt() );
                ppEta->push_back( tp0->eta() );
                ppPx->push_back( tp0->px() );
                ppPy->push_back( tp0->py() );
                ppPz->push_back( tp0->pz() );

                pnPt->push_back( tp1->pt() );
                pnEta->push_back( tp1->eta() );
                pnPx->push_back( tp1->px() );
                pnPy->push_back( tp1->py() );
                pnPz->push_back( tp1->pz() );

                ppPxCM->push_back( tp0p4CM.Px() );
                ppPyCM->push_back( tp0p4CM.Py() );
                ppPzCM->push_back( tp0p4CM.Pz() );

                pnPxCM->push_back( tp1p4CM.Px() );
                pnPyCM->push_back( tp1p4CM.Py() );
                pnPzCM->push_back( tp1p4CM.Pz() );
            } else if ( (tp0->charge()<0) and (tp1->charge()>0) ) {
                ppPt->push_back( tp1->pt() );
                ppEta->push_back( tp1->eta() );
                ppPx->push_back( tp1->px() );
                ppPy->push_back( tp1->py() );
                ppPz->push_back( tp1->pz() );

                pnPt->push_back( tp0->pt() );
                pnEta->push_back( tp0->eta() );
                pnPx->push_back( tp0->px() );
                pnPy->push_back( tp0->py() );
                pnPz->push_back( tp0->pz() );

                ppPxCM->push_back( tp1p4CM.Px() );
                ppPyCM->push_back( tp1p4CM.Py() );
                ppPzCM->push_back( tp1p4CM.Pz() );

                pnPxCM->push_back( tp0p4CM.Px() );
                pnPyCM->push_back( tp0p4CM.Py() );
                pnPzCM->push_back( tp0p4CM.Pz() );
            }
        }
    }

	iEvent.put(std::move(pphi), std::string("phi"));
	iEvent.put(std::move(peta), std::string("eta"));
	iEvent.put(std::move(prapidity), std::string("rapidity"));
	iEvent.put(std::move(ppt), std::string("pt"));
	iEvent.put(std::move(pmass), std::string("mass"));
	iEvent.put(std::move(ppdgId), std::string("pdgId"));

	iEvent.put(std::move(ppPt), std::string("pPt"));
	iEvent.put(std::move(ppEta), std::string("pEta"));
	iEvent.put(std::move(ppPx), std::string("pPx"));
	iEvent.put(std::move(ppPy), std::string("pPy"));
	iEvent.put(std::move(ppPz), std::string("pPz"));

	iEvent.put(std::move(pnPt), std::string("nPt"));
	iEvent.put(std::move(pnEta), std::string("nEta"));
	iEvent.put(std::move(pnPx), std::string("nPx"));
	iEvent.put(std::move(pnPy), std::string("nPy"));
	iEvent.put(std::move(pnPz), std::string("nPz"));

	iEvent.put(std::move(ppPxCM), std::string("pPxCM"));
	iEvent.put(std::move(ppPyCM), std::string("pPyCM"));
	iEvent.put(std::move(ppPzCM), std::string("pPzCM"));
	iEvent.put(std::move(pnPxCM), std::string("nPxCM"));
	iEvent.put(std::move(pnPyCM), std::string("nPyCM"));
	iEvent.put(std::move(pnPzCM), std::string("nPzCM"));
}


DEFINE_FWK_MODULE(QWGenV0VectProducer);
