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
		edm::InputTag   fC_;
		int		NS_;
		int		nChannel_;
		int		nSig_;

		MonitorElement *	hc;
		MonitorElement *	_fC_BX_perChannel[50];
};

QWZDC2018BXTask::QWZDC2018BXTask(edm::ParameterSet const& pset) :
	BX_(pset.getUntrackedParameter<edm::InputTag>("BX")),
	fC_(pset.getUntrackedParameter<edm::InputTag>("fC")),
	NS_(pset.getUntrackedParameter<int>("NS", 10)),
	nChannel_(pset.getUntrackedParameter<int>("nChannel")),
	nSig_(pset.getUntrackedParameter<int>("soi", 3))
{
	consumes<double>(BX_);
	consumes<std::vector<double>>(fC_);
}

/* virtual */
void QWZDC2018BXTask::bookHistograms(DQMStore::IBooker &ib,
	edm::Run const& r, edm::EventSetup const& es)
{

	ib.cd();
	ib.setCurrentFolder("Hcal/ZDC_BX_timein");
	hc = ib.book1D("fC_vs_BX", "fC_vs_BX", 4000, 0, 4000);
	hc->setAxisTitle("BX", 1);
	hc->setAxisTitle("sum fC", 2);

	//book histos per channel
	ib.setCurrentFolder("Hcal/ZDC_BX_timein/fC_vs_BX_perChannel");
	const char * histoname[50] = {
		"EM_M_1",
		"EM_M_2",
		"EM_M_3",
		"EM_M_4",
		"EM_M_5",
		"HAD_M_1",
		"HAD_M_2",
		"HAD_M_3",
		"HAD_M_4",
		"EM_P_1",
		"EM_P_2",
		"EM_P_3",
		"EM_P_4",
		"EM_P_5",
		"HAD_P_1",
		"HAD_P_2",
		"HAD_P_3",
		"HAD_P_4",
		"RPD_M_1",
		"RPD_M_2",
		"RPD_M_3",
		"RPD_M_4",
		"RPD_M_5",
		"RPD_M_6",
		"RPD_M_7",
		"RPD_M_8",
		"RPD_M_9",
		"RPD_M_10",
		"RPD_M_11",
		"RPD_M_12",
		"RPD_M_13",
		"RPD_M_14",
		"RPD_M_15",
		"RPD_M_16",
		"RPD_P_1",
		"RPD_P_2",
		"RPD_P_3",
		"RPD_P_4",
		"RPD_P_5",
		"RPD_P_6",
		"RPD_P_7",
		"RPD_P_8",
		"RPD_P_9",
		"RPD_P_10",
		"RPD_P_11",
		"RPD_P_12",
		"RPD_P_13",
		"RPD_P_14",
		"RPD_P_15",
		"RPD_P_16"
	};
	for ( int i = 0; i < 50; i++ ) {
		_fC_BX_perChannel[i] = ib.book1D( histoname[i], histoname[i], 4000, 0, 4000);
		_fC_BX_perChannel[i]->setAxisTitle("BX", 1);
		_fC_BX_perChannel[i]->setAxisTitle("sum fC", 2);
	}
}


/* virtual */ void QWZDC2018BXTask::analyze(edm::Event const& iEvent, edm::EventSetup const& iSetup)
{
	using namespace edm;
	Handle<double> hBX;
	Handle<std::vector<double>> hfC;

	iEvent.getByLabel(BX_, hBX);
	iEvent.getByLabel(fC_, hfC);

	for ( int i = 0; i < 50; i++ ) {
		for ( int ts = 0; ts < NS_; ts++ ) {
			double q = (*hfC)[ int(NS_ * i + ts) ];
			_fC_BX_perChannel[i]->Fill( (*hBX)+ts-nSig_, q );
			hc->Fill( (*hBX)+ts-nSig_, q );
		}
	}
}


DEFINE_FWK_MODULE(QWZDC2018BXTask);
