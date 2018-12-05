#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HcalDetId/interface/HcalZDCDetId.h"

#include "iostream"

using namespace std;
class QWZDC2018CalibProducer : public edm::EDProducer {
public:
	explicit QWZDC2018CalibProducer(const edm::ParameterSet&);
	~QWZDC2018CalibProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	virtual void beginRun(const edm::Run&, const edm::EventSetup&) override;
	///

	edm::InputTag   srcDid_;
	edm::InputTag	srcChargeSum_;

	double		dPHad1_;
	double		dPHad2_;
	double		dPHad3_;
	double		dPHad4_;
	double		dMHad1_;
	double		dMHad2_;
	double		dMHad3_;
	double		dMHad4_;

	double		dPEM1_;
	double		dPEM2_;
	double		dPEM3_;
	double		dPEM4_;
	double		dPEM5_;
	double		dMEM1_;
	double		dMEM2_;
	double		dMEM3_;
	double		dMEM4_;
	double		dMEM5_;

	map<uint32_t, double>	calib_;
};


QWZDC2018CalibProducer::QWZDC2018CalibProducer(const edm::ParameterSet& pset) :
	srcDid_(pset.getUntrackedParameter<edm::InputTag>("srcDetId")),
	srcChargeSum_(pset.getUntrackedParameter<edm::InputTag>("srcChargeSum")),
	dPHad1_(pset.getUntrackedParameter<double>("PHad1")),
	dPHad2_(pset.getUntrackedParameter<double>("PHad2")),
	dPHad3_(pset.getUntrackedParameter<double>("PHad3")),
	dPHad4_(pset.getUntrackedParameter<double>("PHad4")),
	dMHad1_(pset.getUntrackedParameter<double>("MHad1")),
	dMHad2_(pset.getUntrackedParameter<double>("MHad2")),
	dMHad3_(pset.getUntrackedParameter<double>("MHad3")),
	dMHad4_(pset.getUntrackedParameter<double>("MHad4")),
	dPEM1_(pset.getUntrackedParameter<double>("PEM1")),
	dPEM2_(pset.getUntrackedParameter<double>("PEM2")),
	dPEM3_(pset.getUntrackedParameter<double>("PEM3")),
	dPEM4_(pset.getUntrackedParameter<double>("PEM4")),
	dPEM5_(pset.getUntrackedParameter<double>("PEM5")),
	dMEM1_(pset.getUntrackedParameter<double>("MEM1")),
	dMEM2_(pset.getUntrackedParameter<double>("MEM2")),
	dMEM3_(pset.getUntrackedParameter<double>("MEM3")),
	dMEM4_(pset.getUntrackedParameter<double>("MEM4")),
	dMEM5_(pset.getUntrackedParameter<double>("MEM5"))
{
	consumes<std::vector<double>>(srcDid_);
	consumes<std::vector<double>>(srcChargeSum_);

	calib_[(HcalZDCDetId(HcalZDCDetId::EM, true, 1))()] = dPEM1_;
	calib_[(HcalZDCDetId(HcalZDCDetId::EM, true, 2))()] = dPEM2_;
	calib_[(HcalZDCDetId(HcalZDCDetId::EM, true, 3))()] = dPEM3_;
	calib_[(HcalZDCDetId(HcalZDCDetId::EM, true, 4))()] = dPEM4_;
	calib_[(HcalZDCDetId(HcalZDCDetId::EM, true, 5))()] = dPEM5_;

	calib_[(HcalZDCDetId(HcalZDCDetId::EM, false, 1))()] = dMEM1_;
	calib_[(HcalZDCDetId(HcalZDCDetId::EM, false, 2))()] = dMEM2_;
	calib_[(HcalZDCDetId(HcalZDCDetId::EM, false, 3))()] = dMEM3_;
	calib_[(HcalZDCDetId(HcalZDCDetId::EM, false, 4))()] = dMEM4_;
	calib_[(HcalZDCDetId(HcalZDCDetId::EM, false, 5))()] = dMEM5_;

	calib_[(HcalZDCDetId(HcalZDCDetId::HAD, true, 1))()] = dPHad1_;
	calib_[(HcalZDCDetId(HcalZDCDetId::HAD, true, 2))()] = dPHad2_;
	calib_[(HcalZDCDetId(HcalZDCDetId::HAD, true, 3))()] = dPHad3_;
	calib_[(HcalZDCDetId(HcalZDCDetId::HAD, true, 4))()] = dPHad4_;

	calib_[(HcalZDCDetId(HcalZDCDetId::HAD, false, 1))()] = dPHad1_;
	calib_[(HcalZDCDetId(HcalZDCDetId::HAD, false, 2))()] = dPHad2_;
	calib_[(HcalZDCDetId(HcalZDCDetId::HAD, false, 3))()] = dPHad3_;
	calib_[(HcalZDCDetId(HcalZDCDetId::HAD, false, 4))()] = dPHad4_;

	produces<double>("sumP");
	produces<double>("sumM");

}

QWZDC2018CalibProducer::~QWZDC2018CalibProducer()
{
	return;
}

void QWZDC2018CalibProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle<std::vector<double> > hDid;
	Handle<std::vector<double>> hChargeSum;

	iEvent.getByLabel(srcDid_, hDid);
	iEvent.getByLabel(srcChargeSum_, hChargeSum);

	double sumP = 0.;
	double sumM = 0.;

	int idx = 0;
	for ( auto it = hDid->begin(); it != hDid->end(); it++ ) {
		HcalZDCDetId did = HcalZDCDetId( uint32_t(*it) );
		if ( (calib_.find(did()) != calib_.end()) and (did.section() == HcalZDCDetId::EM or did.section() == HcalZDCDetId::HAD) ) {
			if ( did.zside() > 0 ) {
				sumP += (*hChargeSum)[idx] * calib_[did()];
			} else {
				sumM += (*hChargeSum)[idx] * calib_[did()];
			}
		}
		idx++;
	}

	iEvent.put(make_unique<double>(sumP), std::string("sumP"));
	iEvent.put(make_unique<double>(sumM), std::string("sumM"));
}


void QWZDC2018CalibProducer::beginRun(const edm::Run &r, const edm::EventSetup & iSetup)
{
	return;
}


DEFINE_FWK_MODULE(QWZDC2018CalibProducer);
