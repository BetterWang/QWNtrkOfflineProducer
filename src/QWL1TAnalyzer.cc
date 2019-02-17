#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"
//#include "DataFormats/L1TGlobal/interface/GlobalExtBlk.h"
#include "DataFormats/L1Trigger/interface/BXVector.h"
#include "L1Trigger/L1TGlobal/interface/L1TGlobalUtil.h"

#include <iostream>
#include <vector>
#include <TTree.h>

using namespace std;
class QWL1TAnalyzer : public edm::EDAnalyzer {
public:
	explicit QWL1TAnalyzer(const edm::ParameterSet&);
	~QWL1TAnalyzer();

private:
	virtual void beginJob(){};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void beginRun(const edm::Run&, const edm::EventSetup&);
	virtual void endJob(){};
	///

	edm::InputTag	Src_;
	std::shared_ptr<l1t::L1TGlobalUtil> gtUtil_;
	int		numAlgs_;
	bool		bFinal_[512][5];
	bool		bInitial_[512][5];
	bool		bInterm_[512][5];

	TTree*		trV_;
};


QWL1TAnalyzer::QWL1TAnalyzer(const edm::ParameterSet& pset) :
	Src_(pset.getUntrackedParameter<edm::InputTag>("l1tStage2uGtSource")),
	gtUtil_(new l1t::L1TGlobalUtil(pset, consumesCollector(), *this, pset.getUntrackedParameter<edm::InputTag>("l1tStage2uGtSource"), pset.getUntrackedParameter<edm::InputTag>("l1tStage2uGtSource")))
{
	consumes<GlobalAlgBlkBxCollection>(Src_);
	edm::Service<TFileService> fs;

	trV_ = fs->make<TTree>("trV", "trV");
	trV_->Branch("bFinal", bFinal_, "bFinal[512][5]/O");
	trV_->Branch("bInitial", bInitial_, "bInitial[512][5]/O");
	trV_->Branch("bInterm", bInterm_, "bInterm[512][5]/O");
}

QWL1TAnalyzer::~QWL1TAnalyzer()
{
	return;
}


void
QWL1TAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	Handle<GlobalAlgBlkBxCollection> uGtAlgs;
	iEvent.getByLabel(Src_, uGtAlgs);

//	int bx = iEvent.bunchCrossing();
//	cout << " --> bx = " << bx << "\t getFirstBX = " << uGtAlgs->getFirstBX() << "\t getLastBX = " << uGtAlgs->getLastBX() << endl;

	if ( not uGtAlgs.isValid() ) {
		cout << __LINE__ << "\tNot Valid." << endl;
	}

	for ( int ibx = uGtAlgs->getFirstBX(); ibx <= uGtAlgs->getLastBX(); ++ibx ) {
		for ( auto itr = uGtAlgs->begin(ibx); itr != uGtAlgs->end(ibx); ++itr ) {
			for ( int i = 0; i < 512; i++ ) {
				bInitial_[i][ibx+2] = itr->getAlgoDecisionInitial(i);
				bInterm_[i][ibx+2]  = itr->getAlgoDecisionInterm(i);
				bFinal_[i][ibx+2]   = itr->getAlgoDecisionFinal(i);
			}
		}
	}

	trV_->Fill();

}


void QWL1TAnalyzer::beginRun(const edm::Run &r, const edm::EventSetup & iSetup)
{
	// Get the trigger menu information
	gtUtil_->retrieveL1Setup(iSetup);
	// Find the number of algos defined
	numAlgs_ = static_cast<int>(gtUtil_->decisionsInitial().size());
	std::cout << "\033[1;31m --> QWL1TAnalyzer numAlgs = " << numAlgs_ << "\033[0m\n";
	return;
}


DEFINE_FWK_MODULE(QWL1TAnalyzer);
