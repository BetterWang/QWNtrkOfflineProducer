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
#		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAMinimumBias1/AOD/PromptReco-v1/000/286/178/00000/0ACB2C21-BDB9-E611-9F95-02163E01231F.root",
#		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAMinimumBias1/AOD/PromptReco-v1/000/286/178/00000/08AEE0E9-BEB9-E611-9813-02163E01354E.root",
#		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAMinimumBias1/AOD/PromptReco-v1/000/286/178/00000/08E45934-BBB9-E611-8B47-02163E0146FA.root",
#		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAMinimumBias1/AOD/PromptReco-v1/000/286/178/00000/0A3A6BEF-BEB9-E611-A6DF-02163E014626.root"
		'file:/eos/cms/store/hidata/PARun2016C/PAMinimumBias14/AOD/PromptReco-v1/000/286/178/00000/DAB00669-0DBA-E611-8411-02163E011A40.root',
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
    fileName = cms.string('zdc.root')
)


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


process.eventSelection = cms.Sequence(process.PAprimaryVertexFilter * process.NoScraping )


process.monSumN0N = cms.EDAnalyzer('QWHistDAnalyzer',
		src = cms.untracked.InputTag('QWZDC', 'SumN'),
		Nbins = cms.untracked.int32(7000),
		start = cms.untracked.double(-100.),
		end = cms.untracked.double(600.),
		)

process.monSumN = process.monSumN0N.clone()

process.ana0N = cms.Path( process.hltMB * process.eventSelection * process.QWZDC * process.QWZDCFilter0N * process.monSumN0N)
process.ana   = cms.Path( process.hltMB * process.eventSelection * process.QWZDC * process.monSumN)


process.schedule = cms.Schedule(
	process.ana,
	process.ana0N,
)
