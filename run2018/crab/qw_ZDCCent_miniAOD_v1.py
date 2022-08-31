import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Era_Run2_2018_pp_on_AA_cff import Run2_2018_pp_on_AA
from Configuration.ProcessModifiers.run2_miniAOD_pp_on_AA_103X_cff import run2_miniAOD_pp_on_AA_103X
from Configuration.AlCa.GlobalTag import GlobalTag
import os
import sys
import FWCore.ParameterSet.VarParsing as VarParsing


GT        = "112X_dataRun2_PromptLike_HI_v6"


#-----------------------------------
# Standard CMSSW Imports/Definitions
#-----------------------------------
process = cms.Process('MyTree',Run2_2018_pp_on_AA,run2_miniAOD_pp_on_AA_103X)
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = GT
process.GlobalTag = GlobalTag(process.GlobalTag, GT, '')
#process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")

process.dbCentBin = cms.EDProducer('QWInt2Double',
		src = cms.untracked.InputTag('centralityBin', 'HFtowers')
	)

#-----------------------------------
# Input source
#-----------------------------------
process.source = cms.Source('PoolSource',
        fileNames = cms.untracked.vstring('file:/eos/cms/store/group/phys_heavyions/qwang/data/PbPb2018_MB_MiniAOD.root')
    )

#-----------
# Log output
#-----------
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = ''
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )


#-----------------
# Files to process
#-----------------
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('293765:264-293765:9999')

#-----------------------------------------
# HLT selection -- MB
#-----------------------------------------

import HLTrigger.HLTfilters.hltHighLevel_cfi

process.hltSelect = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltSelect.HLTPaths = [
    "HLT_HIMinimumBias_*",
]
process.hltSelect.andOr = cms.bool(True)
process.hltSelect.throw = cms.bool(False)

#-----------------------------------------
# CMSSW Event Selection
#-----------------------------------------
process.load('HeavyIonsAnalysis.EventAnalysis.hffilter_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.collisionEventSelection_cff')
process.load('HeavyIonsAnalysis.EventAnalysis.hltanalysis_cfi')

process.eventSelection = cms.Sequence(
    process.primaryVertexFilter
    * process.phfCoincFilter2Th4
    * process.clusterCompatibilityFilter
)

#-----------------------------------------
# CMSSW/Hcal Related Module import
#-----------------------------------------
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')


# event info
process.evtInfo = cms.EDProducer('QWEventInfoProducer')

# ZDC rechit
process.load('HeavyIonsAnalysis.ZDCAnalysis.QWZDC2018Producer_cfi')
process.load('HeavyIonsAnalysis.ZDCAnalysis.QWZDC2018RecHit_cfi')

process.newCent = cms.EDProducer("CentralityProducer",
    produceHFhits = cms.bool(False),
    produceHFtowers = cms.bool(False),
    produceEcalhits = cms.bool(False),
    produceZDChits = cms.bool(True),
    produceETmidRapidity = cms.bool(False),
    producePixelhits = cms.bool(False),
    produceTracks = cms.bool(False),
    producePixelTracks = cms.bool(False),
    reUseCentrality = cms.bool(True),
    srcZDChits = cms.InputTag("QWzdcreco"),
    lowGainZDC = cms.bool(True),

    doPixelCut = cms.bool(True),
    useQuality = cms.bool(True),
    trackQuality = cms.string('highPurity'),
    trackEtaCut = cms.double(2),
    trackPtCut = cms.double(1),
    hfEtaCut = cms.double(4), #hf above the absolute value of this cut is used
    midRapidityRange = cms.double(1),

    srcReUse = cms.InputTag("hiCentrality")
    )

process.QWcent = cms.EDProducer('QWCentrality2018Producer',
		Src = cms.untracked.InputTag('newCent')
		)

process.QWcc = cms.EDProducer('QWClusterCompatibilityProducer',
		src = cms.untracked.InputTag('hiClusterCompatibility'),
        minZ =cms.untracked.double(-20),
        maxZ =cms.untracked.double(20),
		)

