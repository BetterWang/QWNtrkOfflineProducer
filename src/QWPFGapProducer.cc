#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "TH1D.h"
#include "iostream"

class QWPFGapProducer : public edm::EDProducer {
public:
	explicit QWPFGapProducer(const edm::ParameterSet&);
	~QWPFGapProducer() {return;}
private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;

	edm::InputTag	src_;
//	double		etamin_;
//	double		etamax_;
	double const	pfE[20] = {13.4, 16.4, 15.3, 16.9, 27.4, 5.5, 3.5, 2.5, 1.9, 2.7, 2.3, 1.7, 2.4, 3.8, 5.8, 21.8, 15.9, 31.7, 17.1, 13.6};
};

QWPFGapProducer::QWPFGapProducer(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src"))
//	etamin_(pset.getUntrackedParameter<double>("etamin", -5.)),
//	etamax_(pset.getUntrackedParameter<double>("etamax", 5.))
{
	consumes<reco::PFCandidateCollection>(src_);
	// FRG -- Forward Rapidity Gap, count from eta=-5
	// BRG -- Backward Rapidity Gap, count from eta=5
	produces< double >("FRG");
	produces< double >("BRG");
}

void QWPFGapProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	edm::Handle<reco::PFCandidateCollection> pf;
	iEvent.getByLabel( src_, pf );

	TH1D h("h", "h", 20, -5., 5.);

	for ( auto it = pf->begin(); it != pf->end(); it++ ) {
		double eta = it->eta();
		double energy = it->energy();
		h.Fill(eta, energy);
	}

	double FRG = -99.;
	double BRG = -99.;
	for ( int i = 0; i < 20; i++ ) {
		if ( h.GetBinContent(i+1) > pfE[i] ) {
			FRG = 0.5 * i;
			break;
		}
	}

	for ( int i = 0; i < 20; i++ ) {
		if ( h.GetBinContent(20-i) > pfE[19-i] ) {
			BRG = 0.5 * i;
			break;
		}
	}

	iEvent.put(std::make_unique<double>(FRG), std::string("FRG"));
	iEvent.put(std::make_unique<double>(BRG), std::string("BRG"));

	return;
}

DEFINE_FWK_MODULE(QWPFGapProducer);
