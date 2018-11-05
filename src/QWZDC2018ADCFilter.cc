#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HcalDetId/interface/HcalZDCDetId.h"
#include "iostream"

class QWZDC2018ADCFilter : public edm::EDFilter {
public:
	explicit QWZDC2018ADCFilter(const edm::ParameterSet&);
	~QWZDC2018ADCFilter() {return;}
private:
	virtual bool filter(edm::Event&, const edm::EventSetup&);

	edm::InputTag	srcCid_;
	edm::InputTag	srcDid_;
	edm::InputTag	srcADC_;
	unsigned int	count_;
	std::map<uint32_t, std::string> cname;
};

QWZDC2018ADCFilter::QWZDC2018ADCFilter(const edm::ParameterSet& pset) :
	srcCid_(pset.getUntrackedParameter<edm::InputTag>("srcCapId")),
	srcDid_(pset.getUntrackedParameter<edm::InputTag>("srcDetId")),
	srcADC_(pset.getUntrackedParameter<edm::InputTag>("srcADC")),
	count_(0)
{
	consumes<std::vector<double>>(srcCid_);
	consumes<std::vector<double>>(srcDid_);
	consumes<std::vector<double>>(srcADC_);

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

	return;
}

bool QWZDC2018ADCFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	edm::Handle<std::vector<double>> psrcDid;
	edm::Handle<std::vector<double>> psrcCid;
	edm::Handle<std::vector<double>> psrcADC;

	iEvent.getByLabel( srcDid_, psrcDid );
	iEvent.getByLabel( srcCid_, psrcCid );
	iEvent.getByLabel( srcADC_, psrcADC );

	int NS = psrcCid->size() / psrcDid->size();

	int idx = 0;
	for ( auto it = psrcDid->begin(); it != psrcDid->end(); it++ ) {
		int capId0 = (*psrcCid)[idx];
		idx++;
		for ( int ts = 1; ts < NS; ts++ ) {
			int capId = ( int( (*psrcCid)[idx] ) + 40 - ts ) % 4;
			if ( capId != capId0 ) {
				count_++;
				std::cout << "\033[1;31m  --> Inconsistency of " << cname[uint32_t(*it)] << " ADC = " << int( (*psrcADC)[idx] ) << " filtering total #" << count_ << "\033[0m\n";
				return false;
			}
			idx++;
		}
	}
	return true;
}

DEFINE_FWK_MODULE(QWZDC2018ADCFilter);