process.evtTree = cms.EDAnalyzer('QWTreeMaker',
        Vtags = cms.untracked.VPSet(),
        Dtags = cms.untracked.VPSet(
            cms.PSet(
                tag = cms.untracked.InputTag('QWcc', 'nPxlHits'),
                name = cms.untracked.string('nPxlHits')
                ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcc', 'clusVtxQual'),
                name = cms.untracked.string('clusVtxQual')
                ),
            cms.PSet(
                tag = cms.untracked.InputTag('evtInfo', 'RunId'),
                name = cms.untracked.string('Run')
                ),
            cms.PSet(
                tag = cms.untracked.InputTag('evtInfo', 'Lumi'),
                name = cms.untracked.string('Lumi')
                ),
            cms.PSet(
                tag = cms.untracked.InputTag('evtInfo', 'BX'),
                name = cms.untracked.string('BX')
                ),
            cms.PSet(
                tag = cms.untracked.InputTag('evtInfo', 'Orbit'),
                name = cms.untracked.string('Orbit')
                ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFhitSum'),
                name = cms.untracked.string('etHFhitSum')
                ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFhitSumPlus'),
                name = cms.untracked.string('etHFhitSumPlus')
                ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFhitSumMinus'),
                name = cms.untracked.string('etHFhitSumMinus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFtowerSum'),
                name = cms.untracked.string('etHFtowerSum')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFtowerSumPlus'),
                name = cms.untracked.string('etHFtowerSumPlus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFtowerSumMinus'),
                name = cms.untracked.string('etHFtowerSumMinus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFtruncated'),
                name = cms.untracked.string('etHFtruncated')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFtruncatedPlus'),
                name = cms.untracked.string('etHFtruncatedPlus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFtruncatedMinus'),
                name = cms.untracked.string('etHFtruncatedMinus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEESum'),
                name = cms.untracked.string('etEESum')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEESumPlus'),
                name = cms.untracked.string('etEESumPlus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEESumMinus'),
                name = cms.untracked.string('etEESumMinus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEEtruncated'),
                name = cms.untracked.string('etEEtruncated')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEEtruncatedPlus'),
                name = cms.untracked.string('etEEtruncatedPlus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEEtruncatedMinus'),
                name = cms.untracked.string('etEEtruncatedMinus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEBSum'),
                name = cms.untracked.string('etEBSum')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEcalSum'),
                name = cms.untracked.string('etEcalSum')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEBtruncated'),
                name = cms.untracked.string('etEBtruncated')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEcalTrancated'),
                name = cms.untracked.string('etEcalTrancated')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'zdcSum'),
                name = cms.untracked.string('zdcSum')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'zdcSumPlus'),
                name = cms.untracked.string('zdcSumPlus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'zdcSumMinus'),
                name = cms.untracked.string('zdcSumMinus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'pixelMultiplicity'),
                name = cms.untracked.string('pixelMultiplicity')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'trackMultiplicity'),
                name = cms.untracked.string('trackMultiplicity')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'ntracksPtCut'),
                name = cms.untracked.string('ntracksPtCut')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'ntracksEtaCut'),
                name = cms.untracked.string('ntracksEtaCut')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'ntracksEtaPtCut'),
                name = cms.untracked.string('ntracksEtaPtCut')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'nPixelTracks'),
                name = cms.untracked.string('nPixelTracks')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etMidRapiditySum'),
                name = cms.untracked.string('etMidRapiditySum')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('dbCentBin'),
                name = cms.untracked.string('CentBin')
            )
        )
    )

process.zdcana = cms.EDAnalyzer('QWZDC2018Analyzer',
		srcADC = cms.untracked.InputTag('zdcdigi', 'ADC'),
		srcfC = cms.untracked.InputTag('zdcdigi', 'regularfC'),
		srcDetId = cms.untracked.InputTag('zdcdigi', 'DetId'),
		srcCapId = cms.untracked.InputTag('zdcdigi', 'CapId'),
		srcHigh = cms.untracked.InputTag('zdcdigi', 'chargeHigh'),
		srcLow = cms.untracked.InputTag('zdcdigi', 'chargeLow'),
		srcSum = cms.untracked.InputTag('zdcdigi', 'chargeSum'),
		Norm = cms.untracked.bool(False),
		bTree = cms.untracked.bool(True)
		)

process.digiPath = cms.Path(
    process.hltSelect *
    process.eventSelection *
    process.zdcdigi *
    process.zdcana *
    process.QWzdcreco*
    process.newCent*
    process.QWcent*
    process.QWcc*
    process.evtInfo*
    process.dbCentBin*
#    process.hltanalysis*
    process.evtTree
)

process.TFileService = cms.Service("TFileService",
		fileName = cms.string('zdcTree.root')
		)

#process.outpath = cms.EndPath(process.output)
