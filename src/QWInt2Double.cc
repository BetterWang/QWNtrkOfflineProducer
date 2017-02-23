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
	consumes<int>(src_);

	produces<double>();
}

QWGenHIProducer::~QWGenHIProducer()
{
	return;
}

void QWGenHIProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	Handle<int> pX;
	iEvent.getByLabel(src_, pX);

        auto_ptr<double> p(new double(*pX));

	iEvent.put(p);
}


DEFINE_FWK_MODULE(QWGenHIProducer);
