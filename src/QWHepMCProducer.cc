#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "TFile.h"
#include "TH2.h"
#include "TMath.h"

using namespace std;
class QWHepMCProducer : public edm::EDProducer {
public:
	explicit QWHepMCProducer(const edm::ParameterSet&);
	~QWHepMCProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///

	edm::InputTag	src_;

};

QWHepMCProducer::QWHepMCProducer(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src"))
{
	consumes<edm::HepMCProduct>(src_);

	produces<double>("b");
	produces<double>("EP");
	produces<double>("Npart");
	produces<double>("Ncoll");
}

QWHepMCProducer::~QWHepMCProducer()
{
	return;
}

void QWHepMCProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	Handle<HepMCProduct> HepMCEvt ;
	iEvent.getByLabel( src_ , HepMCEvt ) ;

	HepMC::GenEvent * genevt = (HepMC::GenEvent *)HepMCEvt->GetEvent();
	HepMC::HeavyIon * hi = genevt->heavy_ion();

	if ( hi ) {
		unique_ptr<double> pb(new double(hi->impact_parameter()));
		unique_ptr<double> pEP(new double(hi->event_plane_angle()));
		unique_ptr<double> pNpart(new double(hi->Npart_proj() + hi->Npart_targ()));
		unique_ptr<double> pNcoll(new double(hi->Ncoll()));

		iEvent.put(move(pb), std::string("b"));
		iEvent.put(move(pEP), std::string("EP"));
		iEvent.put(move(pNpart), std::string("Npart"));
		iEvent.put(move(pNcoll), std::string("Ncoll"));
	} else {
		iEvent.put(unique_ptr<double>(new double(0.)), std::string("b"));
		iEvent.put(unique_ptr<double>(new double(0.)), std::string("EP"));
		iEvent.put(unique_ptr<double>(new double(0.)), std::string("Npart"));
		iEvent.put(unique_ptr<double>(new double(0.)), std::string("Ncoll"));
	}

}


DEFINE_FWK_MODULE(QWHepMCProducer);
