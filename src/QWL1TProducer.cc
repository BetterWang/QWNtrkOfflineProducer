#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"
//#include "DataFormats/L1TGlobal/interface/GlobalExtBlk.h"
#include "DataFormats/L1Trigger/interface/BXVector.h"
#include "L1Trigger/L1TGlobal/interface/L1TGlobalUtil.h"

#include <iostream>
#include <vector>

using namespace std;
class QWL1TProducer : public edm::EDProducer {
public:
	explicit QWL1TProducer(const edm::ParameterSet&);
	~QWL1TProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	virtual void beginRun(const edm::Run&, const edm::EventSetup&) override;
	///

	edm::InputTag	Src_;
	vector<int>	vAlgoBits_;
};


QWL1TProducer::QWL1TProducer(const edm::ParameterSet& pset) :
	Src_(pset.getUntrackedParameter<edm::InputTag>("l1tStage2uGtSource")),
	vAlgoBits_(pset.getUntrackedParameter<vector<int>>("algoBits"))
{
	consumes<GlobalAlgBlkBxCollection>(Src_);
	produces<vector<double>>("Final");
	produces<vector<double>>("Initial");
	produces<vector<double>>("Interm");
	for (auto it = vAlgoBits_.begin(); it != vAlgoBits_.end(); it++ ) {
		cout << __LINE__ << "\t" << *it << endl;
	}
}

QWL1TProducer::~QWL1TProducer()
{
	return;
}

void QWL1TProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	std::unique_ptr<vector<double>> pFinal = std::make_unique<vector<double>>();
	std::unique_ptr<vector<double>> pInitial = std::make_unique<vector<double>>();
	std::unique_ptr<vector<double>> pInterm = std::make_unique<vector<double>>();

	Handle<GlobalAlgBlkBxCollection> uGtAlgs;
	iEvent.getByLabel(Src_, uGtAlgs);

	int bx = iEvent.bunchCrossing();
	cout << " --> bx = " << bx << "\t getFirstBX = " << uGtAlgs->getFirstBX() << "\t getLastBX = " << uGtAlgs->getLastBX() << endl;

	if ( not uGtAlgs.isValid() ) {
		cout << __LINE__ << "\tNot Valid." << endl;
	}

	for (auto it = vAlgoBits_.begin(); it != vAlgoBits_.end(); it++ ) {
		cout << __LINE__ << "\t" << *it << endl;
	}
	for ( int ibx = uGtAlgs->getFirstBX(); ibx <= uGtAlgs->getLastBX(); ++ibx ) {
		cout << __LINE__ << endl;
		for ( auto itr = uGtAlgs->begin(ibx); itr != uGtAlgs->end(ibx); ++itr ) {
		cout << __LINE__ << endl;
			for ( auto it = vAlgoBits_.begin(); it != vAlgoBits_.end(); it++ ) {

		cout << __LINE__ << "\t" << *it << endl;
				pInitial->push_back( itr->getAlgoDecisionInitial(*it) );
				pInterm->push_back( itr->getAlgoDecisionInterm(*it) );
				pFinal->push_back( itr->getAlgoDecisionFinal(*it) );
				cout << __LINE__ << "\t ibx = " << ibx << "\t getAlgoDecisionInterm = " << itr->getAlgoDecisionInterm(*it) << endl;
			}
		}
	}

	iEvent.put( move(pFinal), string("Final") );
	iEvent.put( move(pInitial), string("Initial") );
	iEvent.put( move(pInterm), string("Interm") );

}


void QWL1TProducer::beginRun(const edm::Run &r, const edm::EventSetup & iSetup)
{
	// Get the trigger menu information
//	gtUtil_->retrieveL1Setup(iSetup);
	// Find the number of algos defined
//	numAlgs_ = static_cast<int>(gtUtil_->decisionsInitial().size());
//	std::cout << "\033[1;31m --> QWL1TProducer numAlgs = " << numAlgs_ << "\033[0m\n";
	return;
}


DEFINE_FWK_MODULE(QWL1TProducer);
