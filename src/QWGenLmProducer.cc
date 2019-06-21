#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "CommonTools/CandUtils/interface/CenterOfMassBooster.h"

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

//	double	pTmin_;
//	double	pTmax_;
//	double	Etamin_;
//	double	Etamax_;
//	bool	isPrompt_;

};

QWGenLmProducer::QWGenLmProducer(const edm::ParameterSet& pset) :
	trackSrc_(pset.getUntrackedParameter<edm::InputTag>("trackSrc"))
{
	consumes<reco::GenParticleCollection>(trackSrc_);

//	pTmin_ = pset.getUntrackedParameter<double>("ptMin", 0.3);
//	pTmax_ = pset.getUntrackedParameter<double>("ptMax", 3.0);
//	Etamin_ = pset.getUntrackedParameter<double>("Etamin", -2.4);
//	Etamax_ = pset.getUntrackedParameter<double>("Etamax", 2.4);
//	isPrompt_ = pset.getUntrackedParameter<bool>("isPrompt", true);

//	produces<std::vector<double> >("phi");
//	produces<std::vector<double> >("eta");
//	produces<std::vector<double> >("rapidity");
//	produces<std::vector<double> >("pt");
//	produces<std::vector<double> >("mass");
//
//	produces<std::vector<double> >("pdgId");
//	produces<std::vector<double> >("pPhiCM");
//	produces<std::vector<double> >("nPhiCM");


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

	Handle<GenParticleCollection> tracks;
	iEvent.getByLabel(trackSrc_,tracks);

	for(GenParticleCollection::const_iterator itTrack = tracks->begin();
			itTrack != tracks->end();
			++itTrack) {
		if ( itTrack->pdgId() != 3122 and itTrack->pdgId() != -3122 ) continue;
        if ( abs(itTrack->eta()) > 1.2 ) continue;
		if ( itTrack->status() != 1 ) {
//            cout << __LINE__ << endl;
            continue;
        }

//		if ( itTrack->isPromptFinalState() ) {
//            cout << __LINE__ << endl;
//            continue;
//        }

		if ( itTrack->pdgId() == 3122 ) {
//            cout << __LINE__ << "\t itTrack->numberOfDaughters() " << itTrack->numberOfDaughters() << endl;
			if ( itTrack->numberOfDaughters() != 2 ) continue;
//            cout << __LINE__ << endl;
			if ( !(itTrack->daughter(0)->pdgId() == 2212 and itTrack->daughter(1)->pdgId() == -211) and
				!(itTrack->daughter(1)->pdgId() == 2212 and itTrack->daughter(0)->pdgId() == -211) ) continue;
//            cout << __LINE__ << endl;
		}
		if ( itTrack->pdgId() == -3122 ) {
//            cout << __LINE__ << "\t itTrack->numberOfDaughters() " << itTrack->numberOfDaughters() << endl;
			if ( itTrack->numberOfDaughters() != 2 ) continue;
//            cout << __LINE__ << endl;
			if ( !(itTrack->daughter(0)->pdgId() == -2212 and itTrack->daughter(1)->pdgId() == 211) and
				!(itTrack->daughter(1)->pdgId() == -2212 and itTrack->daughter(0)->pdgId() == 211) ) continue;
//            cout << __LINE__ << endl;
		}

		GenParticle d0( * (dynamic_cast<const reco::LeafCandidate*>( itTrack->daughter(0) ) ) );
		GenParticle d1( * (dynamic_cast<const reco::LeafCandidate*>( itTrack->daughter(1) ) ) );

        if ( (abs(d0.eta()) > 2.4) or (abs(d1.eta()) > 2.4) ) continue;
//            cout << __LINE__ << endl;
        if ( (d0.pt() < 0.4) or (d1.pt() < 0.4) ) continue;
//            cout << __LINE__ << endl;

		pphi->push_back(itTrack->phi());
		peta->push_back(itTrack->eta());
		prapidity->push_back(itTrack->rapidity());
		ppt->push_back(itTrack->pt());
		pmass->push_back(itTrack->mass());
		ppdgId->push_back(itTrack->pdgId());

        if ( d0.charge() > 0 ) {
            ppPt-> push_back( d0.pt()  );
            ppEta->push_back( d0.eta() );
            ppPx ->push_back( d0.px()  );
            ppPy ->push_back( d0.py()  );
            ppPz ->push_back( d0.pz()  );
        } else {
            pnPt-> push_back( d0.pt()  );
            pnEta->push_back( d0.eta() );
            pnPx ->push_back( d0.px()  );
            pnPy ->push_back( d0.py()  );
            pnPz ->push_back( d0.pz()  );
        }

        if ( d1.charge() > 0 ) {
            ppPt-> push_back( d1.pt()  );
            ppEta->push_back( d1.eta() );
            ppPx ->push_back( d1.px()  );
            ppPy ->push_back( d1.py()  );
            ppPz ->push_back( d1.pz()  );
        } else {
            pnPt-> push_back( d1.pt()  );
            pnEta->push_back( d1.eta() );
            pnPx ->push_back( d1.px()  );
            pnPy ->push_back( d1.py()  );
            pnPz ->push_back( d1.pz()  );
        }

		CenterOfMassBooster b(dynamic_cast<const reco::Candidate&>(*itTrack));

		GenParticle v0(
				itTrack->charge(),
				itTrack->p4(),
				itTrack->vertex(),
				itTrack->pdgId(),
				itTrack->status(),
				true
				);
		b.set(dynamic_cast<reco::Candidate&>(v0));
		b.set(dynamic_cast<reco::Candidate&>(d0));
		b.set(dynamic_cast<reco::Candidate&>(d1));

        if ( d0.charge() > 0 ) {
            ppPxCM ->push_back( d0.px()  );
            ppPyCM ->push_back( d0.py()  );
            ppPzCM ->push_back( d0.pz()  );
        } else {
            pnPxCM ->push_back( d0.px()  );
            pnPyCM ->push_back( d0.py()  );
            pnPzCM ->push_back( d0.pz()  );
        }

        if ( d1.charge() > 0 ) {
            ppPxCM ->push_back( d1.px()  );
            ppPyCM ->push_back( d1.py()  );
            ppPzCM ->push_back( d1.pz()  );
        } else {
            pnPxCM ->push_back( d1.px()  );
            pnPyCM ->push_back( d1.py()  );
            pnPzCM ->push_back( d1.pz()  );
        }
	}
	iEvent.put(move(pphi), std::string("phi"));
	iEvent.put(move(peta), std::string("eta"));
	iEvent.put(move(ppt), std::string("pt"));
	iEvent.put(move(prapidity), std::string("rapidity"));
	iEvent.put(move(pmass), std::string("mass"));
	iEvent.put(move(ppdgId), std::string("pdgId"));

	iEvent.put(move(ppPt),  std::string("pPt"));
	iEvent.put(move(ppEta), std::string("pEta"));
	iEvent.put(move(ppPx),  std::string("pPx"));
	iEvent.put(move(ppPy),  std::string("pPy"));
	iEvent.put(move(ppPz),  std::string("pPz"));

	iEvent.put(move(pnPt),  std::string("nPt"));
	iEvent.put(move(pnEta), std::string("nEta"));
	iEvent.put(move(pnPx),  std::string("nPx"));
	iEvent.put(move(pnPy),  std::string("nPy"));
	iEvent.put(move(pnPz),  std::string("nPz"));

	iEvent.put(move(ppPxCM),  std::string("pPxCM"));
	iEvent.put(move(ppPyCM),  std::string("pPyCM"));
	iEvent.put(move(ppPzCM),  std::string("pPzCM"));
	iEvent.put(move(pnPxCM),  std::string("nPxCM"));
	iEvent.put(move(pnPyCM),  std::string("nPyCM"));
	iEvent.put(move(pnPzCM),  std::string("nPzCM"));

}


DEFINE_FWK_MODULE(QWGenLmProducer);
