import FWCore.ParameterSet.Config as cms

QWZDC = cms.EDProducer("QWZDCProducer",
		Src = cms.untracked.InputTag("castorDigis"),
		)
