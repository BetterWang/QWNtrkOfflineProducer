#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "TFile.h"
#include "TH2.h"
#include "TMath.h"

using namespace std;
class QWVectorSelector : public edm::EDProducer {
public:
	explicit QWVectorSelector(const edm::ParameterSet&);
	~QWVectorSelector();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;

	edm::InputTag	vectSrc_;
	int	idx_;
};

QWVectorSelector::QWVectorSelector(const edm::ParameterSet& pset) :
	vectSrc_(pset.getUntrackedParameter<edm::InputTag>("vectSrc")),
	idx_(pset.getUntrackedParameter<int>("idx", 0))
{
	consumes< std::vector<double> >(vectSrc_);
	produces<double>();
}

QWVectorSelector::~QWVectorSelector()
{
	return;
}

void QWVectorSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	Handle<std::vector<double> > t;
	iEvent.getByLabel(vectSrc_, t);

	double ret;
	if ( t->size() <= (unsigned int)(idx_) ) ret = -9999999;
	else ret = (*t)[idx_];

	iEvent.put(make_unique<double>(ret));
}


DEFINE_FWK_MODULE(QWVectorSelector);
