import FWCore.ParameterSet.Config as cms

QWGenEvent = cms.EDProducer("QWGenEventProducer",
		trackSrc  = cms.untracked.InputTag("genParticles")
		)
