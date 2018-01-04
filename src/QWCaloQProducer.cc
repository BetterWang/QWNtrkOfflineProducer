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

class QWCaloQProducer : public edm::EDProducer {
public:
	explicit QWCaloQProducer(const edm::ParameterSet&);
	~QWCaloQProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	edm::InputTag	caloSrc_;
	double		etaMin_;
	double		etaMax_;
	int		N_;
};

QWCaloQProducer::QWCaloQProducer(const edm::ParameterSet& pset) :
	caloSrc_(pset.getUntrackedParameter<edm::InputTag>("caloSrc")),
	etaMin_(pset.getUntrackedParameter<double>("etaMin")),
	etaMax_(pset.getUntrackedParameter<double>("etaMax")),
	N_(pset.getUntrackedParameter<int>("N"))
{
	consumes<CaloTowerCollection>(caloSrc_);
	produces<double>("arg");
	produces<double>("argp");
	produces<double>("argm");

	produces<double>("abs");
	produces<double>("absp");
	produces<double>("absm");

	produces<double>("W");
	produces<double>("Wplus");
	produces<double>("Wminus");

	produces<std::vector<double> >("CaloTowerEmE");
	produces<std::vector<double> >("CaloTowerHadE");
	produces<std::vector<double> >("CaloTowerE");
	produces<std::vector<double> >("CaloTowerPhi");
	produces<std::vector<double> >("CaloTowerEta");
}

QWCaloQProducer::~QWCaloQProducer() {
	return;
}

void QWCaloQProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;

	std::complex<double> HFsum(0., 0.);
	std::complex<double> HFplus(0., 0.);
	std::complex<double> HFminus(0., 0.);
	double Wsum = 0.;
	double Wplus = 0.;
	double Wminus = 0.;

	auto caloEmE = std::make_unique<std::vector<double>>();
	auto caloHadE = std::make_unique<std::vector<double>>();
	auto caloE = std::make_unique<std::vector<double>>();
	auto caloPhi = std::make_unique<std::vector<double>>();
	auto caloEta = std::make_unique<std::vector<double>>();

	Handle<CaloTowerCollection> caloCollection;
	iEvent.getByLabel(caloSrc_, caloCollection);

	if ( caloCollection.isValid() ) {
		for ( auto j = caloCollection->begin(); j != caloCollection->end(); j++ ) {
			double eta = j->eta();
			double phi = j->phi();
			double et = j->emEt() + j->hadEt();

			std::complex<double> Q(TMath::Cos(N_*phi), TMath::Sin(N_*phi));
			if ( fabs(eta) > etaMin_ and fabs(eta) < etaMax_ ) {
				caloEmE->push_back( j->emEnergy() );
				caloHadE->push_back( j->hadEnergy() );
				caloE  ->push_back( j->emEnergy() + j->hadEnergy() );
				caloPhi->push_back( phi );
				caloEta->push_back( eta );

				HFsum += et * Q;
				Wsum += et;
				if ( eta > 0 ) {
					HFplus += et * Q;
					Wplus += et;
				} else {
					HFminus += et * Q;
					Wminus += et;
				}
			}
		}
	}


	iEvent.put(std::make_unique<double>(double(std::arg(HFsum))),   "arg");
	iEvent.put(std::make_unique<double>(double(std::arg(HFplus))),  "argp");
	iEvent.put(std::make_unique<double>(double(std::arg(HFminus))), "argm");

	iEvent.put(std::make_unique<double>(double(std::abs(HFsum))),   "abs");
	iEvent.put(std::make_unique<double>(double(std::abs(HFplus))),  "absp");
	iEvent.put(std::make_unique<double>(double(std::abs(HFminus))), "absm");

	iEvent.put(std::make_unique<double>(double(Wsum)), "W");
	iEvent.put(std::make_unique<double>(double(Wplus)), "Wplus");
	iEvent.put(std::make_unique<double>(double(Wminus)), "Wminus");

	iEvent.put(move(caloEmE), "CaloTowerEmE");
	iEvent.put(move(caloHadE),"CaloTowerHadE");
	iEvent.put(move(caloE),   "CaloTowerE");
	iEvent.put(move(caloPhi), "CaloTowerPhi");
	iEvent.put(move(caloEta), "CaloTowerEta");

}

DEFINE_FWK_MODULE(QWCaloQProducer);
