import FWCore.ParameterSet.Config as cms

process = cms.Process("CumuV3")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = 100

#from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/q/qwang/work/cleanroomRun2/Ana/data/pPb_8_FEVT.root")
)

import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltMB = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltMB.HLTPaths = [
	"HLT_PAL1MinimumBiasHF_OR_SinglePixelTrack_part*",
]
process.hltMB.andOr = cms.bool(True)
process.hltMB.throw = cms.bool(False)


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('cumu.root')
)


process.load("HeavyIonsAnalysis.VertexAnalysis.PAPileUpVertexFilter_cff")

process.PAprimaryVertexFilter = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && abs(z) <= 25 && position.Rho <= 2 && tracksSize >= 2"),
    filter = cms.bool(True), # otherwise it won't filter the events
)

process.NoScraping = cms.EDFilter("FilterOutScraping",
 applyfilter = cms.untracked.bool(True),
 debugOn = cms.untracked.bool(False),
 numtrack = cms.untracked.uint32(10),
 thresh = cms.untracked.double(0.25)
)

process.load("HeavyIonsAnalysis.Configuration.hfCoincFilter_cff")
#process.load("HeavyIonsAnalysis.EventAnalysis.pileUpFilter_cff")

process.hfPosOnNegOff = cms.Sequence(
        process.towersAboveThreshold *
        process.hfPosTowers *
        process.hfNegTowers *
        process.hfPosFilter *
        ~process.hfNegFilter
        )

process.QWZDC = cms.EDProducer("QWZDCProducer",
	Src = cms.untracked.InputTag("castorDigis"),
	)

process.QWZDCFilter0N = cms.EDFilter('QWDoubleFilter',
        src = cms.untracked.InputTag('QWZDC', 'SumN'),
        dmin = cms.untracked.double(-10.),
        dmax = cms.untracked.double(1.0)
        )

process.QWZDCFilter1N = cms.EDFilter('QWDoubleFilter',
        src = cms.untracked.InputTag('QWZDC', 'SumN'),
        dmin = cms.untracked.double(1.0),
        dmax = cms.untracked.double(4.0)
        )

process.selection0N = cms.Sequence(
        process.QWZDC,
        process.QWZDCFilter0N
        )

process.selection1N = cms.Sequence(
        process.QWZDC,
        process.QWZDCFilter1N
        )

#process.eventSelection = cms.Sequence(process.hfCoincFilter * process.PAprimaryVertexFilter * process.NoScraping * process.olvFilter_pPb8TeV_dz1p0)
process.eventSelection = cms.Sequence(process.process.hfPosOnNegOff * process.PAprimaryVertexFilter * process.NoScraping )


process.load('pPb_HM_eff')

process.ana0N = cms.Path(process.hltMB * process.eventSelection * process.selection0N * process.Noff * process.QWEvent * process.vectMonW)
process.ana1N = cms.Path(process.hltMB * process.eventSelection * process.selection1N * process.Noff * process.QWEvent * process.vectMonW)

process.schedule = cms.Schedule(
	process.ana0N,
	process.ana1N,
)
