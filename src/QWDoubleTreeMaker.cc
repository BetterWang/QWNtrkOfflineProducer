#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"
#include "TMath.h"
#include <iostream>
#include <tuple>
#include "string"
#include "vector"


class QWDoubleTreeMaker : public edm::EDAnalyzer {
public:
	explicit QWDoubleTreeMaker(const edm::ParameterSet&);
	~QWDoubleTreeMaker() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob() {};

	edm::InputTag   src_;
	std::vector<std::string>	vTags_;

	TTree*	trV;
	std::map< std::string, double*>	psv_;
};

QWDoubleTreeMaker::QWDoubleTreeMaker(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src")),
	vTags_(pset.getUntrackedParameter< std::vector<std::string> >("vTag"))
{
	for ( auto tag : vTags_ ) {
		consumes<double>(edm::InputTag(src_.label(), tag));
	}

	edm::Service<TFileService> fs;
	trV = fs->make<TTree>("trV", "trV");
	for ( auto tag : vTags_ ) {
		psv_[tag] = new double;
		trV->Branch(tag.c_str(), psv_[tag]);
	}
}

void
QWDoubleTreeMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	for ( auto tag : vTags_ ) {
		Handle<double> handle;
		iEvent.getByLabel(InputTag(src_.label(), tag), handle);
		*(psv_[tag]) = *handle;
	}

	trV->Fill();
	return;
}

DEFINE_FWK_MODULE(QWDoubleTreeMaker);
