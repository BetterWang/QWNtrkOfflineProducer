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
#include "TRandom.h"

using namespace std;
class QWPhiWeightProducer : public edm::EDProducer {
public:
	explicit QWPhiWeightProducer(const edm::ParameterSet&);
	~QWPhiWeightProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///
	edm::InputTag	src_;
	edm::InputTag	srcPhi_;
	edm::InputTag	srcWeight_;
	TH1D * 		hPhi;
};

QWPhiWeightProducer::QWPhiWeightProducer(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src")),
	srcPhi_(pset.getUntrackedParameter<edm::InputTag>("srcPhi")),
	srcWeight_(pset.getUntrackedParameter<edm::InputTag>("srcWeight"))
{
	consumes<std::vector<double> >(srcPhi_);
	consumes<std::vector<double> >(srcWeight_);

	TFile *f = new TFile(src_.label().c_str());
	hPhi = (TH1D*) f->Get("hc");

	produces<std::vector<double> >("");
}

QWPhiWeightProducer::~QWPhiWeightProducer()
{
	return;
}

void QWPhiWeightProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle< std::vector<double> > pphi;
	Handle< std::vector<double> > pweight;
	iEvent.getByLabel(srcPhi_, pphi);
	iEvent.getByLabel(srcWeight_, pweight);

	int sz = pphi->size();

	std::vector<double> pw;
	pw.reserve(sz);

	for ( int i = 0; i < sz; i++ ) {
		double weight = hPhi->GetBinContent( hPhi->FindBin( (*pphi)[i] ) );
		weight = (*pweight)[i] / weight;
		pw.push_back(weight);
	}

	iEvent.put(std::make_unique<std::vector<double> >(pw), string(""));
}


DEFINE_FWK_MODULE(QWPhiWeightProducer);
