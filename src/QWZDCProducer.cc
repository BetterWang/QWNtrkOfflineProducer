#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalPulseShapes.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "iostream"

#include <unordered_map>

using namespace std;
class QWZDCProducer : public edm::EDProducer {
public:
	explicit QWZDCProducer(const edm::ParameterSet&);
	~QWZDCProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///

	edm::InputTag	Src_;

	unordered_map<int, vector<double> >  const hilo_ratio_ = {
		{286178, {0.101456, 0.0921307, 0.0737181, 0.0649541, 0.0760209, 0.0955824, 0.098161, 0.0787626, 0.0951242}},
		{286200, {0.0995503, 0.0887865, 0.0714984, 0.0564896, 0.073293, 0.0917825, 0.0942577, 0.0776064, 0.0938577}},
		{286201, {0.0996868, 0.0892797, 0.0719536, 0.0569093, 0.0741179, 0.0926197, 0.0949234, 0.0777105, 0.0940316}},
		{286288, {0.0996525, 0.0883002, 0.0717301, 0.0562979, 0.0739234, 0.091685, 0.094193, 0.0775277, 0.0938492}},
		{286301, {0.100398, 0.0906163, 0.072981, 0.0618954, 0.075815, 0.0940047, 0.0964657, 0.0783921, 0.0943756}},
		{286302, {0.100989, 0.0920262, 0.073589, 0.0666262, 0.0767815, 0.0950682, 0.0979009, 0.0787675, 0.0944607}},
		{286309, {0.101231, 0.0927904, 0.0736417, 0.0710078, 0.0770608, 0.0955825, 0.0985583, 0.0788699, 0.0944062}},
		{286314, {0.10107, 0.0928507, 0.0735201, 0.0711714, 0.0768765, 0.0955292, 0.0985479, 0.0787742, 0.0942272}},
		{286327, {0.100372, 0.0908043, 0.073077, 0.0622925, 0.0757536, 0.094053, 0.0968709, 0.0784046, 0.0944128}},
		{286329, {0.0998273, 0.0895008, 0.0723493, 0.0578334, 0.0744077, 0.0926813, 0.0955043, 0.0780124, 0.0941527}},
		{286365, {0.100515, 0.090798, 0.0729713, 0.0619827, 0.0757911, 0.0940925, 0.0968624, 0.078394, 0.0944329}},
		{286420, {0.0996654, 0.0891266, 0.0720619, 0.0573825, 0.0739155, 0.0922591, 0.0948796, 0.0777361, 0.0941135}},
		{286422, {0.0997191, 0.0892974, 0.072073, 0.0575806, 0.074795, 0.0923891, 0.0952604, 0.0777223, 0.0941788}},
		{286425, {0.100561, 0.0909779, 0.0731912, 0.0633512, 0.0760494, 0.0942059, 0.0970197, 0.0785329, 0.0943925}},
		{286441, {0.100047, 0.0901258, 0.0724894, 0.0600918, 0.0752657, 0.0931614, 0.0959933, 0.0780913, 0.0943971}},
		{286442, {0.100453, 0.0906358, 0.0730142, 0.0615559, 0.0759813, 0.0940093, 0.0968645, 0.0783091, 0.09441}},
		{286450, {0.100495, 0.0908488, 0.0732224, 0.0626892, 0.0759236, 0.0941719, 0.0970464, 0.078405, 0.0943366}},
		{286471, {0.0999455, 0.0898587, 0.0723992, 0.0579187, 0.0745072, 0.0932623, 0.0959074, 0.0779568, 0.0942506}},
		{286496, {0.100246, 0.0903138, 0.0729581, 0.06099, 0.0755684, 0.0938663, 0.0965225, 0.078246, 0.0944352}},
		{286514, {0.0, 0.0, 0.0819553, 0.0, 0.0, 0.0969938, 0.083415, 0.094552, 0.0}},
		{286515, {0.0, 0.0, 0.0818176, 0.0, 0.0, 0.0969775, 0.0834413, 0.0945504, 0.0}},
		{286516, {0.0, 0.0, 0.0818498, 0.0, 0.0, 0.0969311, 0.0833954, 0.0945373, 0.0}},
		{286517, {0.0, 0.0, 0.0817458, 0.0, 0.0, 0.0968436, 0.083333, 0.0944457, 0.0}},
		{286518, {0.0, 0.0, 0.0817181, 0.0, 0.0, 0.096841, 0.0833295, 0.0944558, 0.0}},
		{286519, {0.0, 0.0, 0.08175, 0.0, 0.0, 0.096839, 0.0833641, 0.0944578, 0.0}},
		{286520, {0.0, 0.0, 0.081889, 0.0, 0.0, 0.096848, 0.0834166, 0.0945476, 0.0}}
	};

	unordered_map<int, double> const gain_ = {
		{286178,  0.86460},
		{286200,  1.0299 },
		{286201,  1.0028 },
		{286288,  1.0394 },
		{286301,  0.9483 },
		{286302,  0.88582},
		{286309,  0.84648},
		{286314,  0.83864},
		{286327,  0.95121},
		{286329,  0.99051},
		{286365,  0.93698},
		{286420,  1.01743},
		{286422,  1.01092},
		{286425,  0.93148},
		{286441,  0.97273},
		{286442,  0.92922},
		{286450,  0.91443},
		{286471,  0.95149},
		{286496,  0.93422},
	};
};


QWZDCProducer::QWZDCProducer(const edm::ParameterSet& pset) :
	Src_(pset.getUntrackedParameter<edm::InputTag>("Src"))
{
	consumes<ZDCDigiCollection>(Src_);

	produces<std::vector<double> >("ADC");
	produces<std::vector<double> >("nominalfC");
	produces<std::vector<double> >("regularfC");
}

QWZDCProducer::~QWZDCProducer()
{
	return;
}

void QWZDCProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	std::auto_ptr<std::vector<double> > pADC( new std::vector<double> );
	std::auto_ptr<std::vector<double> > pfC( new std::vector<double> );
	std::auto_ptr<std::vector<double> > pregfC( new std::vector<double> );


	Handle<ZDCDigiCollection> digi;
	iEvent.getByLabel(Src_, digi);

	edm::ESHandle<HcalDbService> conditions;
	iSetup.get<HcalDbRecord>().get(conditions);

	for ( auto i = digi->begin(); i < digi->end(); i++ ) {
		const ZDCDataFrame & rh = (*i);
		HcalZDCDetId zdcid = rh.id();

		const HcalQIECoder* qiecoder = conditions->getHcalCoder(zdcid);
		const HcalQIEShape* qieshape = conditions->getHcalShape(qiecoder);
		HcalCoderDb coder(*qiecoder, *qieshape);
		CaloSamples caldigi;
		coder.adc2fC(rh, caldigi);

		for ( int j = 0; j < 10; j++ ) {
			// hardcode 10 TS
			pADC->push_back((*i)[j].adc());
			pfC->push_back((*i)[j].nominal_fC());
			pregfC->push_back(caldigi[j]);
		}
	// order-> -Side, EM1-5, HAD1-4, +side, EM1-5, HAD1-4
	//	std::cout << "i->id().zside() = " << i->id().zside() << " i->id().section() = " << i->id().section() << " i->id().channel() = " << i->id().channel() << std::endl;
	}

	iEvent.put(pADC, std::string("ADC"));
	iEvent.put(pfC, std::string("nominalfC"));
	iEvent.put(pregfC, std::string("regularfC"));
}


DEFINE_FWK_MODULE(QWZDCProducer);
