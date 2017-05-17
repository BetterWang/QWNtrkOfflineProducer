#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "iostream"

using namespace std;
class QWZDCProducer : public edm::EDProducer {
public:
	explicit QWZDCProducer(const edm::ParameterSet&);
	~QWZDCProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///

	edm::InputTag	Src_;

};

QWZDCProducer::QWZDCProducer(const edm::ParameterSet& pset) :
	Src_(pset.getUntrackedParameter<edm::InputTag>("Src"))
{
	consumes<ZDCDigiCollection>(Src_);

	produces<std::vector<double> >("ADC");
	produces<std::vector<double> >("fC");
}

QWZDCProducer::~QWZDCProducer()
{
	return;
}

void QWZDCProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	std::auto_ptr<std::vector<double> > pADC( new std::vector<double> );
	std::auto_ptr<std::vector<double> > pfC( new std::vector<double> );


	Handle<ZDCDigiCollection> digi;
	iEvent.getByLabel(Src_, digi);

	for ( auto i = digi->begin(); i < digi->end(); i++ ) {
		for ( int j = 0; j < 10; j++ ) {
			// hardcode 10 TS
			pADC->push_back((*i)[j].adc());
			pfC->push_back((*i)[j].nominal_fC());
		}
	// order-> -Side, EM1-5, HAD1-4, +side, EM1-5, HAD1-4
	//	std::cout << "i->id().zside() = " << i->id().zside() << " i->id().section() = " << i->id().section() << " i->id().channel() = " << i->id().channel() << std::endl;
	}

	iEvent.put(pADC, std::string("ADC"));
	iEvent.put(pfC, std::string("fC"));
}


DEFINE_FWK_MODULE(QWZDCProducer);
