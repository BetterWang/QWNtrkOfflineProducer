#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"

//#include "DataFormats/TrackReco/interface/Track.h"
//#include "DataFormats/TrackReco/interface/TrackFwd.h"
//#include "DataFormats/VertexReco/interface/Vertex.h"
//#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include <complex>
#include <TMath.h>

class QWTrackQProducer : public edm::EDProducer {
public:
	explicit QWTrackQProducer(const edm::ParameterSet&);
	~QWTrackQProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	edm::InputTag	trackPhi_;
	edm::InputTag	trackEta_;
	edm::InputTag	trackPt_;
	edm::InputTag	trackWeight_;
	double		etaMin_;
	double		etaMax_;
	double		ptMin_;
	double		ptMax_;
	int		N_;
	bool		ptWeight_;
};

QWTrackQProducer::QWTrackQProducer(const edm::ParameterSet& pset) :
	trackPhi_(pset.getUntrackedParameter<edm::InputTag>("trackPhi")),
	trackEta_(pset.getUntrackedParameter<edm::InputTag>("trackEta")),
	trackPt_(pset.getUntrackedParameter<edm::InputTag>("trackPt")),
	trackWeight_(pset.getUntrackedParameter<edm::InputTag>("trackWeight")),
	etaMin_(pset.getUntrackedParameter<double>("etaMin")),
	etaMax_(pset.getUntrackedParameter<double>("etaMax")),
	ptMin_(pset.getUntrackedParameter<double>("ptMin")),
	ptMax_(pset.getUntrackedParameter<double>("ptMax")),
	N_(pset.getUntrackedParameter<int>("N")),
	ptWeight_(pset.getUntrackedParameter<bool>("ptWeight"))
{
	consumes<std::vector<double> >(trackPhi_);
	consumes<std::vector<double> >(trackEta_);
	consumes<std::vector<double> >(trackPt_);
	consumes<std::vector<double> >(trackWeight_);

	produces<double>("arg");
	produces<double>("abs");
	produces<double>("W");
}

QWTrackQProducer::~QWTrackQProducer() {
	return;
}

void QWTrackQProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	std::complex<double> sum(0., 0.);
	double Wsum = 0.;

	Handle<std::vector<double> > hPhi;
	Handle<std::vector<double> > hEta;
	Handle<std::vector<double> > hPt;
	Handle<std::vector<double> > hWeight;
	iEvent.getByLabel(trackPhi_, hPhi);
	iEvent.getByLabel(trackEta_, hEta);
	iEvent.getByLabel(trackPt_, hPt);
	iEvent.getByLabel(trackWeight_, hWeight);

	int sz = hPhi->size();

	for ( int i = 0; i < sz; i++ ) {
		double eta = (*hEta)[i];
		if ( eta < etaMin_ or eta > etaMax_ ) continue;
		double pt = (*hPt)[i];
		if ( pt < ptMin_ or pt > ptMax_ ) continue;
		double phi = (*hPhi)[i];
		double weight = (*hWeight)[i];

		std::complex<double> Q(TMath::Cos(N_*phi), TMath::Sin(N_*phi));
		if ( ptWeight_ ) {
			sum += Q*pt;
			Wsum += pt * weight;
		} else {
			sum += Q;
			Wsum += weight;
		}
	}

	iEvent.put(std::make_unique<double>(double(std::arg(sum))), "arg");
	iEvent.put(std::make_unique<double>(double(std::abs(sum))), "abs");
	iEvent.put(std::make_unique<double>(double(Wsum)), "W");

}

DEFINE_FWK_MODULE(QWTrackQProducer);
