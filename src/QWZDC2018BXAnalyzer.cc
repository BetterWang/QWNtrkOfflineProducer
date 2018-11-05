/*
 *	Author:			Quan Wang
 *	Description:
 *		Replica of QWZDC2018BXTask as an EDAnalyzer
 */


//#include "DQM/HcalCommon/interface/ElectronicsMap.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalPulseShapes.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

class QWZDC2018BXAnalyzer : public edm::EDAnalyzer {
	public:
		QWZDC2018BXAnalyzer(edm::ParameterSet const&);
		~QWZDC2018BXAnalyzer() override{}

	protected:
		virtual void analyze(edm::Event const&, edm::EventSetup const&) override;

		//	tags
		edm::InputTag   BX_;
		edm::InputTag   srcfC_;
		edm::InputTag   srcDid_;
		int		SOI_;
		bool		firstEvent_;

		TH1D *				hc_;
		std::map<uint32_t, TH1D*>	_fC_BX_perChannel;
		std::map<uint32_t, std::string>	cname_;
};

QWZDC2018BXAnalyzer::QWZDC2018BXAnalyzer(edm::ParameterSet const& pset) :
	BX_(pset.getUntrackedParameter<edm::InputTag>("BX")),
	srcfC_(pset.getUntrackedParameter<edm::InputTag>("srcfC")),
	srcDid_(pset.getUntrackedParameter<edm::InputTag>("srcDetId")),
	SOI_(pset.getUntrackedParameter<int>("SOI", 4)),
	firstEvent_(true)
{
	consumes<double>(BX_);
	consumes<std::vector<double>>(srcfC_);
	consumes<std::vector<double>>(srcDid_);

	//book histos per channel
	std::string histoname;
	for ( int channel = 0; channel < 16; channel++ ) {
		// EM Pos
		HcalZDCDetId did(HcalZDCDetId::EM, true, channel);
		histoname = std::string("hZDCP_EM") + std::to_string(channel);
		cname_[did()] = histoname;

		// EM Minus
		did = HcalZDCDetId(HcalZDCDetId::EM, false, channel);
		histoname = std::string("hZDCM_EM") + std::to_string(channel);
		cname_[did()] = histoname;

		// HAD Pos
		did = HcalZDCDetId(HcalZDCDetId::HAD, true, channel);
		histoname = std::string("hZDCP_HAD") + std::to_string(channel);
		cname_[did()] = histoname;

		// HAD Minus
		did = HcalZDCDetId(HcalZDCDetId::HAD, false, channel);
		histoname = std::string("hZDCM_HAD") + std::to_string(channel);
		cname_[did()] = histoname;

		// RPD Pos
		did = HcalZDCDetId(HcalZDCDetId::RPD, true, channel+1);
		histoname = std::string("hZDCP_RPD") + std::to_string(channel);
		cname_[did()] = histoname;

		// RPD Minus
		did = HcalZDCDetId(HcalZDCDetId::RPD, false, channel+1);
		histoname = std::string("hZDCM_RPD") + std::to_string(channel);
		cname_[did()] = histoname;
	}

}


void QWZDC2018BXAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle<double> hBX;
	Handle<std::vector<double>> hfC;
	Handle<std::vector<double>> hDid;

	iEvent.getByLabel(BX_, hBX);
	iEvent.getByLabel(srcfC_, hfC);
	iEvent.getByLabel(srcDid_, hDid);

	int NS_ = hfC->size() / hDid->size();
	int BX = *hBX;
	if ( firstEvent_ ) {
		firstEvent_ = false;
		std::cout << "\033[1;31mNsamples = " << NS_ << "\033[0m\n";

		edm::Service<TFileService> fs;
		auto fdir = fs->mkdir("fC_vs_BX_perChannel");

		hc_ = fs->make<TH1D>("fC_vs_BX", "fC_vs_BX", 4000, -100, 3900);
		hc_->GetXaxis()->SetTitle("BX");
		hc_->GetYaxis()->SetTitle("sum fC");

		for ( auto it = hDid->begin(); it != hDid->end(); it++ ) {
			_fC_BX_perChannel[uint32_t(*it)] = fdir.make<TH1D>( cname_[uint32_t(*it)].c_str(), cname_[uint32_t(*it)].c_str(), 4000, -100, 3900);
			_fC_BX_perChannel[uint32_t(*it)]->GetXaxis()->SetTitle("BX");
			_fC_BX_perChannel[uint32_t(*it)]->GetYaxis()->SetTitle("sum fC");
		}
	}

	{
		int idx = 0;
		for ( auto it = hDid->begin(); it != hDid->end(); it++ ) {
			for ( int ts = 0; ts < NS_; ts++ ) {
				double q = (*hfC)[ idx ];
				hc_->Fill( BX + ts - SOI_, q );
				_fC_BX_perChannel[ uint32_t(*it) ]->Fill( BX + ts - SOI_, q );
				idx++;
			}
		}
	}

}


DEFINE_FWK_MODULE(QWZDC2018BXAnalyzer);
