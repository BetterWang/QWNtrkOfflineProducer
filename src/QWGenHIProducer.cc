#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/HiGenData/interface/GenHIEvent.h"

#include "TFile.h"
#include "TH2.h"
#include "TMath.h"

using namespace std;
class QWGenHIProducer : public edm::EDProducer {
public:
	explicit QWGenHIProducer(const edm::ParameterSet&);
	~QWGenHIProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///

	edm::InputTag	src_;

};

QWGenHIProducer::QWGenHIProducer(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src"))
{
	consumes<edm::GenHIEvent>(src_);

	produces<double>("b");
	produces<double>("EP");
	produces<double>("Npart");
	produces<double>("Ncoll");
	produces<double>("ecc");
}

QWGenHIProducer::~QWGenHIProducer()
{
	return;
}

void QWGenHIProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	Handle<GenHIEvent> hi;
	iEvent.getByLabel( src_ , hi ) ;

        unique_ptr<double> pb(new double(hi->b()));
        unique_ptr<double> pEP(new double(hi->evtPlane()));
        unique_ptr<double> pNpart(new double(hi->Npart()));
        unique_ptr<double> pNcoll(new double(hi->Ncoll()));
        unique_ptr<double> pecc(new double(hi->eccentricity()));

	iEvent.put(move(pb), std::string("b"));
	iEvent.put(move(pEP), std::string("EP"));
	iEvent.put(move(pNpart), std::string("Npart"));
	iEvent.put(move(pNcoll), std::string("Ncoll"));
	iEvent.put(move(pecc), std::string("ecc"));

}


DEFINE_FWK_MODULE(QWGenHIProducer);
