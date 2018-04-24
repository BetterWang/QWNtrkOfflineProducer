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
class QWGenEventProducer : public edm::EDProducer {
public:
	explicit QWGenEventProducer(const edm::ParameterSet&);
	~QWGenEventProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///

	edm::InputTag	trackSrc_;

	double	pTmin_;
	double	pTmax_;
	double	Etamin_;
	double	Etamax_;
	bool	isPrompt_;

	bool	doFilterPdg_;
	std::vector<int>	vPdgId_;

};

QWGenEventProducer::QWGenEventProducer(const edm::ParameterSet& pset) :
	trackSrc_(pset.getUntrackedParameter<edm::InputTag>("trackSrc")),
	vPdgId_(pset.getUntrackedParameter<std::vector<int>>("pdgId", std::vector<int>{}))
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
	produces<std::vector<double> >("pt");
	produces<std::vector<double> >("weight");
	produces<std::vector<double> >("charge");
	produces<std::vector<double> >("pdgId");
	produces<std::vector<double> >("vz");
}

QWGenEventProducer::~QWGenEventProducer()
{
	return;
}

void QWGenEventProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;

	std::unique_ptr<std::vector<double> > pphi( new std::vector<double> );
	std::unique_ptr<std::vector<double> > peta( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppT( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pweight( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pcharge( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pvz( new std::vector<double> );
	std::unique_ptr<std::vector<double> > ppdg( new std::vector<double> );

	pvz->push_back(0.0);

	Handle<GenParticleCollection> tracks;
	iEvent.getByLabel(trackSrc_,tracks);

	for(GenParticleCollection::const_iterator itTrack = tracks->begin();
			itTrack != tracks->end();
			++itTrack) {
		if ( itTrack->status() != 1 ) continue;
		if ( isPrompt_ and (not itTrack->isPromptFinalState()) ) continue;
		if ( itTrack->charge() == 0 ) continue;
		if ( itTrack->eta() > Etamax_ or itTrack->eta() < Etamin_ or itTrack->pt() > pTmax_ or itTrack->pt() < pTmin_ ) continue;
		if ( doFilterPdg_ and (std::find(vPdgId_.begin(), vPdgId_.end(), itTrack->pdgId()) == vPdgId_.end()) ) continue;

		pphi->push_back(itTrack->phi());
		peta->push_back(itTrack->eta());
		ppT->push_back(itTrack->pt());
		pweight->push_back(1.);
		pcharge->push_back(itTrack->charge());
		ppdg->push_back(itTrack->pdgId());
	}
	iEvent.put(move(pphi), std::string("phi"));
	iEvent.put(move(peta), std::string("eta"));
	iEvent.put(move(ppT), std::string("pt"));
	iEvent.put(move(pweight), std::string("weight"));
	iEvent.put(move(pcharge), std::string("charge"));
	iEvent.put(move(ppdg), std::string("pdgId"));
	iEvent.put(move(pvz), std::string("vz"));
}


DEFINE_FWK_MODULE(QWGenEventProducer);
