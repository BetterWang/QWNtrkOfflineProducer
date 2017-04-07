import FWCore.ParameterSet.Config as cms

process = cms.Process("UPC")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = 100

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '75X_dataRun2_v13', '')

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/q/qwang/work/cleanroomRun2/Ana/data/ppReco.root")
#	fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch//store/hidata/HIRun2015/HIForward/AOD/02May2016-v1/70000/4E4C9AEA-6817-E611-BF9F-F01FAFD9C1F4.root')
)

import HLTrigger.HLTfilters.hltHighLevel_cfi

process.hltUPC = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltUPC.HLTPaths = [
	"HLT_HIUPCSingleMuNotHF2Pixel_SingleTrack_*",
]
process.hltUPC.andOr = cms.bool(True)
process.hltUPC.throw = cms.bool(False)


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('upc.root')
)

process.load('HeavyIonsAnalysis.Configuration.collisionEventSelection_cff')
process.clusterCompatibilityFilter.clusterPars = cms.vdouble(0.0,0.006)

process.primaryVertexFilter.src = cms.InputTag("offlinePrimaryVertices")

#process.eventSelection = cms.Sequence(
#        process.hfCoincFilter3
#        + process.primaryVertexFilter
##        + process.clusterCompatibilityFilter
#)
#


process.load('PbPb_HIMB5_ppReco_noeff')
process.load('RecoHI.HiCentralityAlgos.CentralityFilter_cfi')

process.ppNoffFilter0 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			2,3,4,5,6,7,8,9
			),
		BinLabel = cms.InputTag("Noff")
		)

process.ppNoffFilter10 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			10,11,12,13,14,15,16,17,18,19
			),
		BinLabel = cms.InputTag("Noff")
		)

process.ppNoffFilter20 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			20,21,22,23,24,25,26,27,28,29
			),
		BinLabel = cms.InputTag("Noff")
		)

process.ppNoffFilter30 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			30,31,32,33,34,35,36,37,38,39
			),
		BinLabel = cms.InputTag("Noff")
		)

process.ppNoffFilter40 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			40,41,42,43,44,45,46,47,48,49
			),
		BinLabel = cms.InputTag("Noff")
		)

process.ppNoffFilter50 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			50,51,52,53,54,55,56,57,58,59
			),
		BinLabel = cms.InputTag("Noff")
		)

process.ppNoffFilter60 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89
			),
		BinLabel = cms.InputTag("Noff")
		)

process.ppNoffFilter90 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			*range(90, 2000)
			),
		BinLabel = cms.InputTag("Noff")
		)

process.corr2DW0 = process.corr2DW.clone()
process.corr2DW10 = process.corr2DW.clone()
process.corr2DW20 = process.corr2DW.clone()
process.corr2DW30 = process.corr2DW.clone()
process.corr2DW40 = process.corr2DW.clone()
process.corr2DW50 = process.corr2DW.clone()
process.corr2DW60 = process.corr2DW.clone()
process.corr2DW90 = process.corr2DW.clone()

process.QWEvent.ptMin = cms.untracked.double(0.3)
process.QWEvent.ptMax = cms.untracked.double(5.0)

process.path   = cms.Path(process.hltUPC*process.Noff*process.QWEvent*process.vectMon*process.corr2DW)
process.path0  = cms.Path(process.hltUPC*process.Noff*process.ppNoffFilter0*process.QWEvent*process.corr2DW0)
process.path10 = cms.Path(process.hltUPC*process.Noff*process.ppNoffFilter10*process.QWEvent*process.corr2DW10)
process.path20 = cms.Path(process.hltUPC*process.Noff*process.ppNoffFilter20*process.QWEvent*process.corr2DW20)
process.path30 = cms.Path(process.hltUPC*process.Noff*process.ppNoffFilter30*process.QWEvent*process.corr2DW30)
process.path40 = cms.Path(process.hltUPC*process.Noff*process.ppNoffFilter40*process.QWEvent*process.corr2DW40)
process.path50 = cms.Path(process.hltUPC*process.Noff*process.ppNoffFilter50*process.QWEvent*process.corr2DW50)
process.path60 = cms.Path(process.hltUPC*process.Noff*process.ppNoffFilter60*process.QWEvent*process.corr2DW60)
process.path90 = cms.Path(process.hltUPC*process.Noff*process.ppNoffFilter90*process.QWEvent*process.corr2DW90)

process.schedule = cms.Schedule(
	process.path,
	process.path0,
	process.path10,
	process.path20,
	process.path30,
	process.path40,
	process.path50,
	process.path60,
	process.path90,
)
