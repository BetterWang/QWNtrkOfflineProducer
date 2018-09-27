#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "TFile.h"
#include "TH2.h"
#include "TMath.h"
#include <algorithm>

using namespace std;
class QWGenLmProducer : public edm::EDProducer {
public:
	explicit QWGenLmProducer(const edm::ParameterSet&);
	~QWGenLmProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///

	edm::InputTag	trackSrc_;

	double	pTmin_;
	double	pTmax_;
	double	Etamin_;
	double	Etamax_;
	bool	isPrompt_;

};

QWGenLmProducer::QWGenLmProducer(const edm::ParameterSet& pset) :
	trackSrc_(pset.getUntrackedParameter<edm::InputTag>("trackSrc"))
{
	consumes<reco::GenParticleCollection>(trackSrc_);

	pTmin_ = pset.getUntrackedParameter<double>("ptMin", 0.3);
	pTmax_ = pset.getUntrackedParameter<double>("ptMax", 3.0);
	Etamin_ = pset.getUntrackedParameter<double>("Etamin", -2.4);
	Etamax_ = pset.getUntrackedParameter<double>("Etamax", 2.4);
	isPrompt_ = pset.getUntrackedParameter<bool>("isPrompt", true);
	doFilterPdg_ = pset.getUntrackedParameter<bool>("doFilterPdg", false);

	produces<std::vector<double> >("phi");
	produces<std::vector<double> >("eta");
	produces<std::vector<double> >("rapidity");
	produces<std::vector<double> >("pt");
	produces<std::vector<double> >("mass");

	produces<std::vector<double> >("pdgId");
	produces<std::vector<double> >("pPhiCM");
	produces<std::vector<double> >("nPhiCM");
}

QWGenLmProducer::~QWGenLmProducer()
{
	return;
}

void QWGenLmProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;

	std::unique_ptr<std::vector<double> > pphi( new std::vector<double> );
	std::unique_ptr<std::vector<double> > peta( new std::vector<double> );
	std::unique_ptr<std::vector<double> > prapidity( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppT( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pmass( new std::vector<double> );

	std::unique_ptr<std::vector<double> > ppdgId( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppPhiCM( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pnPhiCM( new std::vector<double> );

	pvz->push_back(0.0);

	Handle<GenParticleCollection> tracks;
	iEvent.getByLabel(trackSrc_,tracks);

	for(GenParticleCollection::const_iterator itTrack = tracks->begin();
			itTrack != tracks->end();
			++itTrack) {
		if ( itTrack->status() != 1 ) continue;
		if ( isPrompt_ and (not itTrack->isPromptFinalState()) ) continue;
		if ( itTrack->eta() > Etamax_ or itTrack->eta() < Etamin_ or itTrack->pt() > pTmax_ or itTrack->pt() < pTmin_ ) continue;

		if ( itTrack->pdgId() == 3122 ) {
			if ( itTrack->numberOfDaughters() != 2 ) continue;
			if ( !(itTrack->daughter(0)->pdgId() == 2212 and itTrack->daughter(1)->pdgId() == -211) and
				!(itTrack->daughter(1)->pdgId() == 2212 and itTrack->daughter(0)->pdgId() == -211) ) continue;
		}
		if ( itTrack->pdgId() == -3122 ) {
			if ( itTrack->numberOfDaughters() != 2 ) continue;
			if ( !(itTrack->daughter(0)->pdgId() == -2212 and itTrack->daughter(1)->pdgId() == 211) and
				!(itTrack->daughter(1)->pdgId() == -2212 and itTrack->daughter(0)->pdgId() == 211) ) continue;
		}

		pphi->push_back(itTrack->phi());
		peta->push_back(itTrack->eta());
		prapidity->push_back(itTrack->rapidity());
		ppT->push_back(itTrack->pt());
		pmass->push_back(itTrack->mass());
		ppdg->push_back(itTrack->pdgId());

		CenterOfMassBooster b(dynamic_cast<reco::Candidate&>(*itTrack));
		b.set(dynamic_cast<reco::Candidate&>(*itTrack));
		if ( itTrack->pdgId() == 3122 ) {
			if ( itTrack->daughter(0)->pdgId() == 2212 and itTrack->daughter(1)->pdgId() == -211 ) {
				ppPhiCM->push_back(itTrack->daughter(0)->phi());
				pnPhiCM->push_back(itTrack->daughter(1)->phi());
			} else {
				ppPhiCM->push_back(itTrack->daughter(1)->phi());
				pnPhiCM->push_back(itTrack->daughter(0)->phi());
			}
		}

		if ( itTrack->pdgId() == -3122 ) {
			if ( itTrack->daughter(0)->pdgId() == -2212 and itTrack->daughter(1)->pdgId() == 211 ) {
				ppPhiCM->push_back(itTrack->daughter(1)->phi());
				pnPhiCM->push_back(itTrack->daughter(0)->phi());
			} else {
				ppPhiCM->push_back(itTrack->daughter(0)->phi());
				pnPhiCM->push_back(itTrack->daughter(1)->phi());
			}
		}

	}
	iEvent.put(move(pphi), std::string("phi"));
	iEvent.put(move(peta), std::string("eta"));
	iEvent.put(move(ppT), std::string("pt"));
	iEvent.put(move(prapidity), std::string("rapidity"));
	iEvent.put(move(pmass), std::string("mass"));
	iEvent.put(move(ppdg), std::string("pdgId"));
	iEvent.put(std::move(ppPhiCM), std::string("pPhiCM"));
	iEvent.put(std::move(pnPhiCM), std::string("nPhiCM"));
}


DEFINE_FWK_MODULE(QWGenLmProducer);
