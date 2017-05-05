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
class QWAccHole : public edm::EDProducer {
public:
	explicit QWAccHole(const edm::ParameterSet&);
	~QWAccHole();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///
	edm::InputTag	srcPhi_;
	edm::InputTag	srcEta_;
};

QWAccHole::QWAccHole(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src"))
{
	consumes<int>(src_);

	produces<double>();
}

QWAccHole::~QWAccHole()
{
	return;
}

void QWAccHole::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle< std::vector<double> > pphi;
	iEvent.getByLabel(src_, pphi);
	std::vector<double> phi = *pphi;
	auto_ptr<std::vector<double> > pphi_new(new std::vector<double>);
	pphi_new->reserve(phi.size());
	const double pi2 = TMath::Pi()*2;

	for ( auto it = phi.begin(); it != phi.end(); ++it ) {
		double ep = gRandom->Uniform(0, pi2);
		double phi = (*it) + ep;
		while ( phi > TMath::Pi() ) phi -= pi2;
		while ( phi < -TMath::Pi() ) phi += pi2;
		pphi_new->push_back(phi);
	}

	iEvent.put(pphi_new);
}


DEFINE_FWK_MODULE(QWAccHole);
