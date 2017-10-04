import FWCore.ParameterSet.Config as cms

process = cms.Process("ZDC")

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
    fileName = cms.string('upc.root')
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

#process.load("HeavyIonsAnalysis.Configuration.hfCoincFilter_cff")
#process.load("HeavyIonsAnalysis.EventAnalysis.pileUpFilter_cff")

# make calotowers into candidates
process.towersAboveThreshold3 = cms.EDProducer("CaloTowerCandidateCreator",
    src = cms.InputTag("towerMaker"),
    verbose = cms.untracked.int32(0),
    minimumE = cms.double(3.0),
    minimumEt = cms.double(0.0),
)

process.towersAboveThreshold8 = process.towersAboveThreshold3.clone(
    minimumE = cms.double(8.0),
)

# select HF+ towers above threshold 3
process.hfPosTowers3 = cms.EDFilter("EtaPtMinCandSelector",
    src = cms.InputTag("towersAboveThreshold3"),
    ptMin   = cms.double(0),
    etaMin = cms.double(3.0),
    etaMax = cms.double(5.0)
)

# select HF+ towers above threshold 8
process.hfPosTowers8 = process.hfPosTowers3.clone(
    src = cms.InputTag("towersAboveThreshold8")
)

# select HF- towers above threshold 3
process.hfNegTowers3 = cms.EDFilter("EtaPtMinCandSelector",
    src = cms.InputTag("towersAboveThreshold3"),
    ptMin   = cms.double(0),
    etaMin = cms.double(-5.0),
    etaMax = cms.double(-3.0)
)

# select HF- towers above threshold 8
process.hfNegTowers8 = process.hfNegTowers3.clone(
    src = cms.InputTag("towersAboveThreshold8")
)

# require at least one HF+ tower above threshold 3
process.hfPosFilter3 = cms.EDFilter("CandCountFilter",
    src = cms.InputTag("hfPosTowers3"),
    minNumber = cms.uint32(1)
)

# require at least one HF+ tower above threshold 8
process.hfPosFilter8 = process.hfPosFilter3.clone(
    src = cms.InputTag("hfPosTowers8")
)

# require at least one HF- tower above threshold 3
process.hfNegFilter3 = cms.EDFilter("CandCountFilter",
    src = cms.InputTag("hfNegTowers3"),
    minNumber = cms.uint32(1)
)

# require at least one HF- tower above threshold 8
process.hfNegFilter8 = process.hfNegFilter3.clone(
    src = cms.InputTag("hfNegTowers8")
)

