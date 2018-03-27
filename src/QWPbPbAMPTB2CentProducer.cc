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
class QWPbPbAMPTB2CentProducer : public edm::EDProducer {
public:
	explicit QWPbPbAMPTB2CentProducer(const edm::ParameterSet&);
	~QWPbPbAMPTB2CentProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///
	edm::InputTag	src_;

	const int bounds[200]={
		0, 1147, 1630, 2003, 2313, 2586, 2836, 3063, 3274, 3473, 3661, 3844, 4018, 4182, 4339, 4493, 4641, 4782, 4923, 5057, 5187,
		5317, 5443, 5567, 5687, 5805, 5922, 6038, 6149, 6256, 6365, 6472, 6577, 6678, 6777, 6876, 6971, 7069, 7164, 7259, 7350,
		7441, 7532, 7621, 7708, 7793, 7879, 7964, 8050, 8132, 8213, 8298, 8377, 8457, 8536, 8614, 8691, 8769, 8844, 8920, 8997,
		9071, 9146, 9221, 9293, 9367, 9439, 9509, 9578, 9650, 9721, 9790, 9857, 9925, 9992, 10058, 10123, 10190, 10255, 10321, 10385,
		10449, 10514, 10577, 10640, 10703, 10767, 10831, 10893, 10953, 11016, 11077, 11138, 11198, 11256, 11316, 11374, 11432, 11492, 11549, 11607,
		11664, 11722, 11779, 11836, 11893, 11950, 12005, 12061, 12118, 12176, 12232, 12286, 12341, 12394, 12448, 12502, 12556, 12610, 12664, 12717,
		12770, 12822, 12875, 12926, 12977, 13028, 13078, 13128, 13180, 13231, 13283, 13333, 13383, 13434, 13484, 13534, 13584, 13635, 13686, 13735,
		13783, 13832, 13880, 13928, 13976, 14025, 14073, 14120, 14168, 14214, 14263, 14310, 14359, 14407, 14454, 14500, 14548, 14595, 14643, 14691,
		14739, 14787, 14836, 14883, 14933, 14982, 15032, 15081, 15131, 15182, 15234, 15286, 15339, 15391, 15445, 15502, 15559, 15616, 15675, 15737,
		15798, 15863, 15928, 15996, 16068, 16142, 16221, 16302, 16388, 16482, 16583, 16693, 16815, 16952, 17107, 17286, 17513, 17825, 18347
	};
};

QWPbPbAMPTB2CentProducer::QWPbPbAMPTB2CentProducer(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src"))
{
	consumes<double>(src_);

	produces< int >("AMPT");
}

QWPbPbAMPTB2CentProducer::~QWPbPbAMPTB2CentProducer()
{
	return;
}

void QWPbPbAMPTB2CentProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;
	edm::Handle<double> b;
	iEvent.getByLabel(src_, b);
	int i;
	for(i=199; i>0; i--) {
		if(1000*(*b)<bounds[i]) continue;
		else break;
	}
//	std::cout << " --> 5*b = " << 5*(*b) << " i = " << i << " bounds[i] = " << bounds[i] << std::endl;
	iEvent.put(std::make_unique<int> (i), std::string("AMPT"));
}


DEFINE_FWK_MODULE(QWPbPbAMPTB2CentProducer);
