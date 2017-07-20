import FWCore.ParameterSet.Config as cms

process = cms.Process("CumuV3")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = 100

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_Prompt_v16', '')

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(
		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAMinimumBias1/AOD/PromptReco-v1/000/286/178/00000/0ACB2C21-BDB9-E611-9F95-02163E01231F.root",
		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAMinimumBias1/AOD/PromptReco-v1/000/286/178/00000/08AEE0E9-BEB9-E611-9813-02163E01354E.root",
		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAMinimumBias1/AOD/PromptReco-v1/000/286/178/00000/08E45934-BBB9-E611-8B47-02163E0146FA.root",
		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAMinimumBias1/AOD/PromptReco-v1/000/286/178/00000/0A3A6BEF-BEB9-E611-A6DF-02163E014626.root"
		)
)

import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltMB = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltMB.HLTPaths = [
	"HLT_PAL1MinimumBiasHF_OR_SinglePixelTrack_part*_v*",
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
        process.QWZDC *
        process.QWZDCFilter0N
        )

process.selection1N = cms.Sequence(
        process.QWZDC *
        process.QWZDCFilter1N
        )

#process.eventSelection = cms.Sequence(process.hfCoincFilter * process.PAprimaryVertexFilter * process.NoScraping * process.olvFilter_pPb8TeV_dz1p0)
process.eventSelection = cms.Sequence(process.hfPosOnNegOff * process.PAprimaryVertexFilter * process.NoScraping )


process.load('pPb_HM_eff')
process.QWEvent.fweight = cms.untracked.InputTag('NA')

process.histNoff0N = process.histNoff.clone()
process.histNoff1N = process.histNoff.clone()

process.vectPhi0N = process.vectPhi.clone()
process.vectPhi1N = process.vectPhi.clone()

process.vectEta0N = process.vectEta.clone()
process.vectEta1N = process.vectEta.clone()

process.vectPt0N = process.vectPt.clone()
process.vectPt1N = process.vectPt.clone()

process.monSumN0N = cms.EDAnalyzer('QWHistDAnalyzer',
		src = cms.untracked.InputTag('QWZDC', 'SumN'),
		Nbins = cms.untracked.int32(7000),
		start = cms.untracked.double(-100.),
		end = cms.untracked.double(600.),
		)

process.monSumN1N = process.monSumN0N.clone()


process.mon0N = cms.Sequence( process.histNoff0N + process.vectPhi0N + process.vectEta0N + process.vectPt0N + process.monSumN0N)
process.mon1N = cms.Sequence( process.histNoff1N + process.vectPhi1N + process.vectEta1N + process.vectPt1N + process.monSumN1N)

process.ana0N = cms.Path(process.hltMB * process.eventSelection * process.selection0N * process.Noff * process.QWEvent * process.mon0N)
process.ana1N = cms.Path(process.hltMB * process.eventSelection * process.selection1N * process.Noff * process.QWEvent * process.mon1N)

process.schedule = cms.Schedule(
	process.ana0N,
	process.ana1N,
)
