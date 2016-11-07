import FWCore.ParameterSet.Config as cms

QWGenEvent = cms.EDProducer("QWGenEventProducer"
		, trackSrc = cms.untracked.InputTag('genParticles')
                )

makeEvent = cms.Sequence(QWGenEvent)

# monitoring
vectPhi = cms.EDAnalyzer('QWVectorAnalyzer',
		src = cms.untracked.InputTag("QWGenEvent", "phi"),
		hNbins = cms.untracked.int32(5000),
		hstart = cms.untracked.double(0),
		hend = cms.untracked.double(5000),
		cNbins = cms.untracked.int32(1000),
		cstart = cms.untracked.double(-5),
		cend = cms.untracked.double(5),
		)

vectPt = cms.EDAnalyzer('QWVectorAnalyzer',
		src = cms.untracked.InputTag("QWGenEvent", "pt"),
		hNbins = cms.untracked.int32(5000),
		hstart = cms.untracked.double(0),
		hend = cms.untracked.double(5000),
		cNbins = cms.untracked.int32(1000),
		cstart = cms.untracked.double(0),
		cend = cms.untracked.double(5),
		)

vectEta = cms.EDAnalyzer('QWVectorAnalyzer',
		src = cms.untracked.InputTag("QWGenEvent", "eta"),
		hNbins = cms.untracked.int32(5000),
		hstart = cms.untracked.double(0),
		hend = cms.untracked.double(5000),
		cNbins = cms.untracked.int32(1000),
		cstart = cms.untracked.double(-2.5),
		cend = cms.untracked.double(2.5),
		)
vectEtaW = vectEta.clone( srcW = cms.untracked.InputTag("QWGenEvent", "weight") )
vectPtW = vectPt.clone( srcW = cms.untracked.InputTag("QWGenEvent", "weight") )
vectPhiW = vectPhi.clone( srcW = cms.untracked.InputTag("QWGenEvent", "weight") )

vectMon = cms.Sequence(vectPhi*vectPt*vectEta)
vectMonW = cms.Sequence(vectPhi*vectPt*vectEta*vectPhiW*vectPtW*vectEtaW)
