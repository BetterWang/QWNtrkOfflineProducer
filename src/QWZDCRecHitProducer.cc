#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalPulseShapes.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitDefs.h"
#include "iostream"

using namespace std;
class QWZDCRecHitProducer : public edm::EDProducer {
public:
	explicit QWZDCRecHitProducer(const edm::ParameterSet&);
	~QWZDCRecHitProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	virtual void beginRun(const edm::Run&, const edm::EventSetup&) override;
	///

	edm::InputTag	Src_;
	int		SOI_;
	bool		bHardCode_;
	bool		bDebug_;

	std::map<uint32_t, std::vector<double>> pedestal_;
	std::map<std::string, uint32_t>		cname_;

};


QWZDCRecHitProducer::QWZDCRecHitProducer(const edm::ParameterSet& pset) :
	Src_(pset.getUntrackedParameter<edm::InputTag>("Src"))
{
	consumes<ZDCRecHitCollection>(Src_);


	produces<double>("sumP");
	produces<double>("sumM");
	produces<double>("sumPlow");
	produces<double>("sumMlow");

	produces<vector<double>>("Pos");
	produces<vector<double>>("PosLow");
	produces<vector<double>>("Neg");
	produces<vector<double>>("NegLow");
}

QWZDCRecHitProducer::~QWZDCRecHitProducer()
{
	return;
}

void QWZDCRecHitProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	std::unique_ptr<std::vector<double> > ppos( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pneg( new std::vector<double> );

	std::unique_ptr<std::vector<double> > pposlow( new std::vector<double> );
	std::unique_ptr<std::vector<double> > pneglow( new std::vector<double> );

	Handle<ZDCRecHitCollection> hits;
	iEvent.getByLabel(Src_, hits);

	double sumP = 0.;
	double sumM = 0.;
	double sumPlow = 0.;
	double sumMlow = 0.;

	for ( auto it = hits->begin(); it != hits->end(); it++ ) {
		auto rechit = (*it);
		if ( rechit.id().zside() > 0 ) {
			sumP += rechit.energy();
			sumPlow += rechit.lowGainEnergy();
			ppos->push_back(rechit.energy());
			pposlow->push_back(rechit.lowGainEnergy());
		} else {
			sumM += rechit.energy();
			sumMlow += rechit.lowGainEnergy();
			pneg->push_back(rechit.energy());
			pneglow->push_back(rechit.lowGainEnergy());
		}
	}

	iEvent.put(make_unique<double>(sumP), std::string("sumP"));
	iEvent.put(make_unique<double>(sumM), std::string("sumM"));
	iEvent.put(make_unique<double>(sumPlow), std::string("sumPlow"));
	iEvent.put(make_unique<double>(sumMlow), std::string("sumMlow"));

	iEvent.put(move(ppos), std::string("Pos"));
	iEvent.put(move(pneg), std::string("Neg"));
	iEvent.put(move(pposlow), std::string("PosLow"));
	iEvent.put(move(pneglow), std::string("NegLow"));
}


void QWZDCRecHitProducer::beginRun(const edm::Run &r, const edm::EventSetup & iSetup)
{
	return;
}


DEFINE_FWK_MODULE(QWZDCRecHitProducer);
