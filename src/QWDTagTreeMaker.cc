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


class QWDTagTreeMaker : public edm::EDAnalyzer {
public:
	explicit QWDTagTreeMaker(const edm::ParameterSet&);
	~QWDTagTreeMaker() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob() {};

	std::vector<edm::InputTag>	vTags_;

	TTree*	trV;
	std::map< std::string, double* >	psv_;
};

QWDTagTreeMaker::QWDTagTreeMaker(const edm::ParameterSet& pset) :
	vTags_(pset.getUntrackedParameter< std::vector<edm::InputTag> >("vTag"))
{
	for ( auto & tag : vTags_ ) {
		consumes<double>( tag );
	}

	edm::Service<TFileService> fs;
	trV = fs->make<TTree>("trV", "trV");
	for ( auto &tag : vTags_ ) {
		std::string br = tag.instance() + "_" + tag.label();
		psv_[br] = new double;
		trV->Branch(br.c_str(), psv_[br]);
	}
}

void
QWDTagTreeMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	for ( auto & pair : psv_ ) {
		*(pair.second) = 0.;
	}
	for ( auto &tag : vTags_ ) {
		Handle<double> handle;
		iEvent.getByLabel(tag, handle);
		std::string br = tag.instance() + "_" + tag.label();
		*(psv_[br]) = *handle;
	}
	trV->Fill();
	return;
}

DEFINE_FWK_MODULE(QWDTagTreeMaker);
