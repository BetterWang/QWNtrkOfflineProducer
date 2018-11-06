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

class QWZDCQIE10Task : public DQMEDAnalyzer
{
	public:
		QWZDCQIE10Task(edm::ParameterSet const&);
		~QWZDCQIE10Task() override{}

		void bookHistograms(DQMStore::IBooker&,
		edm::Run const&, edm::EventSetup const&) override;

	protected:
		void analyze(edm::Event const&, edm::EventSetup const&) override;

		//	tags
		edm::InputTag   srcADC_;
		edm::InputTag   srcfC_;
		edm::InputTag   srcDid_;
		edm::InputTag   srcHigh_;
		edm::InputTag   srcLow_;
		edm::InputTag   srcSum_;
		bool		firstEvent_;

		std::map<uint32_t,MonitorElement*>   _cADC_EChannel;
		std::map<uint32_t,MonitorElement*>   _cADC_vs_TS_EChannel;
		std::map<uint32_t,MonitorElement*>   _cADC_vs_TS_1D;
		std::map<uint32_t,MonitorElement*>   _cfC_vs_TS_EChannel;

		std::map<uint32_t,MonitorElement*>   _cfC_HighGain_EChannel;
		std::map<uint32_t,MonitorElement*>   _cfC_LowGain_EChannel;
		std::map<uint32_t,MonitorElement*>   _cfC_SumGain_EChannel;
};

QWZDCQIE10Task::QWZDCQIE10Task(edm::ParameterSet const& pset) :
	srcADC_(pset.getUntrackedParameter<edm::InputTag>("srcADC")),
	srcfC_(pset.getUntrackedParameter<edm::InputTag>("srcfC")),
	srcDid_(pset.getUntrackedParameter<edm::InputTag>("srcDetId")),
	srcHigh_(pset.getUntrackedParameter<edm::InputTag>("srcHigh")),
	srcLow_(pset.getUntrackedParameter<edm::InputTag>("srcLow")),
	srcSum_(pset.getUntrackedParameter<edm::InputTag>("srcSum")),
	firstEvent_(true)
{
	consumes<std::vector<double> >(srcADC_);
	consumes<std::vector<double> >(srcfC_);
	consumes<std::vector<double> >(srcDid_);
	consumes<std::vector<double> >(srcHigh_);
	consumes<std::vector<double> >(srcLow_);
	consumes<std::vector<double> >(srcSum_);
}

