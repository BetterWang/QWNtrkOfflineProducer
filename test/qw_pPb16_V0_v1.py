import FWCore.ParameterSet.Config as cms

process = cms.Process("PCA")

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')



process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))
#process.MessageLogger.cerr.FwkReport.reportEvery = 100

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_Prompt_v15', '')

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

process.source = cms.Source("PoolSource",
#        fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/q/qwang/work/cleanroomRun2/Ana/CMSSW_7_5_8_patch2/src/QWAna/QWCumuV3/test/HIMinBias_28.root")
	fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/q/qwang/work/cleanroomRun2/Ana/data/pPb_8_FEVT.root")
)

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring('keep *'),
    fileName = cms.untracked.string('V0.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('AOD')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('path')
    )
)


process.QWV0 = cms.EDProducer("QWV0VectProducer",
                vertexSrc = cms.untracked.InputTag("offlinePrimaryVertices"),
                trackSrc  = cms.untracked.InputTag("generalTracks"),
                V0Src     = cms.untracked.InputTag("generalV0Candidates", "Kshort"),
)


process.path = cms.Path(process.QWV0)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

process.schedule = cms.Schedule(
	process.path,
	process.RAWSIMoutput_step,
)
