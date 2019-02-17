#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HeavyIonEvent/interface/Centrality.h"

#include "iostream"


using namespace std;
class QWCentrality2018Producer : public edm::EDProducer {
public:
	explicit QWCentrality2018Producer(const edm::ParameterSet&);
	~QWCentrality2018Producer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	virtual void beginRun(const edm::Run&, const edm::EventSetup&) override;
	///

	edm::InputTag	Src_;

};


QWCentrality2018Producer::QWCentrality2018Producer(const edm::ParameterSet& pset) :
	Src_(pset.getUntrackedParameter<edm::InputTag>("Src"))
{
	consumes<reco::Centrality>(Src_);

	produces<double>("etHFhitSum");
	produces<double>("etHFhitSumPlus");
	produces<double>("etHFhitSumMinus");
	produces<double>("etHFtowerSum");
	produces<double>("etHFtowerSumPlus");
	produces<double>("etHFtowerSumMinus");
	produces<double>("etHFtruncated");
	produces<double>("etHFtruncatedPlus");
	produces<double>("etHFtruncatedMinus");
	produces<double>("etEESum");
	produces<double>("etEESumPlus");
	produces<double>("etEESumMinus");
	produces<double>("etEEtruncated");
	produces<double>("etEEtruncatedPlus");
	produces<double>("etEEtruncatedMinus");
	produces<double>("etEBSum");
	produces<double>("etEcalSum");
	produces<double>("etEBtruncated");
	produces<double>("etEcalTrancated");
	produces<double>("zdcSum");
	produces<double>("zdcSumPlus");
	produces<double>("zdcSumMinus");
	produces<double>("pixelMultiplicity");
	produces<double>("trackMultiplicity");
	produces<double>("ntracksPtCut");
	produces<double>("ntracksEtaCut");
	produces<double>("ntracksEtaPtCut");
	produces<double>("nPixelTracks");
	produces<double>("etMidRapiditySum");

}

QWCentrality2018Producer::~QWCentrality2018Producer()
{
	return;
}

void QWCentrality2018Producer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	edm::Handle<reco::Centrality> pCent;
	iEvent.getByLabel( Src_, pCent );

	double etHFhitSum_ = pCent->EtHFhitSum();
	double etHFhitSumPlus_ = pCent->EtHFhitSumPlus();
	double etHFhitSumMinus_ = pCent->EtHFhitSumMinus();

	double etHFtowerSum_ = pCent->EtHFtowerSum();
	double etHFtowerSumPlus_ = pCent->EtHFtowerSumPlus();
	double etHFtowerSumMinus_ = pCent->EtHFtowerSumMinus();

	double etHFtruncated_ = pCent->EtHFtruncated();
	double etHFtruncatedPlus_ = pCent->EtHFtruncatedPlus();
	double etHFtruncatedMinus_ = pCent->EtHFtruncatedMinus();

	double etEESum_ = pCent->EtEESum();
	double etEESumPlus_ = pCent->EtEESumPlus();
	double etEESumMinus_ = pCent->EtEESumMinus();

	double etEEtruncated_ = pCent->EtEEtruncated();
	double etEEtruncatedPlus_ = pCent->EtEEtruncatedPlus();
	double etEEtruncatedMinus_ = pCent->EtEEtruncatedMinus();

	double etEBSum_ = pCent->EtEBSum();
	double etEcalSum_ = pCent->EtEcalSum();

	double etEBtruncated_ = pCent->EtEBtruncated();
	double etEcalTrancated_ = pCent->EtEcaltruncated();

	double zdcSum_ = pCent->zdcSum();
	double zdcSumPlus_ = pCent->zdcSumPlus();
	double zdcSumMinus_ = pCent->zdcSumMinus();

	double pixelMultiplicity_ = pCent->multiplicityPixel();
	double trackMultiplicity_ = pCent->Ntracks();
	double ntracksPtCut_ = pCent->NtracksPtCut();
	double ntracksEtaCut_ = pCent->NtracksEtaCut();
	double ntracksEtaPtCut_ = pCent->NtracksEtaPtCut();
	double nPixelTracks_ = pCent->NpixelTracks();
	double etMidRapiditySum_ = pCent->EtMidRapiditySum();


	//

	iEvent.put(std::make_unique<double>( etHFhitSum_	),	"etHFhitSum");
	iEvent.put(std::make_unique<double>( etHFhitSumPlus_	),	"etHFhitSumPlus");
	iEvent.put(std::make_unique<double>( etHFhitSumMinus_	),	"etHFhitSumMinus");
	iEvent.put(std::make_unique<double>( etHFtowerSum_	),	"etHFtowerSum");
	iEvent.put(std::make_unique<double>( etHFtowerSumPlus_	),	"etHFtowerSumPlus");
	iEvent.put(std::make_unique<double>( etHFtowerSumMinus_	),	"etHFtowerSumMinus");
	iEvent.put(std::make_unique<double>( etHFtruncated_	),	"etHFtruncated");
	iEvent.put(std::make_unique<double>( etHFtruncatedPlus_	),	"etHFtruncatedPlus");
	iEvent.put(std::make_unique<double>( etHFtruncatedMinus_),	"etHFtruncatedMinus");
	iEvent.put(std::make_unique<double>( etEESum_		),	"etEESum");
	iEvent.put(std::make_unique<double>( etEESumPlus_	),	"etEESumPlus");
	iEvent.put(std::make_unique<double>( etEESumMinus_	),	"etEESumMinus");
	iEvent.put(std::make_unique<double>( etEEtruncated_	),	"etEEtruncated");
	iEvent.put(std::make_unique<double>( etEEtruncatedPlus_	),	"etEEtruncatedPlus");
	iEvent.put(std::make_unique<double>( etEEtruncatedMinus_),	"etEEtruncatedMinus");
	iEvent.put(std::make_unique<double>( etEBSum_		),	"etEBSum");
	iEvent.put(std::make_unique<double>( etEcalSum_		),	"etEcalSum");
	iEvent.put(std::make_unique<double>( etEBtruncated_	),	"etEBtruncated");
	iEvent.put(std::make_unique<double>( etEcalTrancated_	),	"etEcalTrancated");
	iEvent.put(std::make_unique<double>( zdcSum_		),	"zdcSum");
	iEvent.put(std::make_unique<double>( zdcSumPlus_	),	"zdcSumPlus");
	iEvent.put(std::make_unique<double>( zdcSumMinus_	),	"zdcSumMinus");
	iEvent.put(std::make_unique<double>( pixelMultiplicity_	),	"pixelMultiplicity");
	iEvent.put(std::make_unique<double>( trackMultiplicity_	),	"trackMultiplicity");
	iEvent.put(std::make_unique<double>( ntracksPtCut_	),	"ntracksPtCut");
	iEvent.put(std::make_unique<double>( ntracksEtaCut_	),	"ntracksEtaCut");
	iEvent.put(std::make_unique<double>( ntracksEtaPtCut_	),	"ntracksEtaPtCut");
	iEvent.put(std::make_unique<double>( nPixelTracks_	),	"nPixelTracks");
	iEvent.put(std::make_unique<double>( etMidRapiditySum_	),	"etMidRapiditySum");

}


void QWCentrality2018Producer::beginRun(const edm::Run &r, const edm::EventSetup & iSetup)
{
	return;
}


DEFINE_FWK_MODULE(QWCentrality2018Producer);
