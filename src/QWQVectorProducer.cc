#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <complex>
#include <TMath.h>

using namespace std;
class QWQVectorProducer : public edm::EDProducer {
public:
	explicit QWQVectorProducer(const edm::ParameterSet&);
	~QWQVectorProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///
	edm::InputTag	phiSrc_;
	edm::InputTag	ptSrc_;
	edm::InputTag	weightSrc_;

	int 		N_;
	bool		bWeight_;
};

QWQVectorProducer::QWQVectorProducer(const edm::ParameterSet& pset) :
	phiSrc_(pset.getUntrackedParameter<edm::InputTag>("phiSrc")),
	ptSrc_(pset.getUntrackedParameter<edm::InputTag>("ptSrc")),
	weightSrc_(pset.getUntrackedParameter<edm::InputTag>("weightSrc")),
	N_(pset.getUntrackedParameter<int>("N", 1))
{
	consumes<vector<double>>(phiSrc_);
	consumes<vector<double>>(ptSrc_);
	if ( weightSrc_.label() != "NA" ) {
		consumes<vector<double>>(weightSrc_);
		bWeight_ = true;
	} else {
		bWeight_ = false;
	}

	produces<double>("arg");
	produces<double>("abs");
}

QWQVectorProducer::~QWQVectorProducer()
{
	return;
}

void QWQVectorProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	Handle<std::vector<double> > hPhi;
	Handle<std::vector<double> > hPt;
	Handle<std::vector<double> > hWeight;

	iEvent.getByLabel(phiSrc_,	hPhi);
	iEvent.getByLabel(ptSrc_,	hPt);
	if ( bWeight_ ) {
		iEvent.getByLabel(weightSrc_,	hWeight);
	}

	int sz = int(hPhi->size());
	std::complex<double> Qvect(0., 0.);

	for ( int i = 0; i < sz; i++ ) {
		std::complex<double> Q( TMath::Cos( N_*(*hPhi)[i] ), TMath::Sin( N_*(*hPhi)[i] ) );
		if ( bWeight_ ) {
			Qvect += Q * (*hPt)[i] * (*hWeight)[i];
		} else {
			Qvect += Q * (*hPt)[i];
		}
	}

	iEvent.put(std::make_unique<double>(std::arg(Qvect)), std::string("arg"));
	iEvent.put(std::make_unique<double>(std::abs(Qvect)), std::string("abs"));
}



DEFINE_FWK_MODULE(QWQVectorProducer);
