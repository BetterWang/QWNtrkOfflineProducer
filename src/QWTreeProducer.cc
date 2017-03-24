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
class QWTreeProducer : public edm::EDProducer {
public:
	explicit QWTreeProducer(const edm::ParameterSet&);
	~QWTreeProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///

	edm::InputTag	pathSrc_;

	TChain ch_;
	int idx_;
	const int NCNT = 2500;
	double pT_[NCNT];
	double phi_[NCNT];
	double eta_[NCNT];
	int Nmult;
};

QWTreeProducer::QWTreeProducer(const edm::ParameterSet& pset) :
	pathSrc_(pset.getUntrackedParameter<edm::InputTag>("pathSrc")),
	idx_(0)
{
	ch_.Add(pathSrc_.label().c_str());
	ch_.SetBranchAddress("Nmult", &Nmult);
	ch_.SetBranchAddress("pt", pt);
	ch_.SetBranchAddress("eta", eta);
	ch_.SetBranchAddress("phi", phi);

	produces<std::vector<double> >("phi");
	produces<std::vector<double> >("eta");
	produces<std::vector<double> >("pt");
	produces<std::vector<double> >("weight");
	produces<std::vector<double> >("vz");
}

QWTreeProducer::~QWTreeProducer()
{
	return;
}

void QWTreeProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

	std::auto_ptr<std::vector<double> > pphi( new std::vector<double> );
	std::auto_ptr<std::vector<double> > peta( new std::vector<double> );
	std::auto_ptr<std::vector<double> > ppT( new std::vector<double> );
	std::auto_ptr<std::vector<double> > pweight( new std::vector<double> );
	std::auto_ptr<std::vector<double> > pvz( new std::vector<double> );

	int sz = ch_.GetEntry(idx_++);
	if ( sz > 0 and Nmult > 0 ) {
		pphi->reserve(Nmult);
		peta->reserve(Nmult);
		ppt->reserve(Nmult);
		pweight->reserve(Nmult);

		pvz->reserve(1);
		pvz->push_back(0);

		for ( int i = 0; i < Nmult; i++ ) {
			pphi->push_back(phi[i]);
			peta->push_back(eta[i]);
			ppt->push_back(pt[i]);
			pweight->push_back(1.0);
		}
	}

	iEvent.put(pphi, std::string("phi"));
	iEvent.put(peta, std::string("eta"));
	iEvent.put(ppT, std::string("pt"));
	iEvent.put(pweight, std::string("weight"));
	iEvent.put(pvz, std::string("vz"));
}

DEFINE_FWK_MODULE(QWTreeProducer);
