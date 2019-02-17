#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/HcalDetId/interface/HcalZDCDetId.h"
#include "TH1D.h"
#include <iostream>

class QWZDC2018CalibAnalyzer : public edm::EDAnalyzer {
public:
	explicit QWZDC2018CalibAnalyzer(const edm::ParameterSet&);
	~QWZDC2018CalibAnalyzer() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob();

	edm::InputTag   srcDid_;
	edm::InputTag   srcHigh_;
	edm::InputTag   srcLow_;
	edm::InputTag   srcSum_;
	bool		firstEvent_;
	std::map<uint32_t, TH1D*>	hHigh;
	std::map<uint32_t, TH1D*>	hLow;
	std::map<uint32_t, TH1D*>	hSum;

	std::map<uint32_t, std::string> cname;
	unsigned int	Nevent_;
};

QWZDC2018CalibAnalyzer::QWZDC2018CalibAnalyzer(const edm::ParameterSet& pset) :
	srcDid_(pset.getUntrackedParameter<edm::InputTag>("srcDetId")),
	srcHigh_(pset.getUntrackedParameter<edm::InputTag>("srcHigh")),
	srcLow_(pset.getUntrackedParameter<edm::InputTag>("srcLow")),
	srcSum_(pset.getUntrackedParameter<edm::InputTag>("srcSum")),
	firstEvent_(true)
{
	consumes<std::vector<double> >(srcDid_);
	consumes<std::vector<double> >(srcHigh_);
	consumes<std::vector<double> >(srcLow_);
	consumes<std::vector<double> >(srcSum_);

	for ( int channel = 0; channel < 16; channel++ ) {
		HcalZDCDetId did(HcalZDCDetId::EM, true, channel);
		cname[did()] = std::string("hZDCP_EM") + std::to_string(channel);

		did = HcalZDCDetId(HcalZDCDetId::EM, false, channel);
		cname[did()] = std::string("hZDCM_EM") + std::to_string(channel);

		did = HcalZDCDetId(HcalZDCDetId::HAD, true, channel);
		cname[did()] = std::string("hZDCP_HAD") + std::to_string(channel);

		did = HcalZDCDetId(HcalZDCDetId::HAD, false, channel);
		cname[did()] = std::string("hZDCM_HAD") + std::to_string(channel);

		did = HcalZDCDetId(HcalZDCDetId::RPD, true, channel+1);
		cname[did()] = std::string("hZDCP_RPD") + std::to_string(channel);

		did = HcalZDCDetId(HcalZDCDetId::RPD, false, channel+1);
		cname[did()] = std::string("hZDCM_RPD") + std::to_string(channel);
	}

	Nevent_ = 0;
}

void
QWZDC2018CalibAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle<std::vector<double> > hDid;
	Handle<std::vector<double> > hChargeHigh;
	Handle<std::vector<double> > hChargeLow;
	Handle<std::vector<double> > hChargeSum;

	iEvent.getByLabel(srcDid_, hDid);
	iEvent.getByLabel(srcHigh_, hChargeHigh);
	iEvent.getByLabel(srcLow_, hChargeLow);
	iEvent.getByLabel(srcSum_, hChargeSum);


	if ( firstEvent_ ) {
		firstEvent_ = false;

		edm::Service<TFileService> fs;
		auto fHigh = fs->mkdir("HighGain");
		auto fLow  = fs->mkdir("LowGain");
		auto fSum  = fs->mkdir("SumGain");
		for ( auto it = hDid->begin(); it != hDid->end(); it++ ) {
			if ( cname.find( (uint32_t)(*it) ) != cname.end() ) {
				hHigh[uint32_t(*it)]=fHigh.make<TH1D> (cname[(uint32_t)(*it)].c_str(), (cname[(uint32_t)(*it)]+";ratio;Count").c_str(), 1000, 0., 10.);
				hLow[uint32_t(*it)] = fLow.make<TH1D> (cname[(uint32_t)(*it)].c_str(), (cname[(uint32_t)(*it)]+";ratio;Count").c_str(), 1000, 0., 10.);
				hSum[uint32_t(*it)] = fSum.make<TH1D> (cname[(uint32_t)(*it)].c_str(), (cname[(uint32_t)(*it)]+";ratio;Count").c_str(), 1000, 0., 10.);
			}
		}

	}

	int idx = 0;
	double qHAD1P_high = -1.;
	double qHAD1P_low  = -1.;
	double qHAD1P_sum  = -1.;
	double qHAD1M_high = -1.;
	double qHAD1M_low  = -1.;
	double qHAD1M_sum  = -1.;
	for ( auto it = hDid->begin(); it != hDid->end(); it++ ) {
		if ( uint32_t(*it) == HcalZDCDetId(HcalZDCDetId::HAD, true, 1) ) {
			qHAD1P_high = (*hChargeHigh)[idx];
			qHAD1P_low  = (*hChargeLow)[idx];
			qHAD1P_sum  = (*hChargeSum)[idx];
		} else if ( uint32_t(*it) == HcalZDCDetId(HcalZDCDetId::HAD, false, 1) ) {
			qHAD1M_high = (*hChargeHigh)[idx];
			qHAD1M_low  = (*hChargeLow)[idx];
			qHAD1M_sum  = (*hChargeSum)[idx];
		}
		idx++;
	}

	idx = 0;
	for ( auto it = hDid->begin(); it != hDid->end(); it++ ) {
		if ( HcalZDCDetId(uint32_t(*it)).zside() == 1 ) {
			hHigh[uint32_t(*it)]->Fill( (*hChargeHigh)[idx] / qHAD1P_high );
			hLow [uint32_t(*it)]->Fill( (*hChargeLow )[idx] / qHAD1P_low  );
			hSum [uint32_t(*it)]->Fill( (*hChargeSum )[idx] / qHAD1P_sum  );
		} else {
			hHigh[uint32_t(*it)]->Fill( (*hChargeHigh)[idx] / qHAD1M_high );
			hLow [uint32_t(*it)]->Fill( (*hChargeLow )[idx] / qHAD1M_low  );
			hSum [uint32_t(*it)]->Fill( (*hChargeSum )[idx] / qHAD1M_sum  );
		}
		idx++;
	}

	Nevent_++;
	return;
}

void
QWZDC2018CalibAnalyzer::endJob() {
	return;
}

DEFINE_FWK_MODULE(QWZDC2018CalibAnalyzer);
