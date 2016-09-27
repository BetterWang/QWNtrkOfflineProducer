import FWCore.ParameterSet.Config as cms

QWMult = cms.EDProducer("QWMultProducer",
		vertexSrc = cms.InputTag("offlinePrimaryVertices"),
		trackSrc  = cms.InputTag("generalTracks"),
		centralitySrc = cms.InputTag("centralityBin"),
		)

