import FWCore.ParameterSet.Config as cms

QWNoff = cms.EDProducer("QWNtrkOfflineProducer",
		vertexSrc = cms.InputTag("hiSelectedVertex"),
		trackSrc  = cms.InputTag("hiGeneralAndPixelTracks")
		)

