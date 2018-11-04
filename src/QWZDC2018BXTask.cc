/*
 *	Author:			Quan Wang
 *	Description:
 *		Task for ZDC Read out
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

#include "DQMServices/Core/interface/DQMEDAnalyzer.h"

#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalPulseShapes.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

class QWZDC2018BXTask : public DQMEDAnalyzer
{
	public:
		QWZDC2018BXTask(edm::ParameterSet const&);
		~QWZDC2018BXTask() override{}

		void bookHistograms(DQMStore::IBooker&,
		edm::Run const&, edm::EventSetup const&) override;

	protected:
		void analyze(edm::Event const&, edm::EventSetup const&) override;

		//	tags
		edm::InputTag   BX_;
		edm::InputTag   srcfC_;
		edm::InputTag   srcDid_;
		int		SOI_;
		int		BXShift_;
		bool		firstEvent_;

		MonitorElement *			hc;
		std::map<uint32_t,MonitorElement*>	_fC_BX_perChannel;
};

QWZDC2018BXTask::QWZDC2018BXTask(edm::ParameterSet const& pset) :
	BX_(pset.getUntrackedParameter<edm::InputTag>("BX")),
	srcfC_(pset.getUntrackedParameter<edm::InputTag>("srcfC")),
	srcDid_(pset.getUntrackedParameter<edm::InputTag>("srcDid")),
	SOI_(pset.getUntrackedParameter<int>("SOI", 4)),
	BXShift_(pset.getUntrackedParameter<int>("BXShift", 100)),
	firstEvent_(true)
{
	consumes<double>(BX_);
	consumes<std::vector<double>>(srcfC_);
	consumes<std::vector<double>>(srcDid_);
}

/* virtual */
void QWZDC2018BXTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{

	ib.cd();
	ib.setCurrentFolder("Hcal/ZDC_BX_timein");
	hc = ib.book1D("fC_vs_BX", "fC_vs_BX", 4000, -100, 3900);
	hc->setAxisTitle("BX", 1);
	hc->setAxisTitle("sum fC", 2);

	//book histos per channel
	ib.setCurrentFolder("Hcal/ZDC_BX_timein/fC_vs_BX_perChannel");
	std::string histoname;
	for ( int channel = 0; channel < 16; channel++ ) {
		// EM Pos
		HcalZDCDetId did(HcalZDCDetId::EM, true, channel);
		histoname = std::string("hZDCP_EM") + std::to_string(channel);
		_fC_BX_perChannel[did()] = ib.book1D( histoname, histoname, 4000, -100, 3900);
		_fC_BX_perChannel[did()]->setAxisTitle("BX", 1);
		_fC_BX_perChannel[did()]->setAxisTitle("sum fC", 2);

		// EM Minus
		did = HcalZDCDetId(HcalZDCDetId::EM, false, channel);
		histoname = std::string("hZDCM_EM") + std::to_string(channel);
		_fC_BX_perChannel[did()] = ib.book1D( histoname, histoname, 4000, -100, 3900);
		_fC_BX_perChannel[did()]->setAxisTitle("BX", 1);
		_fC_BX_perChannel[did()]->setAxisTitle("sum fC", 2);

		// HAD Pos
		did = HcalZDCDetId(HcalZDCDetId::HAD, true, channel);
		histoname = std::string("hZDCP_HAD") + std::to_string(channel);
		_fC_BX_perChannel[did()] = ib.book1D( histoname, histoname, 4000, -100, 3900);
		_fC_BX_perChannel[did()]->setAxisTitle("BX", 1);
		_fC_BX_perChannel[did()]->setAxisTitle("sum fC", 2);

		// HAD Minus
		did = HcalZDCDetId(HcalZDCDetId::HAD, false, channel);
		histoname = std::string("hZDCM_HAD") + std::to_string(channel);
		_fC_BX_perChannel[did()] = ib.book1D( histoname, histoname, 4000, -100, 3900);
		_fC_BX_perChannel[did()]->setAxisTitle("BX", 1);
		_fC_BX_perChannel[did()]->setAxisTitle("sum fC", 2);

		// RPD Pos
		did = HcalZDCDetId(HcalZDCDetId::RPD, true, channel+1);
		histoname = std::string("hZDCP_RPD") + std::to_string(channel);
		_fC_BX_perChannel[did()] = ib.book1D( histoname, histoname, 4000, -100, 3900);
		_fC_BX_perChannel[did()]->setAxisTitle("BX", 1);
		_fC_BX_perChannel[did()]->setAxisTitle("sum fC", 2);

		// RPD Minus
		did = HcalZDCDetId(HcalZDCDetId::RPD, false, channel+1);
		histoname = std::string("hZDCM_RPD") + std::to_string(channel);
		_fC_BX_perChannel[did()] = ib.book1D( histoname, histoname, 4000, -100, 3900);
		_fC_BX_perChannel[did()]->setAxisTitle("BX", 1);
		_fC_BX_perChannel[did()]->setAxisTitle("sum fC", 2);
	}

}


/* virtual */ void QWZDC2018BXTask::analyze(edm::Event const& iEvent, edm::EventSetup const& iSetup)
{
	using namespace edm;
	Handle<double> hBX;
	Handle<std::vector<double>> hfC;
	Handle<std::vector<double>> hDid;

	iEvent.getByLabel(BX_, hBX);
	iEvent.getByLabel(srcfC_, hfC);
	iEvent.getByLabel(srcDid_, hDid);

	int NS_ = hfC->size() / hDid->size();
	int BX = *hBX + BXShift_;
	if ( firstEvent_ ) {
		firstEvent_ = false;
		std::cout << "\033[1;31mNsamples = " << NS_ << "\033[0m\n";
	}

	{
		int idx = 0;
		for ( auto it = hDid->begin(); it != hDid->end(); it++ ) {
			for ( int ts = 0; ts < NS_; ts++ ) {
				double q = (*hfC)[ idx ];
				hc->Fill( BX + ts - SOI_, q );
				_fC_BX_perChannel[ uint32_t(*it) ]->Fill( BX + ts - SOI_, q );
				idx++;
			}
		}
	}

}


DEFINE_FWK_MODULE(QWZDC2018BXTask);
