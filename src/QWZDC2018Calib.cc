#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/HcalDetId/interface/HcalZDCDetId.h"
#include "TProfile.h"
#include <iostream>

class QWZDC2018Calib : public edm::EDAnalyzer {
public:
	explicit QWZDC2018Calib(const edm::ParameterSet&);
	~QWZDC2018Calib() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob();

	edm::InputTag   srcCapId_;
	edm::InputTag   srcfC_;
	edm::InputTag   srcDid_;
	bool		firstEvent_;
	std::map<uint32_t, TProfile*>	hfC;

	std::map<uint32_t, std::string> cname;
};

QWZDC2018Calib::QWZDC2018Calib(const edm::ParameterSet& pset) :
	srcCapId_(pset.getUntrackedParameter<edm::InputTag>("srcCapId")),
	srcfC_(pset.getUntrackedParameter<edm::InputTag>("srcfC")),
	srcDid_(pset.getUntrackedParameter<edm::InputTag>("srcDetId")),
	firstEvent_(true)
{
	consumes<std::vector<double> >(srcCapId_);
	consumes<std::vector<double> >(srcfC_);
	consumes<std::vector<double> >(srcDid_);

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

}

void
QWZDC2018Calib::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle<std::vector<double> > hCapId;
	Handle<std::vector<double> > hfc;
	Handle<std::vector<double> > hDid;

	iEvent.getByLabel(srcCapId_, hCapId);
	iEvent.getByLabel(srcfC_,  hfc);
	iEvent.getByLabel(srcDid_, hDid);

	int sz = hfc->size();

	int NS_ = sz / hDid->size();

	if ( firstEvent_ ) {
		firstEvent_ = false;
		edm::Service<TFileService> fs;
		for ( auto it = hDid->begin(); it != hDid->end(); it++ ) {
			if ( cname.find( (uint32_t)(*it) ) != cname.end() ) {
				hfC[uint32_t(*it)]  = fs->make<TProfile> (cname[(uint32_t)(*it)].c_str(), cname[(uint32_t)(*it)].c_str(), 4, 0, 4);
			}
		}
	}

	int idx = 0;
	for ( auto it = hDid->begin(); it != hDid->end(); it++ ) {
		for ( int ts = 0; ts < NS_; ts++ ) {
			hfC [uint32_t(*it)]->Fill( (*hCapId)[idx], (*hfc) [idx] );
			idx++;
		}
	}
	return;
}

void
QWZDC2018Calib::endJob() {
	for ( auto it = hfC.begin(); it != hfC.end(); it++ ) {
		std::cout << "\t\tcms.PSet(" << "\n";
		std::cout << "\t\t\tobject = cms.untracked.string('" << cname[it->first] << "'),\n";
		std::cout << "\t\t\tped = cms.untracked.vdouble(" << std::setw(6) << it->second->GetBinContent(1) << ", " << std::setw(6) << it->second->GetBinContent(2) << ", " << std::setw(6) << it->second->GetBinContent(3) << ", " << std::setw(6) << it->second->GetBinContent(4) << ")\n";
		std::cout << "\t\t),\n";
	}
}

DEFINE_FWK_MODULE(QWZDC2018Calib);
