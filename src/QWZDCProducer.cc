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
#include <array>


using namespace std;
class QWZDCProducer : public edm::EDProducer {
public:
	explicit QWZDCProducer(const edm::ParameterSet&);
	~QWZDCProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	virtual void beginRun(const edm::Run&, const edm::EventSetup&) override;
	///
	void Fix2016PA(double input[][10]);

	edm::InputTag	Src_;
	array<double, 9> hilo_ratio_;
	array<double, 18> weights_;
};


QWZDCProducer::QWZDCProducer(const edm::ParameterSet& pset) :
	Src_(pset.getUntrackedParameter<edm::InputTag>("Src")),
	hilo_ratio_(array<double, 9>{}),
	weights_(array<double, 18>{})
{
	consumes<ZDCDigiCollection>(Src_);

	produces<std::vector<double> >("ADC");
	produces<std::vector<double> >("nominalfC");
	produces<std::vector<double> >("regularfC");

	produces< double >("Sum");
	produces< double >("SumP");
	produces< double >("SumN");

	produces< double >("emSumP");
	produces< double >("emSumN");

	produces< double >("hadSumP");
	produces< double >("hadSumN");
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

	auto rhprod = digi.product();
	if ( rhprod->size() == 0 ) {
		iEvent.put(pADC, std::string("ADC"));
		iEvent.put(pfC, std::string("nominalfC"));
		iEvent.put(pregfC, std::string("regularfC"));

		iEvent.put(auto_ptr<double>(new double(-999.)), std::string("Sum"));
		iEvent.put(auto_ptr<double>(new double(-999.)), std::string("SumP"));
		iEvent.put(auto_ptr<double>(new double(-999.)), std::string("SumN"));

		iEvent.put(auto_ptr<double>(new double(-999.)), std::string("emSumP"));
		iEvent.put(auto_ptr<double>(new double(-999.)), std::string("emSumN"));

		iEvent.put(auto_ptr<double>(new double(-999.)), std::string("hadSumP"));
		iEvent.put(auto_ptr<double>(new double(-999.)), std::string("hadSumN"));

		return;
	}

	double adc[18][10] = {};
	double nom_fC[18][10] = {};
	double reg_fC[18][10] = {};
	double signal[18] = {};
	// order-> -Side, EM1-5, HAD1-4, +side, EM1-5, HAD1-4
	for ( int i = 0; i < 18; i++ ) {

		auto rh = (*rhprod)[i];
		//const ZDCDataFrame & rh = (*digi)[i];
		HcalZDCDetId zdcid = rh.id();
		int zside   = zdcid.zside();
		int section = zdcid.section();
		int channel = zdcid.channel();
		if( (zside   != -1 && zside   != 1) 
			or (section !=  1 && section != 2) 
			or (section == 1 && (channel < 1 || channel > 5)) 
			or (section == 2 && (channel < 1 || channel > 4)) ) {

			iEvent.put(pADC, std::string("ADC"));
			iEvent.put(pfC, std::string("nominalfC"));
			iEvent.put(pregfC, std::string("regularfC"));

			iEvent.put(auto_ptr<double>(new double(-999.)), std::string("Sum"));
			iEvent.put(auto_ptr<double>(new double(-999.)), std::string("SumP"));
			iEvent.put(auto_ptr<double>(new double(-999.)), std::string("SumN"));

			iEvent.put(auto_ptr<double>(new double(-999.)), std::string("emSumP"));
			iEvent.put(auto_ptr<double>(new double(-999.)), std::string("emSumN"));

			iEvent.put(auto_ptr<double>(new double(-999.)), std::string("hadSumP"));
			iEvent.put(auto_ptr<double>(new double(-999.)), std::string("hadSumN"));

			return;
		}

		const HcalQIECoder* qiecoder = nullptr;
		const HcalQIEShape* qieshape = nullptr;
		try {
			qiecoder = conditions->getHcalCoder(zdcid);
		}
		catch (cms::Exception& e) {
			cout << __LINE__ << "\t" << e.what() << endl;
			cout << __LINE__ << "\t" << zdcid << endl;

			iEvent.put(pADC, std::string("ADC"));
			iEvent.put(pfC, std::string("nominalfC"));
			iEvent.put(pregfC, std::string("regularfC"));

			iEvent.put(auto_ptr<double>(new double(-999.)), std::string("Sum"));
			iEvent.put(auto_ptr<double>(new double(-999.)), std::string("SumP"));
			iEvent.put(auto_ptr<double>(new double(-999.)), std::string("SumN"));

			iEvent.put(auto_ptr<double>(new double(-999.)), std::string("emSumP"));
			iEvent.put(auto_ptr<double>(new double(-999.)), std::string("emSumN"));

			iEvent.put(auto_ptr<double>(new double(-999.)), std::string("hadSumP"));
			iEvent.put(auto_ptr<double>(new double(-999.)), std::string("hadSumN"));

			return;
		}
		qieshape = conditions->getHcalShape(qiecoder);
		HcalCoderDb coder(*qiecoder, *qieshape);
		CaloSamples caldigi;
		coder.adc2fC(rh, caldigi);

		for ( int j = 0; j < 10; j++ ) {
			// hardcode 10 TS
			adc[i][j] = rh[j].adc();
			nom_fC[i][j] = rh[j].nominal_fC();
			reg_fC[i][j] = caldigi[j];
		}
	}

	Fix2016PA(adc);
	Fix2016PA(nom_fC);
	Fix2016PA(reg_fC);

	for ( int i = 0; i < 18; i++ ) {
		for ( int j = 0; j < 10; j++ ) {
			pADC->push_back(adc[i][j]);
			pfC->push_back(nom_fC[i][j]);
			pregfC->push_back(reg_fC[i][j]);
		}
		double ped = 1.5 * ( reg_fC[i][2] + reg_fC[i][6]);
		if ( adc[i][3] > 126 ) {
			signal[i] = (reg_fC[i][4] + reg_fC[i][5] - 0.666 * ped) * hilo_ratio_[i%9];
		} else {
			signal[i] = reg_fC[i][3] + reg_fC[i][4] + reg_fC[i][5] - ped;
		}
//		cout << " weights_[" << i << "] = " << weights_[i] << "\tsignal[" << i << "] = " << signal[i] << "\tadc[" << i << "][3] = " << adc[i][3];
		signal[i] *= weights_[i];
//		cout << "\tsignal[i] = " << signal[i] << endl;
	}

//	for ( int i = 0; i < 9; i++ ) {
//		for ( int t = 0; t < 1; t++ ) {
//			cout << "rawadc[" << i << "][" << t << "] = " << adc[i][t] << "\t";
//		}
//		cout << endl;
//	}

	double emEn = signal[0] + signal[1] + signal[2] + signal[3] + signal[4];
	double hadEn= signal[5] + signal[6] + signal[7] + signal[8];
	double emEp = signal[9] + signal[10]+ signal[11]+ signal[12]+ signal[13];
	double hadEp= signal[14]+ signal[15]+ signal[16]+ signal[17];

	double sum = emEn + emEp + hadEn + hadEp;
	double sump = emEp + hadEp;
	double sumn = emEn + hadEn;

//	cout << " emEn = " << emEn << "\thadEn = " << hadEn << "\temEp = " << emEp << "\thadEp = " << hadEp << endl;

	iEvent.put(pADC, std::string("ADC"));
	iEvent.put(pfC, std::string("nominalfC"));
	iEvent.put(pregfC, std::string("regularfC"));

	iEvent.put(auto_ptr<double>(new double(sum)), std::string("Sum"));
	iEvent.put(auto_ptr<double>(new double(sump)), std::string("SumP"));
	iEvent.put(auto_ptr<double>(new double(sumn)), std::string("SumN"));

	iEvent.put(auto_ptr<double>(new double(emEp)), std::string("emSumP"));
	iEvent.put(auto_ptr<double>(new double(emEn)), std::string("emSumN"));

	iEvent.put(auto_ptr<double>(new double(hadEp)), std::string("hadSumP"));
	iEvent.put(auto_ptr<double>(new double(hadEn)), std::string("hadSumN"));
}


