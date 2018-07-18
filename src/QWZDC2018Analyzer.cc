#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1D.h"
#include <iostream>

class QWZDC2018Analyzer : public edm::EDAnalyzer {
public:
	explicit QWZDC2018Analyzer(const edm::ParameterSet&);
	~QWZDC2018Analyzer() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob() {};

	edm::InputTag   srcADC_;
	TH1D * hADC[50];
};

QWZDC2018Analyzer::QWZDC2018Analyzer(const edm::ParameterSet& pset) :
	srcADC_(pset.getUntrackedParameter<edm::InputTag>("srcADC"))
{
	consumes<std::vector<double> >(srcADC_);


	std::string cname[] = {
		"hZDCM_EM1",   // 0
		"hZDCM_EM2",   // 1
		"hZDCM_EM3",   // 2
		"hZDCM_EM4",   // 3
		"hZDCM_EM5",   // 4
		"hZDCM_HAD1",  // 5
		"hZDCM_HAD2",  // 6
		"hZDCM_HAD3",  // 7
		"hZDCP_EM1",   // 8
		"hZDCP_EM2",   // 9
		"hZDCP_EM3",   // 10
		"hZDCP_EM4",   // 11
		"hZDCP_EM5",   // 12
		"hZDCP_HAD1",  // 13
		"hZDCP_HAD2",  // 14
		"hZDCP_HAD3",  // 15
		"hZDCM_RPD1",  // 16
		"hZDCM_RPD2",  // 17
		"hZDCM_RPD3",  // 18
		"hZDCM_RPD4",  // 19
		"hZDCM_RPD5",  // 20
		"hZDCM_RPD6",  // 21
		"hZDCM_RPD7",  // 22
		"hZDCM_RPD8",  // 23
		"hZDCM_RPD9",  // 24
		"hZDCM_RPD10", // 25
		"hZDCM_RPD11", // 26
		"hZDCM_RPD12", // 27
		"hZDCM_RPD13", // 28
		"hZDCM_RPD14", // 29
		"hZDCM_RPD15", // 30
		"hZDCM_RPD16", // 31
		"hZDCP_RPD1",  // 32
		"hZDCP_RPD2",  // 33
		"hZDCP_RPD3",  // 34
		"hZDCP_RPD4",  // 35
		"hZDCP_RPD5",  // 36
		"hZDCP_RPD6",  // 37
		"hZDCP_RPD7",  // 38
		"hZDCP_RPD8",  // 39
		"hZDCP_RPD9",  // 40
		"hZDCP_RPD10", // 41
		"hZDCP_RPD11", // 42
		"hZDCP_RPD12", // 43
		"hZDCP_RPD13", // 44
		"hZDCP_RPD14", // 45
		"hZDCP_RPD15", // 46
		"hZDCP_RPD16", // 47
		"hZDCM_HAD4",  // 48 not in 904 runs
		"hZDCP_HAD4",  // 49 not in 904 runs
	};

	edm::Service<TFileService> fs;
	auto fADC = fs->mkdir("ADC");
	for ( int i = 0; i < 50; i++ ) {
		hADC[i] = fADC.make<TH1D>(cname[i].c_str(), cname[i].c_str(), 10, 0, 10);
	}

}

void
QWZDC2018Analyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle<std::vector<double> > hadc;

	iEvent.getByLabel(srcADC_, hadc);
	int sz = hadc->size();
	for ( int ch = 0; ch < sz/10; ch++ ) {
		for ( int i = 0; i < 10; i++ ) {
			hADC[ch]->Fill(i, (*hadc)[10*ch+i]);
		}
	}
	return;
}

DEFINE_FWK_MODULE(QWZDC2018Analyzer);
