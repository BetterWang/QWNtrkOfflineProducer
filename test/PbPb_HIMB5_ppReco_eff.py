import FWCore.ParameterSet.Config as cms

centralityBins = cms.EDProducer("QWPPRecoCentBinProducer")

Noff = cms.EDProducer("QWNtrkOfflineProducer",
                vertexSrc = cms.untracked.InputTag("offlinePrimaryVertices"),
                trackSrc  = cms.untracked.InputTag("generalTracks")
                )


QWEvent = cms.EDProducer("QWEventProducer"
		, vertexSrc = cms.untracked.InputTag('offlinePrimaryVertices', "")
		, trackSrc = cms.untracked.InputTag('generalTracks')
		, fweight = cms.untracked.InputTag('Hydjet_eff_mult_v1.root')
                , centralitySrc = cms.untracked.InputTag("Noff")
		, dzdzerror = cms.untracked.double(3.0)
		, d0d0error = cms.untracked.double(3.0)
		, pterrorpt = cms.untracked.double(0.1)
		, ptMin = cms.untracked.double(0.3)
		, ptMax= cms.untracked.double(3.0)
		, Etamin = cms.untracked.double(-2.4)
		, Etamax = cms.untracked.double(2.4)
                )

makeEvent = cms.Sequence(centralityBins*Noff*QWEvent)

# monitoring
histNoff = cms.EDAnalyzer('QWHistAnalyzer',
		src = cms.untracked.InputTag("Noff"),
		Nbins = cms.untracked.int32(5000),
		start = cms.untracked.double(0),
		end = cms.untracked.double(5000),
		)

histCentBin = cms.EDAnalyzer('QWHistAnalyzer',
		src = cms.untracked.InputTag("centralityBins"),
		Nbins = cms.untracked.int32(200),
		start = cms.untracked.double(0),
		end = cms.untracked.double(200),
		)
vectPhi = cms.EDAnalyzer('QWVectorAnalyzer',
		src = cms.untracked.InputTag("QWEvent", "phi"),
		hNbins = cms.untracked.int32(5000),
		hstart = cms.untracked.double(0),
		hend = cms.untracked.double(5000),
		cNbins = cms.untracked.int32(1000),
		cstart = cms.untracked.double(-5),
		cend = cms.untracked.double(5),
		)

vectPt = cms.EDAnalyzer('QWVectorAnalyzer',
		src = cms.untracked.InputTag("QWEvent", "pt"),
		hNbins = cms.untracked.int32(5000),
		hstart = cms.untracked.double(0),
		hend = cms.untracked.double(5000),
		cNbins = cms.untracked.int32(1000),
		cstart = cms.untracked.double(0),
		cend = cms.untracked.double(5),
		)

vectEta = cms.EDAnalyzer('QWVectorAnalyzer',
		src = cms.untracked.InputTag("QWEvent", "eta"),
		hNbins = cms.untracked.int32(5000),
		hstart = cms.untracked.double(0),
		hend = cms.untracked.double(5000),
		cNbins = cms.untracked.int32(1000),
		cstart = cms.untracked.double(-2.5),
		cend = cms.untracked.double(2.5),
		)

corr2D = cms.EDAnalyzer('QW2DAnalyzer',
		srcPhi = cms.untracked.InputTag("QWEvent", "phi"),
		srcEta = cms.untracked.InputTag("QWEvent", "eta"),
                hNbins = cms.untracked.int32(5000),
                hstart = cms.untracked.double(0),
                hend = cms.untracked.double(5000),
		)

vectEtaW = vectEta.clone( srcW = cms.untracked.InputTag("QWEvent", "weight") )
vectPtW = vectPt.clone( srcW = cms.untracked.InputTag("QWEvent", "weight") )
vectPhiW = vectPhi.clone( srcW = cms.untracked.InputTag("QWEvent", "weight") )

corr2DW = corr2D.clone( srcW = cms.untracked.InputTag("QWEvent", "weight") )

vectMon = cms.Sequence(histNoff*histCentBin*vectPhi*vectPt*vectEta)
vectMonW = cms.Sequence(histNoff*histCentBin*vectPhi*vectPt*vectEta*vectPhiW*vectPtW*vectEtaW)
