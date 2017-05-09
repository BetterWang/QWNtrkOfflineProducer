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
	double		PhiMin_;
	double		PhiMax_;
	double		EtaMin_;
	double		EtaMax_;
	std::vector<edm::InputTag>	srcVtag_;
};

QWAccHole::QWAccHole(const edm::ParameterSet& pset) :
	srcPhi_(pset.getUntrackedParameter<edm::InputTag>("srcPhi")),
	srcEta_(pset.getUntrackedParameter<edm::InputTag>("srcEta")),
	PhiMin_(pset.getUntrackedParameter<double>("PhiMin")),
	PhiMax_(pset.getUntrackedParameter<double>("PhiMax")),
	EtaMin_(pset.getUntrackedParameter<double>("EtaMin")),
	EtaMax_(pset.getUntrackedParameter<double>("EtaMax")),
	srcVtag_(pset.getUntrackedParameter< std::vector< edm::InputTag > > ("srcVtag") )
{
	consumes< std::vector<double> >(srcPhi_);
	consumes< std::vector<double> >(srcEta_);

	for ( auto it = srcVtag_.begin(); it != srcVtag_.end(); it++ ) {
		consumes< std::vector<double> >( *it );
		produces<std::vector<double> >(it->instance());
	}

	produces<std::vector<double> >("phi");
	produces<std::vector<double> >("eta");
}

QWAccHole::~QWAccHole()
{
	return;
}

void QWAccHole::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle< std::vector<double> > pphi;
	Handle< std::vector<double> > peta;
	iEvent.getByLabel(srcPhi_, pphi);
	iEvent.getByLabel(srcEta_, peta);

	std::vector<double> phi = *pphi;
	std::vector<double> eta = *peta;

	auto_ptr<std::vector<double> > pphi_new( new std::vector<double> );
	auto_ptr<std::vector<double> > peta_new( new std::vector<double> );

	std::vector< auto_ptr<std::vector<double> > >  vnew;

	std::vector< Handle< std::vector<double> > > pH;
	for ( unsigned int i = 0; i < srcVtag_.size(); i++ ) {
		vnew.push_back( auto_ptr<std::vector<double> >(new std::vector<double>) );
		pH.push_back(Handle< std::vector<double> >() );
		iEvent.getByLabel(srcVtag_[i], pH[i]);
	}

	for ( unsigned int i = 0; i < phi.size(); i++ ) {
		if ( phi[i] > PhiMin_ and phi[i] < PhiMax_ and eta[i] > EtaMin_ and eta[i] < EtaMax_ ) {
//			std::cout << __LINE__ << " " << phi[i] << "\t" << eta[i] << std::endl;
			continue;
		}
//		std::cout << __LINE__ << std::endl;

		pphi_new->push_back(phi[i]);
		peta_new->push_back(eta[i]);

		for ( unsigned int j = 0; j < srcVtag_.size(); j++ ) {
			vnew[j]->push_back( (*(pH[j]))[i] );
		}
	}

	iEvent.put(pphi_new, std::string("phi"));
	iEvent.put(peta_new, std::string("eta"));
	for ( unsigned int i = 0; i < srcVtag_.size(); i++ ) {
		iEvent.put( vnew[i], srcVtag_[i].instance() );
	}
}


DEFINE_FWK_MODULE(QWAccHole);
