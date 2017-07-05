#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalPulseShapes.h"

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
	produces<std::vector<double> >("nominal_fC");
	produces<std::vector<double> >("regular_fC");
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
	std::auto_ptr<std::vector<double> > pregfC( new std::vector<double> );


	Handle<ZDCDigiCollection> digi;
	iEvent.getByLabel(Src_, digi);

	edm::ESHandle<HcalDbService> conditions;
	iSetup.get<HcalDbRecord>().get(conditions);

	for ( auto i = digi->begin(); i < digi->end(); i++ ) {
		const ZDCDataFrame & rh = (*i);
		HcalZDCDetId zdcid = rh.id();

		const HcalQIECoder* qiecoder = conditions->getHcalCoder(zdcid);
		const HcalQIEShape* qieshape = conditions->getHcalShape(qiecoder);
		HcalCoderDb coder(*qiecoder, *qieshape);
		CaloSamples caldigi;
		coder.adc2fC(rh, caldigi);

		for ( int j = 0; j < 10; j++ ) {
			// hardcode 10 TS
			pADC->push_back((*i)[j].adc());
			pfC->push_back((*i)[j].nominal_fC());
			pregfC->push_back(caldigi[j]);
		}
	// order-> -Side, EM1-5, HAD1-4, +side, EM1-5, HAD1-4
	//	std::cout << "i->id().zside() = " << i->id().zside() << " i->id().section() = " << i->id().section() << " i->id().channel() = " << i->id().channel() << std::endl;
	}

	iEvent.put(pADC, std::string("ADC"));
	iEvent.put(pfC, std::string("nominal_fC"));
	iEvent.put(pregfC, std::string("regular_fC"));
}


DEFINE_FWK_MODULE(QWZDCProducer);
