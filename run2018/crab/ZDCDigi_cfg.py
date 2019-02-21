import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
import os
import sys
import FWCore.ParameterSet.VarParsing as VarParsing


GT        = "103X_dataRun2_Prompt_v2"


#-----------------------------------
# Standard CMSSW Imports/Definitions
#-----------------------------------
process = cms.Process('MyTree',eras.Run2_2018_pp_on_AA)

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = GT
process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")
process.GlobalTag.toGet.extend([
    cms.PSet(record = cms.string("HeavyIonRcd"),
        tag = cms.string("CentralityTable_HFtowers200_DataPbPb_periHYDJETshape_run2v1031x02_offline"),
        connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS"),
        label = cms.untracked.string("HFtowers")
        ),
    ])

process.load("RecoHI.HiCentralityAlgos.CentralityBin_cfi")
process.centralityBin.Centrality = cms.InputTag("hiCentrality")
process.centralityBin.centralityVariable = cms.string("HFtowers")
process.dbCentBin = cms.EDProducer('QWInt2Double',
		src = cms.untracked.InputTag('centralityBin', 'HFtowers')
	)

#-----------------------------------
# Input source
#-----------------------------------
process.source = cms.Source('PoolSource',
        fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/q/qwang/work/cleanroomRun2/Ana/data/PbPb2018_MB.root')
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
process.load('HeavyIonsAnalysis.Configuration.hfCoincFilter_cff')
process.load('HeavyIonsAnalysis.Configuration.collisionEventSelection_cff')

process.eventSelection = cms.Sequence(
    process.primaryVertexFilter
    * process.hfCoincFilter2Th4
    * process.clusterCompatibilityFilter
)

#-----------------------------------------
# CMSSW/Hcal Related Module import
#-----------------------------------------
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')


# event info
process.HFQ2 = cms.EDProducer('QWCaloQProducer',
		caloSrc = cms.untracked.InputTag('towerMaker'),
		etaMin = cms.untracked.double(3.),
		etaMax = cms.untracked.double(5.),
		N = cms.untracked.int32(2)
	)

process.QWHFTree = cms.EDAnalyzer('QWDTagTreeMaker',
		vTag = cms.untracked.VInputTag(
			cms.untracked.InputTag('dbCentBin'),
			cms.untracked.InputTag('HFQ2', 'abs'),
			cms.untracked.InputTag('HFQ2', 'absp'),
			cms.untracked.InputTag('HFQ2', 'absm'),
			cms.untracked.InputTag('HFQ2', 'arg'),
			cms.untracked.InputTag('HFQ2', 'argp'),
			cms.untracked.InputTag('HFQ2', 'argm')
			)
	)

process.seqHFTree = cms.Sequence( process.centralityBin * process.dbCentBin * process.HFQ2 * process.QWHFTree )

# ZDC info
process.load('ZDC2018Pedestal_cfg')
process.zdcdigi = cms.EDProducer('QWZDC2018Producer',
		Src = cms.untracked.InputTag('hcalDigis', 'ZDC'),
		SOI = cms.untracked.int32(4),
		Pedestal = process.ZDC2018Pedestal_0,
		HardCode = cms.untracked.bool(True),
		Debug = cms.untracked.bool(False)
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

process.QWcent = cms.EDProducer('QWCentrality2018Producer',
		Src = cms.untracked.InputTag('hiCentrality')
		)
process.centTree = cms.EDAnalyzer('QWDoubleTreeMaker',
		src = cms.untracked.InputTag('QWcent'),
		vTag = cms.untracked.vstring(
			"etHFhitSum",
			"etHFhitSumPlus",
			"etHFhitSumMinus",
			"etHFtowerSum",
			"etHFtowerSumPlus",
			"etHFtowerSumMinus",
			"etHFtruncated",
			"etHFtruncatedPlus",
			"etHFtruncatedMinus",
			"etEESum",
			"etEESumPlus",
			"etEESumMinus",
			"etEEtruncated",
			"etEEtruncatedPlus",
			"etEEtruncatedMinus",
			"etEBSum",
			"etEcalSum",
			"etEBtruncated",
			"etEcalTrancated",
			"zdcSum",
			"zdcSumPlus",
			"zdcSumMinus",
			"pixelMultiplicity",
			"trackMultiplicity",
			"ntracksPtCut",
			"ntracksEtaCut",
			"ntracksEtaPtCut",
			"nPixelTracks",
			"etMidRapiditySum")
		)
process.cent = cms.Sequence( process.QWcent * process.centTree )

process.digiPath = cms.Path(
    process.hltSelect *
    process.eventSelection *
    process.zdcdigi *
    process.zdcana *
    process.seqHFTree *
    process.cent
)

process.TFileService = cms.Service("TFileService",
		fileName = cms.string('zdcTree.root')
		)

#process.outpath = cms.EndPath(process.output)
