#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HeavyIonEvent/interface/Centrality.h"

#include "TFile.h"
#include "TH2.h"
#include "TMath.h"
#include "iostream"

using namespace std;
class QWXeXeB2CentProducer : public edm::EDProducer {
public:
	explicit QWXeXeB2CentProducer(const edm::ParameterSet&);
	~QWXeXeB2CentProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///
	edm::InputTag	src_;

	const int bounds[200]={
		0,501,706,869,1005,1121,1231,1335,1427,1513,1592,
		1672,1746,1817,1889,1953,2017,2079,2138,2197,2253,
		2307,2361,2414,2467,2517,2566,2614,2663,2712,2759,
		2804,2849,2894,2938,2979,3020,3061,3102,3143,3182,
		3222,3261,3300,3337,3375,3411,3447,3485,3520,3557,
		3592,3628,3662,3695,3729,3763,3797,3832,3865,3898,
		3930,3962,3993,4024,4055,4087,4118,4148,4178,4208,
		4239,4270,4301,4330,4360,4388,4417,4445,4474,4501,
		4528,4555,4583,4610,4638,4665,4693,4720,4747,4773,
		4799,4825,4851,4877,4903,4929,4954,4979,5004,5029,
		5054,5080,5103,5128,5153,5177,5202,5225,5250,5274,
		5298,5322,5346,5369,5391,5414,5438,5461,5486,5508,
		5532,5555,5578,5600,5622,5645,5668,5691,5713,5734,
		5756,5778,5800,5821,5844,5865,5887,5908,5930,5952,
		5973,5995,6016,6037,6059,6079,6101,6122,6143,6163,
		6185,6205,6227,6248,6268,6289,6310,6331,6353,6374,
		6396,6418,6440,6463,6486,6507,6529,6552,6575,6599,
		6623,6646,6671,6695,6719,6745,6770,6797,6825,6852,
		6881,6912,6942,6974,7007,7042,7077,7116,7156,7199,
		7247,7300,7358,7421,7497,7585,7694,7846,8096};
};

QWXeXeB2CentProducer::QWXeXeB2CentProducer(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src"))
{
	consumes<double>(src_);

	produces< int >("AMPT");
}

QWXeXeB2CentProducer::~QWXeXeB2CentProducer()
{
	return;
}

void QWXeXeB2CentProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;
	edm::Handle<double> b;
	iEvent.getByLabel(src_, b);
	int i;
	for(i=199; i>0; i--) {
		if(500*(*b)<bounds[i]) continue;
		else break;
	}
//	std::cout << " --> 5*b = " << 5*(*b) << " i = " << i << " bounds[i] = " << bounds[i] << std::endl;
	iEvent.put(std::make_unique<int> (i), std::string("AMPT"));
}


DEFINE_FWK_MODULE(QWXeXeB2CentProducer);
