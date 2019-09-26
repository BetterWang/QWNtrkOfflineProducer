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

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/TrackAssociation.h"


#include "TFile.h"
#include "TH2.h"
#include "TMath.h"

using namespace std;
class QWV0MCSignalSelector : public edm::EDProducer {
public:
	explicit QWV0MCSignalSelector(const edm::ParameterSet&);
	~QWV0MCSignalSelector();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///
    edm::EDGetTokenT<reco::RecoToSimCollection> recoRecoToSimCollectionToken_;
    edm::EDGetTokenT<reco::VertexCompositeCandidateCollection> Ks_Token_;
    edm::EDGetTokenT<reco::VertexCompositeCandidateCollection> Lm_Token_;
    bool    bSelectBkg_;
};

QWV0MCSignalSelector::QWV0MCSignalSelector(const edm::ParameterSet& pset) :
    recoRecoToSimCollectionToken_(consumes<reco::RecoToSimCollection>(
                pset.getUntrackedParameter<edm::InputTag>("trackAssociatorMap"))),
    Ks_Token_(consumes<reco::VertexCompositeCandidateCollection>(
                pset.getUntrackedParameter<edm::InputTag>(
                    "KsCollection"))),
    Lm_Token_(consumes<reco::VertexCompositeCandidateCollection>(
                pset.getUntrackedParameter<edm::InputTag>(
                    "LmCollection"))),
    bSelectBkg_(pset.getUntrackedParameter<bool>("SelectBackground", false))
{
	produces<reco::VertexCompositeCandidateCollection>("Kshort");
	produces<reco::VertexCompositeCandidateCollection>("Lambda");
}

QWV0MCSignalSelector::~QWV0MCSignalSelector()
{
	return;
}

void QWV0MCSignalSelector::produce(edm::Event& iEvent, const edm::EventSetup& iConfig)
{
	using namespace edm;

    // Make matching collections
    Handle<reco::RecoToSimCollection> recotosimCollectionH;
    iEvent.getByToken(recoRecoToSimCollectionToken_, recotosimCollectionH);

    // get the V0s;
    Handle<reco::VertexCompositeCandidateCollection> KsH;
    Handle<reco::VertexCompositeCandidateCollection> LmH;
    iEvent.getByToken(Ks_Token_, KsH);
    iEvent.getByToken(Lm_Token_, LmH);

    std::unique_ptr<reco::VertexCompositeCandidateCollection> Ks_sig(new reco::VertexCompositeCandidateCollection);
    std::unique_ptr<reco::VertexCompositeCandidateCollection> Lm_sig(new reco::VertexCompositeCandidateCollection);

    if ( KsH.isValid() ) {
        for ( reco::VertexCompositeCandidateCollection::const_iterator iCand = KsH->begin();
                iCand != KsH->end(); iCand++ ) {
            bool isTruth = false;
            RefToBase<reco::Track> d0((*(dynamic_cast<const reco::RecoChargedCandidate*>(iCand->daughter(0)))).track());
            RefToBase<reco::Track> d1((*(dynamic_cast<const reco::RecoChargedCandidate*>(iCand->daughter(1)))).track());

            if ( (recotosimCollectionH->find(d0) != recotosimCollectionH->end()) and
                 (recotosimCollectionH->find(d1) != recotosimCollectionH->end()) ) {

                const std::vector<std::pair<TrackingParticleRef, double> >& tp0 = (*recotosimCollectionH)[d0];
                const std::vector<std::pair<TrackingParticleRef, double> >& tp1 = (*recotosimCollectionH)[d1];
                if ( (!tp0.empty()) and (!tp1.empty()) ) {
                    auto tp0ref = tp0.begin()->first;
                    auto tp1ref = tp1.begin()->first;
                    TrackingVertexRef parentVertex0 = tp0ref->parentVertex();
                    TrackingVertexRef parentVertex1 = tp1ref->parentVertex();
                    if ( parentVertex0.isNonnull() and parentVertex1.isNonnull() and (parentVertex0->position() == parentVertex1->position() ) ) {
                        if ( parentVertex0->nDaughterTracks() == 2 ) {
                            for (TrackingVertex::tp_iterator iTP = parentVertex0->sourceTracks_begin(); iTP != parentVertex0->sourceTracks_end(); iTP++) {
                                if ( fabs((*iTP)->pdgId()) == 310 && (fabs(tp0ref->pdgId())==211) && (fabs(tp1ref->pdgId())==211) ) {
                                    isTruth = true;
                                }
                            }
                        }
                    }
                }
            }
            if ( bSelectBkg_ ) {
                if ( !isTruth ) Ks_sig->push_back( *iCand );
            } else {
                if ( isTruth ) Ks_sig->push_back( *iCand );
            }
        }
    }

    if ( LmH.isValid() ) {
        for ( reco::VertexCompositeCandidateCollection::const_iterator iCand = LmH->begin();
                iCand != LmH->end(); iCand++ ) {
            bool isTruth = false;
            RefToBase<reco::Track> d0((*(dynamic_cast<const reco::RecoChargedCandidate*>(iCand->daughter(0)))).track());
            RefToBase<reco::Track> d1((*(dynamic_cast<const reco::RecoChargedCandidate*>(iCand->daughter(1)))).track());

            if ( (recotosimCollectionH->find(d0) != recotosimCollectionH->end()) and
                 (recotosimCollectionH->find(d1) != recotosimCollectionH->end()) ) {

                const std::vector<std::pair<TrackingParticleRef, double> >& tp0 = (*recotosimCollectionH)[d0];
                const std::vector<std::pair<TrackingParticleRef, double> >& tp1 = (*recotosimCollectionH)[d1];
                if ( (!tp0.empty()) and (!tp1.empty()) ) {
                    auto tp0ref = tp0.begin()->first;
                    auto tp1ref = tp1.begin()->first;
                    TrackingVertexRef parentVertex0 = tp0ref->parentVertex();
                    TrackingVertexRef parentVertex1 = tp1ref->parentVertex();
                    if ( parentVertex0.isNonnull() and parentVertex1.isNonnull() and (parentVertex0->position() == parentVertex1->position() ) ) {
                        if ( parentVertex0->nDaughterTracks() == 2 ) {
                            for (TrackingVertex::tp_iterator iTP = parentVertex0->sourceTracks_begin(); iTP != parentVertex0->sourceTracks_end(); iTP++) {
                                if ( ((*iTP)->pdgId() == 3122) and
                                     ( ( (tp0ref->pdgId()==2212) and (tp1ref->pdgId()==-211) ) or
                                     ( (tp1ref->pdgId()==2212) and (tp0ref->pdgId()==-211) ) ) ) {
                                    isTruth = true;
                                }
                                if ( ((*iTP)->pdgId() == -3122) and
                                     ( ( (tp0ref->pdgId()==-2212) and (tp1ref->pdgId()==211) ) or
                                     ( (tp1ref->pdgId()==-2212) and (tp0ref->pdgId()==211) ) ) ) {
                                    isTruth = true;
                                }
                            }
                        }
                    }
                }
            }
            if ( bSelectBkg_ ) {
                if ( !isTruth ) Lm_sig->push_back( *iCand );
            } else {
                if ( isTruth ) Lm_sig->push_back( *iCand );
            }
        }
    }

    iEvent.put(std::move(Ks_sig), "Kshort");
    iEvent.put(std::move(Lm_sig), "Lambda");
}



DEFINE_FWK_MODULE(QWV0MCSignalSelector);