/* virtual */
void QWZDCQIE10Task::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{

	ib.cd();

	//book histos per channel
	std::string histoname;
	for ( int channel = 0; channel < 16; channel++ ) {
		// EM Pos
		HcalZDCDetId did(HcalZDCDetId::EM, true, channel);
		histoname = std::string("hZDCP_EM") + std::to_string(channel);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_perChannel");
		_cADC_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 256, 0, 256);
		_cADC_EChannel[did()]->setAxisTitle("ADC", 1);
		_cADC_EChannel[did()]->setAxisTitle("N", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel");
		_cADC_vs_TS_EChannel[did()] = ib.book2D( histoname.c_str(), histoname.c_str(), 10, 0, 10, 256, 0, 256);
		_cADC_vs_TS_EChannel[did()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_EChannel[did()]->setAxisTitle("ADC", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_vs_TS_perChannel");
		_cfC_vs_TS_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cfC_vs_TS_EChannel[did()]->setAxisTitle("TS", 1);
		_cfC_vs_TS_EChannel[did()]->setAxisTitle("sum fC", 2);

		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_HighGain_perChannel");
		_cfC_HighGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_HighGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_HighGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_LowGain_perChannel");
		_cfC_LowGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_LowGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_LowGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_SumGain_perChannel");
		_cfC_SumGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_SumGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_SumGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel1D");
		_cADC_vs_TS_1D[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cADC_vs_TS_1D[did()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_1D[did()]->setAxisTitle("Sum ADC", 2);

		// EM Minus
		did = HcalZDCDetId(HcalZDCDetId::EM, false, channel);
		histoname = std::string("hZDCM_EM") + std::to_string(channel);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_perChannel");
		_cADC_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 256, 0, 256);
		_cADC_EChannel[did()]->setAxisTitle("ADC", 1);
		_cADC_EChannel[did()]->setAxisTitle("N", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel");
		_cADC_vs_TS_EChannel[did()] = ib.book2D( histoname.c_str(), histoname.c_str(), 10, 0, 10, 256, 0, 256);
		_cADC_vs_TS_EChannel[did()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_EChannel[did()]->setAxisTitle("ADC", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_vs_TS_perChannel");
		_cfC_vs_TS_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cfC_vs_TS_EChannel[did()]->setAxisTitle("TS", 1);
		_cfC_vs_TS_EChannel[did()]->setAxisTitle("sum fC", 2);

		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_HighGain_perChannel");
		_cfC_HighGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_HighGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_HighGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_LowGain_perChannel");
		_cfC_LowGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_LowGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_LowGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_SumGain_perChannel");
		_cfC_SumGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_SumGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_SumGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel1D");
		_cADC_vs_TS_1D[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cADC_vs_TS_1D[did()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_1D[did()]->setAxisTitle("Sum ADC", 2);

		// HAD Pos
		did = HcalZDCDetId(HcalZDCDetId::HAD, true, channel);
		histoname = std::string("hZDCP_HAD") + std::to_string(channel);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_perChannel");
		_cADC_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 256, 0, 256);
		_cADC_EChannel[did()]->setAxisTitle("ADC", 1);
		_cADC_EChannel[did()]->setAxisTitle("N", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel");
		_cADC_vs_TS_EChannel[did()] = ib.book2D( histoname.c_str(), histoname.c_str(), 10, 0, 10, 256, 0, 256);
		_cADC_vs_TS_EChannel[did()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_EChannel[did()]->setAxisTitle("ADC", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_vs_TS_perChannel");
		_cfC_vs_TS_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cfC_vs_TS_EChannel[did()]->setAxisTitle("TS", 1);
		_cfC_vs_TS_EChannel[did()]->setAxisTitle("sum fC", 2);

		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_HighGain_perChannel");
		_cfC_HighGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_HighGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_HighGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_LowGain_perChannel");
		_cfC_LowGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_LowGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_LowGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_SumGain_perChannel");
		_cfC_SumGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_SumGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_SumGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel1D");
		_cADC_vs_TS_1D[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cADC_vs_TS_1D[did()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_1D[did()]->setAxisTitle("Sum ADC", 2);

		// HAD Minus
		did = HcalZDCDetId(HcalZDCDetId::HAD, false, channel);
		histoname = std::string("hZDCM_HAD") + std::to_string(channel);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_perChannel");
		_cADC_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 256, 0, 256);
		_cADC_EChannel[did()]->setAxisTitle("ADC", 1);
		_cADC_EChannel[did()]->setAxisTitle("N", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel");
		_cADC_vs_TS_EChannel[did()] = ib.book2D( histoname.c_str(), histoname.c_str(), 10, 0, 10, 256, 0, 256);
		_cADC_vs_TS_EChannel[did()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_EChannel[did()]->setAxisTitle("ADC", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_vs_TS_perChannel");
		_cfC_vs_TS_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cfC_vs_TS_EChannel[did()]->setAxisTitle("TS", 1);
		_cfC_vs_TS_EChannel[did()]->setAxisTitle("sum fC", 2);

		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_HighGain_perChannel");
		_cfC_HighGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_HighGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_HighGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_LowGain_perChannel");
		_cfC_LowGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_LowGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_LowGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_SumGain_perChannel");
		_cfC_SumGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_SumGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_SumGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel1D");
		_cADC_vs_TS_1D[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cADC_vs_TS_1D[did()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_1D[did()]->setAxisTitle("Sum ADC", 2);

		// RPD Pos
		did = HcalZDCDetId(HcalZDCDetId::RPD, true, channel+1);
		histoname = std::string("hZDCP_RPD") + std::to_string(channel);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_perChannel");
		_cADC_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 256, 0, 256);
		_cADC_EChannel[did()]->setAxisTitle("ADC", 1);
		_cADC_EChannel[did()]->setAxisTitle("N", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel");
		_cADC_vs_TS_EChannel[did()] = ib.book2D( histoname.c_str(), histoname.c_str(), 10, 0, 10, 256, 0, 256);
		_cADC_vs_TS_EChannel[did()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_EChannel[did()]->setAxisTitle("ADC", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_vs_TS_perChannel");
		_cfC_vs_TS_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cfC_vs_TS_EChannel[did()]->setAxisTitle("TS", 1);
		_cfC_vs_TS_EChannel[did()]->setAxisTitle("sum fC", 2);

		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_HighGain_perChannel");
		_cfC_HighGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_HighGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_HighGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_LowGain_perChannel");
		_cfC_LowGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_LowGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_LowGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_SumGain_perChannel");
		_cfC_SumGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_SumGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_SumGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel1D");
		_cADC_vs_TS_1D[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cADC_vs_TS_1D[did()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_1D[did()]->setAxisTitle("Sum ADC", 2);

		// RPD Minus
		did = HcalZDCDetId(HcalZDCDetId::RPD, false, channel+1);
		histoname = std::string("hZDCM_RPD") + std::to_string(channel);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_perChannel");
		_cADC_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 256, 0, 256);
		_cADC_EChannel[did()]->setAxisTitle("ADC", 1);
		_cADC_EChannel[did()]->setAxisTitle("N", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel");
		_cADC_vs_TS_EChannel[did()] = ib.book2D( histoname.c_str(), histoname.c_str(), 10, 0, 10, 256, 0, 256);
		_cADC_vs_TS_EChannel[did()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_EChannel[did()]->setAxisTitle("ADC", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_vs_TS_perChannel");
		_cfC_vs_TS_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cfC_vs_TS_EChannel[did()]->setAxisTitle("TS", 1);
		_cfC_vs_TS_EChannel[did()]->setAxisTitle("sum fC", 2);

		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_HighGain_perChannel");
		_cfC_HighGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_HighGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_HighGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_LowGain_perChannel");
		_cfC_LowGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_LowGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_LowGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_SumGain_perChannel");
		_cfC_SumGain_EChannel[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 3500, 0, 35000);
		_cfC_SumGain_EChannel[did()]->setAxisTitle("fC", 1);
		_cfC_SumGain_EChannel[did()]->setAxisTitle("Count", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel1D");
		_cADC_vs_TS_1D[did()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cADC_vs_TS_1D[did()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_1D[did()]->setAxisTitle("Sum ADC", 2);
	}

}


/* virtual */ void QWZDCQIE10Task::analyze(edm::Event const& iEvent, edm::EventSetup const& iSetup)
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
		std::cout << "\033[1;31mNsamples = " << NS_ << "\033[0m\n";
	}


	{
		int idx = 0;
		int idid = 0;
		for ( auto it = hDid->begin() ; it != hDid->end(); it++ ) {
			if ( _cADC_EChannel.find(uint32_t(*it)) == _cADC_EChannel.end() ) continue;
			for ( int ts = 0; ts < NS_; ts++ ) {
				_cADC_EChannel[uint32_t(*it)]->Fill((*hadc)[idx]);
				_cADC_vs_TS_EChannel[uint32_t(*it)]->Fill(ts, (*hadc)[idx]);
				_cADC_vs_TS_1D[uint32_t(*it)]->Fill(ts, (*hadc)[idx]);
				_cfC_vs_TS_EChannel[uint32_t(*it)]->Fill(ts, (*hadc)[idx]);
				idx++;
			}
			_cfC_HighGain_EChannel[uint32_t(*it)]->Fill( (*hChargeHigh)[idid]);
			_cfC_LowGain_EChannel [uint32_t(*it)]->Fill( (*hChargeLow )[idid]);
			_cfC_SumGain_EChannel [uint32_t(*it)]->Fill( (*hChargeSum )[idid]);
			idid++;
		}
	}
}


DEFINE_FWK_MODULE(QWZDCQIE10Task);
