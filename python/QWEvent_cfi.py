import FWCore.ParameterSet.Config as cms

QWEvent = cms.EDProducer("QWEventProducer",
		vertexSrc = cms.InputTag("offlinePrimaryVertices"),
		trackSrc  = cms.InputTag("generalTracks"),
		centralitySrc = cms.InputTag("centralityBin"),
		fweight = cms.InputTag("NA"),
		)