void QWZDCProducer::beginRun(edm::Run const &r, edm::EventSetup const& iSetup)
{
	unordered_map<int, array<double, 9> >  hilo = {
		{286178, {{9.856489512695157, 10.854145252342596, 13.565189553176221, 15.395486966950507, 13.154277310581694, 10.462177137213546, 10.187345279693565, 12.696381277408312, 10.512571984836665}}},
		{286200, {{10.04517314362689, 11.262973537643672, 13.986326966757296, 17.70237353424347, 13.643867763633635, 10.895323182523901, 10.609212828235783, 12.88553521359063, 10.654426861088648}}},
		{286201, {{10.03141840243643, 11.200754482821962, 13.897845278068088, 17.571820423024004, 13.492017447876963, 10.796839117379996, 10.534810173255488, 12.868273914078534, 10.634722795315616}}},
		{286288, {{10.03487117734126, 11.325002661375626, 13.94114883431084, 17.762651892877, 13.527516320948441, 10.906909527185471, 10.616500164555752, 12.898615591588554, 10.655391841379576}}},
		{286301, {{9.960357776051316, 11.035542170669075, 13.702196462092873, 16.156289481932422, 13.190001978500298, 10.637765984041224, 10.366378930542151, 12.756387442101946, 10.59595912502808}}},
		{286302, {{9.90206854211845, 10.866470635536402, 13.588987484542526, 15.009110530091766, 13.023970617922286, 10.518764423855716, 10.214410694896575, 12.69559145586695, 10.586413185589352}}},
		{286309, {{9.878396933745591, 10.776976928647791, 13.579262836137676, 14.082959900179981, 12.97676639744202, 10.462166191509953, 10.14627890294374, 12.679108252958352, 10.592524643508584}}},
		{286314, {{9.894132779261898, 10.769978040014777, 13.601722522140204, 14.050587736085001, 13.007876269080928, 10.468003500500371, 10.147349664477884, 12.69451165483115, 10.612646879032805}}},
		{286327, {{9.962937871119436, 11.012694332757368, 13.684196121898818, 16.053296945860257, 13.200692772356692, 10.632303063166512, 10.32301754190371, 12.754353698635029, 10.591784164859002}}},
		{286329, {{10.017299876887385, 11.173084486395652, 13.821833797977312, 17.291046350378846, 13.439469302236194, 10.789663071191276, 10.47073273140581, 12.818475011664813, 10.62104432480428}}},
		{286365, {{9.948763866089639, 11.013458446221282, 13.70401788100253, 16.133534034496723, 13.194161319732792, 10.627839625900046, 10.323923421265631, 12.756078271296271, 10.589529708396121}}},
		{286420, {{10.033572333026306, 11.219994928562292, 13.876958559238656, 17.42691587156363, 13.52896212567053, 10.839039184210556, 10.539673438758173, 12.864036142795946, 10.625468184691888}}},
		{286422, {{10.02816912707796, 11.198534335826126, 13.874821361674968, 17.366960399856897, 13.36987766561936, 10.823787654604276, 10.497541475786372, 12.86632021955089, 10.618100888947406}}},
		{286425, {{9.944212965264864, 10.99168039710743, 13.662844713572124, 15.785020646807007, 13.149347660862544, 10.615046403675354, 10.307185035616477, 12.73351678086509, 10.594062028233175}}},
		{286441, {{9.995302207962258, 11.095601925308845, 13.795120390015645, 16.641205622064906, 13.286264526869477, 10.734059385110141, 10.417393713936285, 12.805523790742374, 10.593545776300331}}},
		{286442, {{9.954904283595313, 11.033167909369146, 13.695965990177253, 16.24539646077793, 13.161133068268112, 10.637245464012603, 10.32369960098901, 12.769907967273278, 10.592098294672175}}},
		{286450, {{9.950743818100403, 11.007300041387449, 13.657022987501092, 15.95171098051977, 13.171135193800085, 10.618878880005607, 10.304349259735549, 12.754288629551686, 10.600339634881902}}},
		{286471, {{10.00545297186967, 11.128582986399758, 13.81230731831291, 17.265580891836315, 13.421521678441815, 10.722446261779947, 10.426724110965369, 12.827617347043489, 10.610012031753644}}},
		{286496, {{9.975460367495959, 11.072504977090988, 13.70649729091081, 16.396130513198884, 13.233044500082046, 10.653450705950911, 10.360278691496802, 12.780206016920994, 10.589271796957068}}}
	};

//		{286514, {0.0, 0.0, 12.201773405746792, 0.0, 0.0, 10.30993733620087, 11.988251513516753, 10.576190879092985, 0.0}},
//		{286515, {0.0, 0.0, 12.222309136420526, 0.0, 0.0, 10.311670232785955, 11.98447291688888, 10.576369851423156, 0.0}},
//		{286516, {0.0, 0.0, 12.217500836898807, 0.0, 0.0, 10.31660633171397, 11.991069051770243, 10.57783541522764, 0.0}},
//		{286517, {0.0, 0.0, 12.23304438882487, 0.0, 0.0, 10.325927578074339, 12.000048000192, 10.588094534743245, 0.0}},
//		{286518, {0.0, 0.0, 12.237191026223076, 0.0, 0.0, 10.326204809946201, 12.000552025393167, 10.586962367583567, 0.0}},
//		{286519, {0.0, 0.0, 12.232415902140673, 0.0, 0.0, 10.3264180753622, 11.995571235100002, 10.586738204785629, 0.0}},
//		{286520, {0.0, 0.0, 12.211652358680652, 0.0, 0.0, 10.325458450355196, 11.988021568848408, 10.576683067576544, 0.0}}

	if ( hilo.find( r.run() ) == hilo.end() ) {
		hilo_ratio_ = array<double, 9>{};

	} else {
		int runid = r.run();
		hilo_ratio_ = hilo[runid];
	}

	auto weights = array<double, 18>{{0.12, 0.12, 0.12, 0.12, 0.12, 1.00, 0.65, 0.27 , 0.23, /*neg*/ 0. , 0. , 0.40, 0. , 0. , 0.60, 1.12, 1.05 , 0. /*pos*/ }};
	unordered_map<int, double> gain = {
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

	if ( gain.find( r.run() ) == gain.end() ) {
		weights_ = array<double, 18>{};
	} else {
		for ( int i = 0; i < 18; i++ ) {
			weights_[i] = weights[i] / gain[r.run()] / 2000. * 2.5617;
		}
	}
	return;
}


void QWZDCProducer::Fix2016PA(double input[][10])
{
	double t[18][10];
	for ( int i = 0; i < 18; i++ ) {
		for ( int j = 0; j < 10; j++ ) {
			t[i][j] = input[i][j];
		}
	}

	int fix[18] = {
		0,	// NZDC_EM1
		1,	// NZDC_EM2
		2,	// NZDC_EM3
		5,	// NZDC_EM4
		3,	// NZDC_EM5
		4,	// NZDC_HAD1
		6,	// NZDC_HAD2
		7,	// NZDC_HAD3
		8,	// NZDC_HAD4
		9,	// PZDC_EM1
		10,	// PZDC_EM2
		11,	// PZDC_EM3
		13,	// PZDC_EM4
		14,	// PZDC_EM5
		12,	// PZDC_HAD1
		15,	// PZDC_HAD2
		16,	// PZDC_HAD3
		17,	// PZDC_HAD4
	};

	for ( int i = 0; i < 18; i++ ) {
		for ( int j = 0; j < 10; j++ ) {
			input[fix[i]][j] = t[i][j];
		}
	}
}

DEFINE_FWK_MODULE(QWZDCProducer);