process.hfPosOnNegOff = cms.Sequence(
        process.towersAboveThreshold3 *
        process.towersAboveThreshold8 *
        process.hfPosTowers3 *
        process.hfNegTowers8 *
        process.hfPosFilter3 *
        ~process.hfNegFilter8
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

# calo
process.QWCalo = cms.EDProducer("QWCaloQProducer",
	caloSrc = cms.untracked.InputTag("towerMaker"),
	etaMin = cms.untracked.double(-5.),
	etaMax = cms.untracked.double(5.),
	N = cms.untracked.int32(2)
)

# calo eta gap
process.QWCaloEtaGap3 = cms.EDProducer("QWEtaGapProducer",
	srcE = cms.untracked.InputTag("QWCalo", "CaloTowerE"),
	srcEta = cms.untracked.InputTag("QWCalo", "CaloTowerEta"),
	minE = cms.untracked.double(3.0)
)

process.QWCaloEtaGap4 = process.QWCaloEtaGap3.clone(
	minE = cms.untracked.double(4.0)
)

process.QWCaloEtaGap5 = process.QWCaloEtaGap3.clone(
	minE = cms.untracked.double(5.0)
)

process.QWCaloEtaGap6 = process.QWCaloEtaGap3.clone(
	minE = cms.untracked.double(6.0)
)

process.QWCaloEtaGap7 = process.QWCaloEtaGap3.clone(
	minE = cms.untracked.double(7.0)
)

process.QWCaloEtaGap8 = process.QWCaloEtaGap3.clone(
	minE = cms.untracked.double(8.0)
)

# track eta gap
process.QWTrackEtaGap = cms.EDProducer("QWEtaGapProducer",
	srcE = cms.untracked.InputTag("QWEvent", "pt"),
	srcEta = cms.untracked.InputTag("QWEvent", "eta"),
	minE = cms.untracked.double(0.3)
)

process.eventSelection = cms.Sequence(process.hfPosOnNegOff * process.PAprimaryVertexFilter * process.NoScraping )
#process.eventSelection = cms.Sequence(process.PAprimaryVertexFilter * process.NoScraping )


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

process.monEtaGapPos3 = cms.EDAnalyzer('QWHistDAnalyzer',
		src = cms.untracked.InputTag('QWCaloEtaGap3', 'posEta'),
		Nbins = cms.untracked.int32(120),
		start = cms.untracked.double(-6.),
		end = cms.untracked.double(6.),
		)

process.monEtaGapPos4 = process.monEtaGapPos3.clone( src = cms.untracked.InputTag('QWCaloEtaGap4', 'posEta') )
process.monEtaGapPos5 = process.monEtaGapPos3.clone( src = cms.untracked.InputTag('QWCaloEtaGap5', 'posEta') )
process.monEtaGapPos6 = process.monEtaGapPos3.clone( src = cms.untracked.InputTag('QWCaloEtaGap6', 'posEta') )
process.monEtaGapPos7 = process.monEtaGapPos3.clone( src = cms.untracked.InputTag('QWCaloEtaGap7', 'posEta') )
process.monEtaGapPos8 = process.monEtaGapPos3.clone( src = cms.untracked.InputTag('QWCaloEtaGap8', 'posEta') )

process.monEtaGapNeg3 = process.monEtaGapPos3.clone( src = cms.untracked.InputTag('QWCaloEtaGap3', 'negEta') )
process.monEtaGapNeg4 = process.monEtaGapPos3.clone( src = cms.untracked.InputTag('QWCaloEtaGap4', 'negEta') )
process.monEtaGapNeg5 = process.monEtaGapPos3.clone( src = cms.untracked.InputTag('QWCaloEtaGap5', 'negEta') )
process.monEtaGapNeg6 = process.monEtaGapPos3.clone( src = cms.untracked.InputTag('QWCaloEtaGap6', 'negEta') )
process.monEtaGapNeg7 = process.monEtaGapPos3.clone( src = cms.untracked.InputTag('QWCaloEtaGap7', 'negEta') )
process.monEtaGapNeg8 = process.monEtaGapPos3.clone( src = cms.untracked.InputTag('QWCaloEtaGap8', 'negEta') )

process.monTrackEtaGapPos = process.monEtaGapPos3.clone( src = cms.untracked.InputTag('QWTrackEtaGap', 'posEta') )
process.monTrackEtaGapNeg = process.monEtaGapPos3.clone( src = cms.untracked.InputTag('QWTrackEtaGap', 'negEta') )

process.monCaloGap = cms.Sequence( process.monEtaGapPos3 + process.monEtaGapPos4 + process.monEtaGapPos5 + process.monEtaGapPos6 + process.monEtaGapPos7 + process.monEtaGapPos8 + process.monEtaGapNeg3 + process.monEtaGapNeg4 + process.monEtaGapNeg5 + process.monEtaGapNeg6 + process.monEtaGapNeg7 + process.monEtaGapNeg8 + process.monTrackEtaGapPos + process.monTrackEtaGapNeg)



process.monTrackEtaPhi = cms.EDAnalyzer('QWEventAccAnalyzer',
	srcPhi = cms.untracked.InputTag('QWEvent', 'phi'),
	srcEta = cms.untracked.InputTag('QWEvent', 'eta'),
	srcPt = cms.untracked.InputTag('QWEvent', 'pt'),
	srcWeight = cms.untracked.InputTag('QWEvent', 'weight'),
)

process.monCaloEtaPhi3 = cms.EDAnalyzer('QWEventAccAnalyzer',
	srcPhi = cms.untracked.InputTag('QWCalo', 'CaloTowerPhi'),
	srcEta = cms.untracked.InputTag('QWCalo', 'CaloTowerEta'),
	srcPt = cms.untracked.InputTag('QWCalo', 'CaloTowerE'),
	srcWeight = cms.untracked.InputTag('QWCalo', 'CaloTowerE'),
	hstartEta = cms.untracked.double(-6.0),
	hendEta = cms.untracked.double(6.0),
	NbinsEta = cms.untracked.int32(60),
	minPt = cms.untracked.double(3.)
)

process.monCaloEtaPhi4 = process.monCaloEtaPhi3.clone( minPt = cms.untracked.double(4.) )
process.monCaloEtaPhi5 = process.monCaloEtaPhi3.clone( minPt = cms.untracked.double(5.) )
process.monCaloEtaPhi6 = process.monCaloEtaPhi3.clone( minPt = cms.untracked.double(6.) )
process.monCaloEtaPhi7 = process.monCaloEtaPhi3.clone( minPt = cms.untracked.double(7.) )
process.monCaloEtaPhi8 = process.monCaloEtaPhi3.clone( minPt = cms.untracked.double(8.) )


process.monEtaPhi = cms.Sequence( process.monTrackEtaPhi + process.monCaloEtaPhi3 + process.monCaloEtaPhi4 + process.monCaloEtaPhi5 + process.monCaloEtaPhi6 + process.monCaloEtaPhi7 + process.monCaloEtaPhi8 )





process.monCaloCorrNeg3Pos3 = cms.EDAnalyzer('QWCorrAnalyzer',
	srcX = cms.untracked.InputTag('QWCaloEtaGap3', 'negEta'),
	srcY = cms.untracked.InputTag('QWCaloEtaGap3', 'posEta'),
	hstartX = cms.untracked.double(-6.0),
	hendX = cms.untracked.double(6.0),
	hstartY = cms.untracked.double(-6.0),
	hendY = cms.untracked.double(6.0),
	NbinsX = cms.untracked.int32(60),
	NbinsY = cms.untracked.int32(60)
)

process.monCaloCorrNeg4Pos4 = process.monCaloCorrNeg3Pos3.clone(
	srcX = cms.untracked.InputTag('QWCaloEtaGap4', 'negEta'),
	srcY = cms.untracked.InputTag('QWCaloEtaGap4', 'posEta')
)

process.monCaloCorrNeg5Pos5 = process.monCaloCorrNeg3Pos3.clone(
	srcX = cms.untracked.InputTag('QWCaloEtaGap5', 'negEta'),
	srcY = cms.untracked.InputTag('QWCaloEtaGap5', 'posEta')
)

process.monCaloCorrNeg6Pos6 = process.monCaloCorrNeg3Pos3.clone(
	srcX = cms.untracked.InputTag('QWCaloEtaGap6', 'negEta'),
	srcY = cms.untracked.InputTag('QWCaloEtaGap6', 'posEta')
)

process.monCaloCorrNeg7Pos7 = process.monCaloCorrNeg3Pos3.clone(
	srcX = cms.untracked.InputTag('QWCaloEtaGap7', 'negEta'),
	srcY = cms.untracked.InputTag('QWCaloEtaGap7', 'posEta')
)

process.monCaloCorrNeg8Pos8 = process.monCaloCorrNeg3Pos3.clone(
	srcX = cms.untracked.InputTag('QWCaloEtaGap8', 'negEta'),
	srcY = cms.untracked.InputTag('QWCaloEtaGap8', 'posEta')
)


process.monTrackCorrNegPos = process.monCaloCorrNeg3Pos3.clone(
	srcX = cms.untracked.InputTag('QWTrackEtaGap', 'negEta'),
	srcY = cms.untracked.InputTag('QWTrackEtaGap', 'posEta')
)

process.monNegPos = cms.Sequence( process.monCaloCorrNeg3Pos3 + process.monCaloCorrNeg4Pos4 + process.monCaloCorrNeg5Pos5 + process.monCaloCorrNeg6Pos6 + process.monCaloCorrNeg7Pos7 + process.monCaloCorrNeg8Pos8 + process.monTrackCorrNegPos )

process.monSumN1N = process.monSumN0N.clone()


process.mon0N = cms.Sequence( process.histNoff0N + process.vectPhi0N + process.vectEta0N + process.vectPt0N + process.monSumN0N + process.monCaloGap + process.monEtaPhi )
process.mon1N = cms.Sequence( process.histNoff1N + process.vectPhi1N + process.vectEta1N + process.vectPt1N + process.monSumN1N)


process.caloGap = cms.Sequence(process.QWTrackEtaGap * process.QWCaloEtaGap3 * process.QWCaloEtaGap4 * process.QWCaloEtaGap5 * process.QWCaloEtaGap6 * process.QWCaloEtaGap7 * process.QWCaloEtaGap8 )


process.ana0N = cms.Path( process.hltMB * process.eventSelection * process.QWZDC * process.makeEvent * process.QWCalo * process.caloGap * process.monNegPos * process.mon0N )

process.RECO = cms.OutputModule("PoolOutputModule",
                outputCommands = cms.untracked.vstring('keep *_QWZDC_SumN_*', 
			'keep *_QWCaloEtaGap3_*_*',
			'keep *_QWCaloEtaGap4_*_*',
			'keep *_QWCaloEtaGap5_*_*',
			'keep *_QWCaloEtaGap6_*_*',
			'keep *_QWCaloEtaGap7_*_*',
			'keep *_QWCaloEtaGap8_*_*',
			'keep *_QWTrackEtaGap_*_*',
			'keep *_QWCalo_*_*',
			'keep *_Noff_*_*'),
                SelectEvents = cms.untracked.PSet(
                        SelectEvents = cms.vstring('ana0N')
                        ),
                fileName = cms.untracked.string('reco.root')
                )

process.out = cms.EndPath(process.RECO)

process.schedule = cms.Schedule(
	process.ana0N,
	process.out,
)
