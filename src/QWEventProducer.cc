#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "TrackingTools/TransientTrack/interface/TransientTrack.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/VolumeBasedEngine/interface/VolumeBasedMagneticField.h"


#include "TFile.h"
#include "TH2.h"
#include "TMath.h"

using namespace std;
class QWEventProducer : public edm::EDProducer {
public:
	explicit QWEventProducer(const edm::ParameterSet&);
	~QWEventProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///
	bool TrackQuality_ppReco(const reco::TrackCollection::const_iterator&, const reco::VertexCollection&);
	bool TrackQuality_HIReco(const reco::TrackCollection::const_iterator&, const reco::VertexCollection&);
	bool TrackQuality_Pixel(const reco::TrackCollection::const_iterator&, const reco::VertexCollection&);

	edm::InputTag	vertexSrc_;
	edm::InputTag	trackSrc_;
	edm::InputTag	fweight_;
	edm::InputTag	centralitySrc_;

	enum    TrackCut {trackUndefine = 0, ppReco = 1, HIReco, Pixel};
	TrackCut sTrackQuality;
	double  dzdzerror_;
	double  dzdzerror_pix_;
	double  d0d0error_;
	double  pterrorpt_;

	double	pTmin_;
	double	pTmax_;
	double	Etamin_;
	double	Etamax_;
	double	chi2_;
	int	minPxLayers_;

	bool	bEff_;
	bool	bFlip_;

	TH2D*	hEff_cbin[2000];
};

