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
	const int bounds_hydjet[200]={
		0,987, 1397, 1711, 1977, 2213, 2421, 2615, 2792, 2962, 3121, 3273, 3419, 3559, 3695, 3825, 3953, 4073, 4191, 4307, 4418,
		4528, 4634, 4739, 4841, 4941, 5038, 5135, 5228, 5320, 5412, 5503, 5591, 5676, 5760, 5845, 5927, 6007, 6088, 6166, 6245,
		6322, 6398, 6475, 6549, 6622, 6696, 6768, 6840, 6910, 6980, 7050, 7117, 7185, 7252, 7317, 7383, 7448, 7512, 7576, 7640,
		7703, 7764, 7826, 7888, 7949, 8010, 8070, 8130, 8188, 8247, 8307, 8364, 8421, 8478, 8536, 8592, 8648, 8703, 8759, 8814,
		8868, 8922, 8976, 9029, 9083, 9136, 9188, 9240, 9292, 9344, 9395, 9446, 9497, 9548, 9598, 9648, 9699, 9749, 9798, 9847,
		9897, 9944, 9992, 10040, 10088, 10136, 10184, 10231, 10277, 10324, 10371, 10418, 10464, 10511, 10557, 10603, 10648, 10694, 10739, 10784,
		10828, 10873, 10917, 10961, 11006, 11050, 11093, 11137, 11180, 11223, 11266, 11309, 11351, 11394, 11437, 11479, 11521, 11563, 11604, 11645,
		11686, 11727, 11769, 11810, 11851, 11892, 11932, 11973, 12013, 12052, 12092, 12132, 12172, 12211, 12251, 12290, 12329, 12368, 12407, 12447,
		12487, 12526, 12565, 12604, 12644, 12683, 12723, 12762, 12802, 12843, 12884, 12925, 12966, 13007, 13050, 13093, 13138, 13182, 13228, 13276,
		13324, 13374, 13425, 13479, 13534, 13590, 13652, 13715, 13782, 13854, 13931, 14016, 14108, 14210, 14325, 14457, 14619, 14824, 15128
	};
};

QWXeXeB2CentProducer::QWXeXeB2CentProducer(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src"))
{
	consumes<double>(src_);

	produces< int >("AMPT");
	produces< int >("Hydjet");
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
	int j;
	for(j=199; j>0; j--) {
		if(1000*(*b)<bounds_hydjet[j]) continue;
		else break;
	}
//	std::cout << " --> 5*b = " << 5*(*b) << " i = " << i << " bounds[i] = " << bounds[i] << std::endl;
	iEvent.put(std::make_unique<int> (i), std::string("AMPT"));
	iEvent.put(std::make_unique<int> (j), std::string("Hydjet"));
}


DEFINE_FWK_MODULE(QWXeXeB2CentProducer);
