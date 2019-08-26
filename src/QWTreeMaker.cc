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


class QWTreeMaker : public edm::EDAnalyzer {
public:
	explicit QWTreeMaker(const edm::ParameterSet&);
	~QWTreeMaker() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob() {};

	TTree*	trV;
	std::map< std::string, std::vector<double>* >	pV_;
	std::map< std::string, double* >		pD_;
	std::map< std::string, edm::InputTag >		pVInput_;
	std::map< std::string, edm::InputTag >		pDInput_;
	std::vector<std::string>			Vnames_;
	std::vector<std::string>			Dnames_;
};

QWTreeMaker::QWTreeMaker(const edm::ParameterSet& pset)
{
	edm::Service<TFileService> fs;
	trV = fs->make<TTree>("trV", "trV");

	auto pVtag = pset.getUntrackedParameter< std::vector< edm::ParameterSet > >("Vtags");
	for ( auto tag : pVtag ) {
		consumes<std::vector<double> >(tag.getUntrackedParameter<edm::InputTag>("tag"));
		std::string name = tag.getUntrackedParameter<std::string>("name");
		pV_[name] = new std::vector<double>;
		trV->Branch(name.c_str(), pV_[name]);
		pVInput_[name] = tag.getUntrackedParameter<edm::InputTag>("tag");
		Vnames_.push_back(name);
	}
	auto pDtag = pset.getUntrackedParameter< std::vector< edm::ParameterSet > >("Dtags");
	for ( auto tag : pDtag ) {
		consumes<double>(tag.getUntrackedParameter<edm::InputTag>("tag"));
		std::string name = tag.getUntrackedParameter<std::string>("name");
		pD_[name] = new double;
		trV->Branch(name.c_str(), pD_[name]);
		pDInput_[name] = tag.getUntrackedParameter<edm::InputTag>("tag");
		Dnames_.push_back(name);
	}
}

void
QWTreeMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	for ( auto it : Vnames_ ) {
		Handle<std::vector<double> > handle;
		iEvent.getByLabel(pVInput_[it], handle);
		*(pV_[it]) = *handle;
	}
	for ( auto it : Dnames_ ) {
		Handle<double> handle;
		iEvent.getByLabel(pDInput_[it], handle);
		*(pD_[it]) = *handle;
	}

	trV->Fill();
	return;
}

DEFINE_FWK_MODULE(QWTreeMaker);
