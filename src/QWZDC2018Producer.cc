#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
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
class QWZDC2018Producer : public edm::EDProducer {
public:
	explicit QWZDC2018Producer(const edm::ParameterSet&);
	~QWZDC2018Producer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	virtual void beginRun(const edm::Run&, const edm::EventSetup&) override;
	///

	edm::InputTag	Src_;
};


QWZDC2018Producer::QWZDC2018Producer(const edm::ParameterSet& pset) :
	Src_(pset.getUntrackedParameter<edm::InputTag>("Src"))
{
	consumes<QIE10DigiCollection>(Src_);

	produces<std::vector<double> >("ADC");
	produces<std::vector<double> >("nominalfC");
//	produces<std::vector<double> >("regularfC");
//
//	produces< double >("Sum");
//	produces< double >("SumP");
//	produces< double >("SumN");
//
//	produces< double >("emSumP");
//	produces< double >("emSumN");
//
//	produces< double >("hadSumP");
//	produces< double >("hadSumN");
}

QWZDC2018Producer::~QWZDC2018Producer()
{
	return;
}

void QWZDC2018Producer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	std::unique_ptr<std::vector<double> > pADC( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pfC( new std::vector<double> );

	Handle<QIE10DigiCollection> digis;
	iEvent.getByLabel(Src_, digis);

	double adc[25][10] = {};
	double nom_fC[25][10] = {};

	for ( auto it = digis->begin(); it != digis->end(); it++ ) {
		const QIE10DataFrame digi = static_cast<const QIE10DataFrame>(*it);
		HcalZDCDetId const& did = digi.detid();
		cout << did << "\n";
	}

	iEvent.put(move(pADC), std::string("ADC"));
	iEvent.put(move(pfC), std::string("nominalfC"));

}


void QWZDC2018Producer::beginRun(const edm::Run &r, const edm::EventSetup & iSetup)
{
	return;
}


DEFINE_FWK_MODULE(QWZDC2018Producer);
