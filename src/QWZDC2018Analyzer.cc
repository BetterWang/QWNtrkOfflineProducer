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

class QWZDC2018Analyzer : public edm::EDAnalyzer {
public:
	explicit QWZDC2018Analyzer(const edm::ParameterSet&);
	~QWZDC2018Analyzer() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob();

	edm::InputTag   srcADC_;
	edm::InputTag   srcfC_;
	edm::InputTag   srcDid_;
	edm::InputTag   srcHigh_;
	edm::InputTag   srcLow_;
	edm::InputTag   srcSum_;
	bool		bNorm_;
	bool		firstEvent_;
	std::map<uint32_t, TH1D*>	hADC;
	std::map<uint32_t, TH1D*>	hfC;
	std::map<uint32_t, TH1D*>	hHigh;
	std::map<uint32_t, TH1D*>	hLow;
	std::map<uint32_t, TH1D*>	hSum;

	std::map<uint32_t, std::string> cname;
	unsigned int	Nevent_;
};

QWZDC2018Analyzer::QWZDC2018Analyzer(const edm::ParameterSet& pset) :
	srcADC_(pset.getUntrackedParameter<edm::InputTag>("srcADC")),
	srcfC_(pset.getUntrackedParameter<edm::InputTag>("srcfC")),
	srcDid_(pset.getUntrackedParameter<edm::InputTag>("srcDetId")),
	srcHigh_(pset.getUntrackedParameter<edm::InputTag>("srcHigh")),
	srcLow_(pset.getUntrackedParameter<edm::InputTag>("srcLow")),
	srcSum_(pset.getUntrackedParameter<edm::InputTag>("srcSum")),
	bNorm_(pset.getUntrackedParameter<bool>("Norm", false)),
	firstEvent_(true)
{
	consumes<std::vector<double> >(srcADC_);
	consumes<std::vector<double> >(srcfC_);
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
QWZDC2018Analyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle<std::vector<double> > hadc;
	Handle<std::vector<double> > hfc;
	Handle<std::vector<double> > hDid;
	Handle<std::vector<double> > hChargeHigh;
	Handle<std::vector<double> > hChargeLow;
	Handle<std::vector<double> > hChargeSum;

	iEvent.getByLabel(srcADC_, hadc);
	iEvent.getByLabel(srcfC_,  hfc);
	iEvent.getByLabel(srcDid_, hDid);
	iEvent.getByLabel(srcHigh_, hChargeHigh);
	iEvent.getByLabel(srcLow_, hChargeLow);
	iEvent.getByLabel(srcSum_, hChargeSum);

	int sz = hadc->size();

	int NS_ = sz / hDid->size();
	if ( firstEvent_ ) {
		firstEvent_ = false;

		edm::Service<TFileService> fs;
		auto fADC = fs->mkdir("ADC");
		auto ffC = fs->mkdir("fC");
		auto fHigh = fs->mkdir("HighGain");
		auto fLow  = fs->mkdir("LowGain");
		auto fSum  = fs->mkdir("SumGain");
		for ( auto it = hDid->begin(); it != hDid->end(); it++ ) {
			if ( cname.find( (uint32_t)(*it) ) != cname.end() ) {
				hADC[uint32_t(*it)] = fADC.make<TH1D>(cname[(uint32_t)(*it)].c_str(), (cname[(uint32_t)(*it)]+";TS;ADC").c_str(), 10, 0, 10);
				hfC[uint32_t(*it)]  = ffC.make<TH1D> (cname[(uint32_t)(*it)].c_str(), (cname[(uint32_t)(*it)]+";TS;fC").c_str(), 10, 0, 10);
				hHigh[uint32_t(*it)]=fHigh.make<TH1D> (cname[(uint32_t)(*it)].c_str(), (cname[(uint32_t)(*it)]+";fC;Count").c_str(), 1100, -100, 1000);
				hLow[uint32_t(*it)] = fLow.make<TH1D> (cname[(uint32_t)(*it)].c_str(), (cname[(uint32_t)(*it)]+";fC;Count").c_str(), 1100, -100, 1000);
				hSum[uint32_t(*it)] = fSum.make<TH1D> (cname[(uint32_t)(*it)].c_str(), (cname[(uint32_t)(*it)]+";fC;Count").c_str(), 1100, -100, 1000);
			}
		}

		std::cout << "\033[1;31mNsamples = " << NS_ << "\033[0m\n";
	}

	int idx = 0;
	for ( auto it = hDid->begin(); it != hDid->end(); it++ ) {
		for ( int ts = 0; ts < NS_; ts++ ) {
			hADC[uint32_t(*it)]->Fill( ts, (*hadc)[idx] );
			hfC [uint32_t(*it)]->Fill( ts, (*hfc) [idx] );
			idx++;
		}
	}

	idx = 0;
	for ( auto it = hDid->begin(); it != hDid->end(); it++ ) {
		hHigh[uint32_t(*it)]->Fill( (*hChargeHigh)[idx] );
		hLow [uint32_t(*it)]->Fill( (*hChargeLow )[idx] );
		hSum [uint32_t(*it)]->Fill( (*hChargeSum )[idx] );
		idx++;
	}

	Nevent_++;
	return;
}

void
QWZDC2018Analyzer::endJob() {
	if ( bNorm_ ) {
		for ( auto it = hfC.begin(); it != hfC.end(); it++ ) {
			it->second->Scale(1./Nevent_);
		}
		for ( auto it = hADC.begin(); it != hADC.end(); it++ ) {
			it->second->Scale(1./Nevent_);
		}
	}
	return;
}

DEFINE_FWK_MODULE(QWZDC2018Analyzer);
