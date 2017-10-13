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
class QWPhiRotation : public edm::EDProducer {
public:
	explicit QWPhiRotation(const edm::ParameterSet&);
	~QWPhiRotation();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///
	edm::InputTag	src_;
};

QWPhiRotation::QWPhiRotation(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src"))
{
	consumes<std::vector<double> >(src_);

	produces<std::vector<double> >("phi");
}

QWPhiRotation::~QWPhiRotation()
{
	return;
}

void QWPhiRotation::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle< std::vector<double> > pphi;
	iEvent.getByLabel(src_, pphi);
	std::vector<double> phi = *pphi;
	unique_ptr<std::vector<double> > pphi_new(new std::vector<double>);
	pphi_new->reserve(phi.size());
	const double pi2 = TMath::Pi()*2;

	for ( auto it = phi.begin(); it != phi.end(); ++it ) {
		double ep = gRandom->Uniform(0, pi2);
		double phi = (*it) + ep;
		while ( phi > TMath::Pi() ) phi -= pi2;
		while ( phi < -TMath::Pi() ) phi += pi2;
		pphi_new->push_back(phi);
	}

	iEvent.put(move(pphi_new), string("phi"));
}


DEFINE_FWK_MODULE(QWPhiRotation);