QWEventProducer::QWEventProducer(const edm::ParameterSet& pset) :
	vertexSrc_(pset.getUntrackedParameter<edm::InputTag>("vertexSrc")),
	trackSrc_(pset.getUntrackedParameter<edm::InputTag>("trackSrc")),
	fweight_(pset.getUntrackedParameter<edm::InputTag>("fweight")),
	centralitySrc_(pset.getUntrackedParameter<edm::InputTag>("centralitySrc"))
{
	consumes<int>(centralitySrc_);
	consumes<reco::TrackCollection>(trackSrc_);
	consumes<reco::VertexCollection>(vertexSrc_);
	dzdzerror_ = pset.getUntrackedParameter<double>("dzdzerror", 3.);         // general nominal 3., tight 2., loose 5
	dzdzerror_pix_ = pset.getUntrackedParameter<double>("dzdzerror_pix", 8.); // pixel: nominal 8., tight 6., loose 10
	d0d0error_ = pset.getUntrackedParameter<double>("d0d0error", 3.);
	pterrorpt_ = pset.getUntrackedParameter<double>("pterrorpt", 0.1);

	pTmin_ = pset.getUntrackedParameter<double>("ptMin", 0.3);
	pTmax_ = pset.getUntrackedParameter<double>("ptMax", 3.0);
	Etamin_ = pset.getUntrackedParameter<double>("Etamin", -2.4);
	Etamax_ = pset.getUntrackedParameter<double>("Etamax", 2.4);
	minPxLayers_ = pset.getUntrackedParameter<int>("minPxLayers", -1);
	chi2_ = pset.getUntrackedParameter<double>("chi2", 12.); // pixel: nominal 12, tight 9, loose 18.,

	bFlip_ = pset.getUntrackedParameter<bool>("bFlip", false);

	bEff_ = true;

	if ( trackSrc_.label() == "hiGeneralTracks" ) {
		sTrackQuality = HIReco;
	} else if ( trackSrc_.label() == "generalTracks" ) {
		sTrackQuality = ppReco;
	} else if ( trackSrc_.label() == "hiGeneralAndPixelTracks" ) {
		sTrackQuality = Pixel;
	} else {
		sTrackQuality = trackUndefine;
	}
	string streff = fweight_.label();
	if ( streff == string("NA") ) {
		cout << "!!! eff NA" << endl;
		bEff_ = false;
	} else {
		TFile * fEffFak = new TFile(streff.c_str());
		cout << "!!! Using particle weight " << streff << endl;
		cout << "!!! Apply Eff correction" << endl;
		if ( streff == string("PbPb_MB_TT_5TeV_v2.root") or streff == string("PbPb_dijet_TT_5TeV_v2.root") ) {
			TH2D * h = (TH2D*) fEffFak->Get("rTotalEff3D_0_5");
			for ( int c = 0; c < 10; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("rTotalEff3D_5_10");
			for ( int c = 10; c < 20; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("rTotalEff3D_10_30");
			for ( int c = 20; c < 60; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("rTotalEff3D_30_50");
			for ( int c = 60; c < 100; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("rTotalEff3D_50_100");
			for ( int c = 100; c < 200; c++ ) {
				hEff_cbin[c] = h;
			}
		} else if ( streff == string("Hijing_8TeV_MB_eff_v2.root") or streff == string("EPOS_8TeV_MB_eff_v2.root") or streff == string("Hijing_8TeV_MB_eff_v3_loose.root") or streff == string("Hijing_8TeV_MB_eff_v3_tight.root")
				or streff == string("Hijing_8TeV_MB_tight2.root") or streff == string("Hijing_8TeV_dataBS.root") or streff == string("Hijing_8TeV_MB_tighter.root")
				or streff == string("Hijing_8TeV_MB_Wide2.root") or streff == string("Hijing_8TeV_MB_Narrow2.root")
				or streff == string("Hijing_8TeV_MB_tight4.root") or streff == string("Hijing_8TeV_MB_tighter_v3.root")
				or streff == string("Hijing_8TeV_MB_eff_v4_narrow.root") or streff == string("Hijing_8TeV_MB_eff_v4_wide.root") ) {
			TH2D * h = (TH2D*) fEffFak->Get("rTotalEff3D_0");
			for ( int c = 0; c < 2000; c++ ) {
				hEff_cbin[c] = h;
			}
		} else if ( streff == string("trkEff_pp_all_74X_origin.root") ) {
			// pp 13 TeV HM 2015 TOTEM
			TH2D * h = (TH2D*) fEffFak->Get("rTotalEff3D");
			for ( int c = 0; c < 2000; c++ ) {
				hEff_cbin[c] = h;
			}
		} else if ( streff == string("Hydjet_ppReco_v5_tight.root") or streff == string("Hydjet_ppReco_v5_loose.root") or streff == string("Hydjet_ppReco_std_v2.root")
				or streff == string("Hydjet_ppReco_tight4_v2.root")
				or streff == string("Hydjet_ppReco_tight3_v2.root") or streff == string("Hydjet_ppReco_tight2_v2.root") or streff == string("Hydjet_ppReco_tighter_v2.root") or streff == string("Hydjet_ppReco_npix0_v2.root")
				or streff == string("Hydjet_ppReco_Narrow2_v2.root") or streff == string("Hydjet_ppReco_Wide2_v2.root") or streff == string("Hydjet_ppReco_tight2dz_v2.root") or streff == string("Hydjet_ppReco_tight2dxy_v2.root")
				or streff == string("Hydjet_ppReco_v6_wide.root") or streff ==string("Hydjet_ppReco_v6_narrow.root") ) {
			// PbPb15 pp reco syst
			TH2D * h = (TH2D*) fEffFak->Get("rTotalEff3D_0");
			for ( int c = 0; c < 120; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("rTotalEff3D_1");
			for ( int c = 120; c < 260; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("rTotalEff3D_2");
			for ( int c = 260; c < 400; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("rTotalEff3D_3");
			for ( int c = 400; c < 800; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("rTotalEff3D_4");
			for ( int c = 800; c < 1200; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("rTotalEff3D_5");
			for ( int c = 1200; c < 2000; c++ ) {
				hEff_cbin[c] = h;
			}
		} else if ( streff == string("Hydjet_PbPb_eff_v1.root") ) {
			// PbPb15 pp reco new
			TH2D * h = (TH2D*) fEffFak->Get("eff_1");
			for ( int c = 0; c < 260; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("eff_2");
			for ( int c = 260; c < 400; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("eff_3");
			for ( int c = 400; c < 800; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("eff_4");
			for ( int c = 800; c < 1200; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("eff_5");
			for ( int c = 1200; c < 2000; c++ ) {
				hEff_cbin[c] = h;
			}
		} else if ( streff == string("Hydjet_eff_mult_v1.root") ) {
			// PbPb15 pp reco old
			TH2D * h = (TH2D*) fEffFak->Get("rTotalEff3D_0");
			for ( int c = 0; c < 120; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("rTotalEff3D_1");
			for ( int c = 120; c < 260; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("rTotalEff3D_2");
			for ( int c = 260; c < 400; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("rTotalEff3D_3");
			for ( int c = 400; c < 800; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("rTotalEff3D_4");
			for ( int c = 800; c < 2000; c++ ) {
				hEff_cbin[c] = h;
			}
		} else if ( streff == std::string("EffCorrectionsPixel_TT_pt_0_10_v2.root") ) {
			TH2D * h = (TH2D*) fEffFak->Get("Eff_0_5");
			for ( int c = 0; c < 10; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("Eff_5_10");
			for ( int c = 10; c < 20; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("Eff_10_30");
			for ( int c = 20; c < 60; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("Eff_30_50");
			for ( int c = 60; c < 100; c++ ) {
				hEff_cbin[c] = h;
			}
			h = (TH2D*) fEffFak->Get("Eff_50_100");
			for ( int c = 100; c < 200; c++ ) {
				hEff_cbin[c] = h;
			}
		}
		cout << "!!! eff histo done" << endl;
	}
	produces<std::vector<double> >("phi");
	produces<std::vector<double> >("eta");
	produces<std::vector<double> >("pt");
	produces<std::vector<double> >("weight");
	produces<std::vector<double> >("charge");
	produces<std::vector<double> >("ref");
	produces<std::vector<double> >("vz");
}

QWEventProducer::~QWEventProducer()
{
	return;
}

void QWEventProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;

	std::auto_ptr<std::vector<double> > pphi( new std::vector<double> );
	std::auto_ptr<std::vector<double> > peta( new std::vector<double> );
	std::auto_ptr<std::vector<double> > ppT( new std::vector<double> );
	std::auto_ptr<std::vector<double> > pweight( new std::vector<double> );
	std::auto_ptr<std::vector<double> > pref( new std::vector<double> );
	std::auto_ptr<std::vector<double> > pcharge( new std::vector<double> );
	std::auto_ptr<std::vector<double> > pvz( new std::vector<double> );

	Handle<VertexCollection> vertexCollection;
	iEvent.getByLabel(vertexSrc_, vertexCollection);
	VertexCollection recoVertices = *vertexCollection;
	sort(recoVertices.begin(), recoVertices.end(), [](const reco::Vertex &a, const reco::Vertex &b){
			return a.tracksSize() > b.tracksSize();
			});
	for ( auto it = recoVertices.begin(); it != recoVertices.end(); ++it ) {
		pvz->push_back(it->z());
	}

	Handle<TrackCollection> tracks;
	iEvent.getByLabel(trackSrc_,tracks);

//	const MagneticField* theMagneticField = nullptr;
//	if (minPxLayers_>=0) {
//		edm::ESHandle<MagneticField> theMagneticFieldHandle;
//		iSetup.get<IdealMagneticFieldRecord>().get(theMagneticFieldHandle);
//		theMagneticField = theMagneticFieldHandle.product();
//	}

	edm::Handle<int> ch;
	iEvent.getByLabel(centralitySrc_,ch);
	int Cent = *(ch.product());
	for(TrackCollection::const_iterator itTrack = tracks->begin();
			itTrack != tracks->end();
			++itTrack) {
		if ( sTrackQuality == HIReco and not TrackQuality_HIReco(itTrack, recoVertices) ) continue;
		else if ( sTrackQuality == ppReco and not TrackQuality_ppReco(itTrack, recoVertices) ) continue;
		else if ( sTrackQuality == Pixel  and not TrackQuality_Pixel (itTrack, recoVertices) ) continue;

		if ( itTrack->eta() > Etamax_ or itTrack->eta() < Etamin_ or itTrack->pt() > pTmax_ or itTrack->pt() < pTmin_ ) continue;
		if (itTrack->hitPattern().pixelLayersWithMeasurement() < minPxLayers_) continue;

//		if (theMagneticField) {
//			reco::TransientTrack tTrack(*itTrack, theMagneticField);
//			if (tTrack.hitPattern().pixelLayersWithMeasurement() < minPxLayers_) continue;
//		}

		double eff = 0.;

		if ( bEff_ ) {
			eff = hEff_cbin[Cent]->GetBinContent( hEff_cbin[Cent]->FindBin( itTrack->eta(), itTrack->pt() ) );
		} else {
			eff = 1.;
		}

		double weight;
		if ( eff >= 0.1 and not TMath::IsNaN(eff) ) {
			weight = 1./eff;
		} else {
			weight = 0.;
		}

		pphi->push_back(itTrack->phi());
		ppT->push_back(itTrack->pt());
		pweight->push_back(weight);
		pcharge->push_back(itTrack->charge());
		pref->push_back(std::distance(tracks->begin(), itTrack));
		if ( bFlip_ ) {
			peta->push_back(-itTrack->eta());
		} else {
			peta->push_back(itTrack->eta());
		}
	}
	iEvent.put(pphi, std::string("phi"));
	iEvent.put(peta, std::string("eta"));
	iEvent.put(ppT, std::string("pt"));
	iEvent.put(pweight, std::string("weight"));
	iEvent.put(pcharge, std::string("charge"));
	iEvent.put(pref, std::string("ref"));
	iEvent.put(pvz, std::string("vz"));
}


///
bool
QWEventProducer::TrackQuality_ppReco(const reco::TrackCollection::const_iterator& itTrack, const reco::VertexCollection& recoVertices)
{
        if ( itTrack->charge() == 0 ) return false;
        if ( !itTrack->quality(reco::TrackBase::highPurity) ) return false;
        if ( itTrack->ptError()/itTrack->pt() > pterrorpt_ ) return false;
	int primaryvtx = 0;
	math::XYZPoint v1( recoVertices[primaryvtx].position().x(), recoVertices[primaryvtx].position().y(), recoVertices[primaryvtx].position().z() );
	double vxError = recoVertices[primaryvtx].xError();
	double vyError = recoVertices[primaryvtx].yError();
	double vzError = recoVertices[primaryvtx].zError();
        double d0 = -1.* itTrack->dxy(v1);
        double derror=sqrt(itTrack->dxyError()*itTrack->dxyError()+vxError*vyError);
        if ( fabs( d0/derror ) > d0d0error_ ) {
                return false;
        }
        double dz=itTrack->dz(v1);
        double dzerror=sqrt(itTrack->dzError()*itTrack->dzError()+vzError*vzError);
        if ( fabs( dz/dzerror ) > dzdzerror_ ) {
                return false;
        }
        return true;
}

///
bool
QWEventProducer::TrackQuality_HIReco(const reco::TrackCollection::const_iterator& itTrack, const reco::VertexCollection& recoVertices)
{
	if ( itTrack->charge() == 0 ) return false;
	if ( !itTrack->quality(reco::TrackBase::highPurity) ) return false;
	if ( itTrack->numberOfValidHits() < 11 ) return false;
	if ( itTrack->normalizedChi2() / itTrack->hitPattern().trackerLayersWithMeasurement() > 0.15 ) {
		return false;
	}
	if ( itTrack->ptError()/itTrack->pt() > pterrorpt_ ) {
		return false;
	}
	if (
		itTrack->originalAlgo() != 4 and
		itTrack->originalAlgo() != 5 and
		itTrack->originalAlgo() != 6 and
		itTrack->originalAlgo() != 7
	) {
		return false;
	}

	int primaryvtx = 0;
	math::XYZPoint v1( recoVertices[primaryvtx].position().x(), recoVertices[primaryvtx].position().y(), recoVertices[primaryvtx].position().z() );
	double vxError = recoVertices[primaryvtx].xError();
	double vyError = recoVertices[primaryvtx].yError();
	double vzError = recoVertices[primaryvtx].zError();
	double d0 = -1.* itTrack->dxy(v1);
	double derror=sqrt(itTrack->dxyError()*itTrack->dxyError()+vxError*vyError);
	if ( fabs( d0/derror ) > d0d0error_ ) {
		return false;
	}

	double dz=itTrack->dz(v1);
	double dzerror=sqrt(itTrack->dzError()*itTrack->dzError()+vzError*vzError);
	if ( fabs( dz/dzerror ) > dzdzerror_ ) {
		return false;
	}
	return true;
}

///
bool
QWEventProducer::TrackQuality_Pixel(const reco::TrackCollection::const_iterator& itTrack, const reco::VertexCollection& recoVertices)
{
	if ( itTrack->charge() == 0 ) return false;
	if ( !itTrack->quality(reco::TrackBase::highPurity) ) return false;
	bool bPix = false;
	int nHits = itTrack->numberOfValidHits();

	int primaryvtx = 0;
	math::XYZPoint v1( recoVertices[primaryvtx].position().x(), recoVertices[primaryvtx].position().y(), recoVertices[primaryvtx].position().z() );
	double vxError = recoVertices[primaryvtx].xError();
	double vyError = recoVertices[primaryvtx].yError();
	double vzError = recoVertices[primaryvtx].zError();
	double d0 = -1.* itTrack->dxy(v1);

	double dz=itTrack->dz(v1);
	double dzerror=sqrt(itTrack->dzError()*itTrack->dzError()+vzError*vzError);
//	std::cout << __LINE__ << "\tnHits = " << nHits << std::endl;
	if ( itTrack->pt() < 2.4 and (nHits==3 or nHits==4 or nHits==5 or nHits==6) ) bPix = true;
	if ( not bPix ) {
		if ( nHits < 11 ) return false;
		if ( itTrack->normalizedChi2() / itTrack->hitPattern().trackerLayersWithMeasurement() > 0.15 ) {
			return false;
		}
		if ( itTrack->ptError()/itTrack->pt() > pterrorpt_ ) {
			return false;
		}
		if (
			itTrack->pt() > 2.4 and
			itTrack->originalAlgo() != 4 and
			itTrack->originalAlgo() != 5 and
			itTrack->originalAlgo() != 6 and
			itTrack->originalAlgo() != 7
		) {
			return false;
		}

		double derror=sqrt(itTrack->dxyError()*itTrack->dxyError()+vxError*vyError);
		if ( fabs( d0/derror ) > d0d0error_ ) {
			return false;
		}

		if ( fabs( dz/dzerror ) > dzdzerror_ ) {
			return false;
		}
	} else {
		if ( itTrack->normalizedChi2() / itTrack->hitPattern().trackerLayersWithMeasurement() > chi2_ ) return false;
		if ( fabs( dz/dzerror ) > dzdzerror_pix_ ) {
			return false;
		}
	}
	return true;
}

DEFINE_FWK_MODULE(QWEventProducer);
