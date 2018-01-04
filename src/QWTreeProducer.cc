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
#include "TChain.h"
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
	double pt_[8500];
	double phi_[8500];
	double eta_[8500];
	int Nmult_;
};

QWTreeProducer::QWTreeProducer(const edm::ParameterSet& pset) :
	pathSrc_(pset.getUntrackedParameter<edm::InputTag>("pathSrc")),
	idx_(0)
{
	ch_.Add(pathSrc_.label().c_str());
	ch_.SetBranchAddress("Nmult", &Nmult_);
	ch_.SetBranchAddress("pt", pt_);
	ch_.SetBranchAddress("eta", eta_);
	ch_.SetBranchAddress("phi", phi_);

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

	auto pphi = std::make_unique<std::vector<double>>();
	auto peta = std::make_unique<std::vector<double>>();
	auto ppT = std::make_unique<std::vector<double>>();
	auto pweight = std::make_unique<std::vector<double>>();
	auto pvz = std::make_unique<std::vector<double>>();

	int sz = ch_.GetEntry(idx_++);
	if ( sz > 0 and Nmult_ > 0 ) {
//		pphi->reserve(Nmult_);
//		peta->reserve(Nmult_);
//		ppT->reserve(Nmult_);
//		pweight->reserve(Nmult_);

		pvz->reserve(1);
		pvz->push_back(0);

		for ( int i = 0; i < Nmult_; i++ ) {
			pphi->push_back(phi_[i]);
			peta->push_back(eta_[i]);
			ppT->push_back(pt_[i]);
			pweight->push_back(1.0);
		}
	}

	iEvent.put(move(pphi), std::string("phi"));
	iEvent.put(move(peta), std::string("eta"));
	iEvent.put(move(ppT), std::string("pt"));
	iEvent.put(move(pweight), std::string("weight"));
	iEvent.put(move(pvz), std::string("vz"));
}

DEFINE_FWK_MODULE(QWTreeProducer);
