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
class QWModDouble : public edm::EDProducer {
public:
	explicit QWModDouble(const edm::ParameterSet&);
	~QWModDouble();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///
	edm::InputTag	src_;
    double          shift_;
};

QWModDouble::QWModDouble(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src")),
	shift_(pset.getUntrackedParameter<double>("shift"))
{
	consumes<double>(src_);

	produces<double>();
}

QWModDouble::~QWModDouble()
{
	return;
}

void QWModDouble::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle<double> pX;
	iEvent.getByLabel(src_, pX);
    double ret = *pX;
    ret += shift_;

	iEvent.put(make_unique<double>(ret));
}


DEFINE_FWK_MODULE(QWModDouble);
