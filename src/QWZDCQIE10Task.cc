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
		edm::InputTag	_tagQIE10;
		edm::EDGetTokenT<QIE10DigiCollection> _tokQIE10;

		std::map<uint32_t,MonitorElement*>   _cADC_EChannel;
		std::map<uint32_t,MonitorElement*>   _cADC_vs_TS_EChannel;
		std::map<uint32_t,MonitorElement*>   _cfC_vs_TS_EChannel;

};

QWZDCQIE10Task::QWZDCQIE10Task(edm::ParameterSet const& ps)
{
	//	tags
	_tagQIE10 = ps.getUntrackedParameter<edm::InputTag>("tagQIE10",
		edm::InputTag("hcalDigis", "ZDC"));
	_tokQIE10 = consumes<QIE10DigiCollection>(_tagQIE10);
}

/* virtual */
void QWZDCQIE10Task::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{

	ib.cd();

	//book histos per channel
	std::string histoname;
	for ( int channel = 1; channel < 6; channel++ ) {
		// EM Pos
		HcalZDCDetId didp(HcalZDCDetId::EM, true, channel);
		histoname = "EM_P_" + std::to_string(channel) + "_1";
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_perChannel");
		_cADC_EChannel[didp()] = ib.book1D( histoname.c_str(), histoname.c_str(), 256, 0, 256);
		_cADC_EChannel[didp()]->setAxisTitle("ADC", 1);
		_cADC_EChannel[didp()]->setAxisTitle("N", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel");
		_cADC_vs_TS_EChannel[didp()] = ib.book2D( histoname.c_str(), histoname.c_str(), 10, 0, 10, 256, 0, 256);
		_cADC_vs_TS_EChannel[didp()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_EChannel[didp()]->setAxisTitle("ADC", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_vs_TS_perChannel");
		_cfC_vs_TS_EChannel[didp()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cfC_vs_TS_EChannel[didp()]->setAxisTitle("TS", 1);
		_cfC_vs_TS_EChannel[didp()]->setAxisTitle("sum fC", 2);

		// EM Minus
		HcalZDCDetId didm(HcalZDCDetId::EM, false, channel);
		histoname = "EM_M_" + std::to_string(channel) + "_1";
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_perChannel");
		_cADC_EChannel[didm()] = ib.book1D( histoname.c_str(), histoname.c_str(), 256, 0, 256);
		_cADC_EChannel[didm()]->setAxisTitle("ADC", 1);
		_cADC_EChannel[didm()]->setAxisTitle("N", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel");
		_cADC_vs_TS_EChannel[didm()] = ib.book2D( histoname.c_str(), histoname.c_str(), 10, 0, 10, 256, 0, 256);
		_cADC_vs_TS_EChannel[didm()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_EChannel[didm()]->setAxisTitle("ADC", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_vs_TS_perChannel");
		_cfC_vs_TS_EChannel[didm()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cfC_vs_TS_EChannel[didm()]->setAxisTitle("TS", 1);
		_cfC_vs_TS_EChannel[didm()]->setAxisTitle("sum fC", 2);
	}

	for ( int channel = 1; channel < 5; channel++ ) {
		// HAD Pos
		HcalZDCDetId didp(HcalZDCDetId::HAD, true, channel);
		histoname = "HAD_P_" + std::to_string(channel) + "_" + std::to_string(channel+2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_perChannel");
		_cADC_EChannel[didp()] = ib.book1D( histoname.c_str(), histoname.c_str(), 256, 0, 256);
		_cADC_EChannel[didp()]->setAxisTitle("ADC", 1);
		_cADC_EChannel[didp()]->setAxisTitle("N", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel");
		_cADC_vs_TS_EChannel[didp()] = ib.book2D( histoname.c_str(), histoname.c_str(), 10, 0, 10, 256, 0, 256);
		_cADC_vs_TS_EChannel[didp()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_EChannel[didp()]->setAxisTitle("ADC", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_vs_TS_perChannel");
		_cfC_vs_TS_EChannel[didp()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cfC_vs_TS_EChannel[didp()]->setAxisTitle("TS", 1);
		_cfC_vs_TS_EChannel[didp()]->setAxisTitle("sum fC", 2);

		// HAD Minus
		HcalZDCDetId didm(HcalZDCDetId::HAD, false, channel);
		histoname = "HAD_M_" + std::to_string(channel) + "_" + std::to_string(channel+2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_perChannel");
		_cADC_EChannel[didm()] = ib.book1D( histoname.c_str(), histoname.c_str(), 256, 0, 256);
		_cADC_EChannel[didm()]->setAxisTitle("ADC", 1);
		_cADC_EChannel[didm()]->setAxisTitle("N", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel");
		_cADC_vs_TS_EChannel[didm()] = ib.book2D( histoname.c_str(), histoname.c_str(), 10, 0, 10, 256, 0, 256);
		_cADC_vs_TS_EChannel[didm()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_EChannel[didm()]->setAxisTitle("ADC", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_vs_TS_perChannel");
		_cfC_vs_TS_EChannel[didm()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cfC_vs_TS_EChannel[didm()]->setAxisTitle("TS", 1);
		_cfC_vs_TS_EChannel[didm()]->setAxisTitle("sum fC", 2);
	}

	for ( int channel = 1; channel < 17; channel++ ) {
		// RPD Pos
		HcalZDCDetId didp(HcalZDCDetId::RPD, true, channel);
		histoname = "RPD_P_" + std::to_string(channel) + "_2";
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_perChannel");
		_cADC_EChannel[didp()] = ib.book1D( histoname.c_str(), histoname.c_str(), 256, 0, 256);
		_cADC_EChannel[didp()]->setAxisTitle("ADC", 1);
		_cADC_EChannel[didp()]->setAxisTitle("N", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel");
		_cADC_vs_TS_EChannel[didp()] = ib.book2D( histoname.c_str(), histoname.c_str(), 10, 0, 10, 256, 0, 256);
		_cADC_vs_TS_EChannel[didp()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_EChannel[didp()]->setAxisTitle("ADC", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_vs_TS_perChannel");
		_cfC_vs_TS_EChannel[didp()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cfC_vs_TS_EChannel[didp()]->setAxisTitle("TS", 1);
		_cfC_vs_TS_EChannel[didp()]->setAxisTitle("sum fC", 2);

		// RPD Minus
		HcalZDCDetId didm(HcalZDCDetId::RPD, false, channel);
		histoname = "RPD_M_" + std::to_string(channel) + "_2";
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_perChannel");
		_cADC_EChannel[didm()] = ib.book1D( histoname.c_str(), histoname.c_str(), 256, 0, 256);
		_cADC_EChannel[didm()]->setAxisTitle("ADC", 1);
		_cADC_EChannel[didm()]->setAxisTitle("N", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/ADC_vs_TS_perChannel");
		_cADC_vs_TS_EChannel[didm()] = ib.book2D( histoname.c_str(), histoname.c_str(), 10, 0, 10, 256, 0, 256);
		_cADC_vs_TS_EChannel[didm()]->setAxisTitle("TS", 1);
		_cADC_vs_TS_EChannel[didm()]->setAxisTitle("ADC", 2);
		ib.setCurrentFolder("Hcal/QWZDCQIE10Task/fC_vs_TS_perChannel");
		_cfC_vs_TS_EChannel[didm()] = ib.book1D( histoname.c_str(), histoname.c_str(), 10, 0, 10);
		_cfC_vs_TS_EChannel[didm()]->setAxisTitle("TS", 1);
		_cfC_vs_TS_EChannel[didm()]->setAxisTitle("sum fC", 2);
	}

}


/* virtual */ void QWZDCQIE10Task::analyze(edm::Event const& e, edm::EventSetup const& iSetup)
{
	using namespace edm;
	edm::Handle<QIE10DigiCollection> digis;
	if (!e.getByToken(_tokQIE10, digis))
		edm::LogError("Collection QIE10DigiCollection for ZDC isn't available"
				+ _tagQIE10.label() + " " + _tagQIE10.instance());

	ESHandle<HcalDbService> conditions;
	iSetup.get<HcalDbRecord>().get(conditions);

	for ( auto it = digis->begin(); it != digis->end(); it++ ) {
		const QIE10DataFrame digi = static_cast<const QIE10DataFrame>(*it);
		HcalZDCDetId const& did = digi.detid();

		const HcalQIECoder* channelCoder = conditions->getHcalCoder(did);
		const HcalQIEShape* shape = conditions->getHcalShape(channelCoder);
		const HcalCoderDb coder(*channelCoder, *shape);
		CaloSamples cs;
		coder.adc2fC(digi, cs);

		for ( int i = 0; i < digi.samples(); i++ ) {
			// iter over all samples
			if ( _cADC_EChannel.find( did()) != _cADC_EChannel.end() ) {
				_cADC_EChannel[did()]->Fill(digi[i].adc());
			}
			if ( _cADC_vs_TS_EChannel.find( did() ) != _cADC_vs_TS_EChannel.end() ) {
				_cADC_vs_TS_EChannel[did()]->Fill(i, digi[i].adc());
			}
			if ( _cfC_vs_TS_EChannel.find( did() ) != _cfC_vs_TS_EChannel.end() ) {
				_cfC_vs_TS_EChannel[did()]->Fill(i, cs[i]);
			}

		}
	}
}


DEFINE_FWK_MODULE(QWZDCQIE10Task);
