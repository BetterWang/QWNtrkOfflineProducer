import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
import os
import sys

runNumber = '1000031366'
if len(sys.argv) > 2:
	runNumber = sys.argv[2]


#---------------
# My definitions
#---------------

sourceTag = "HcalTBSource"         # for 904/local runs
rawTag    = cms.InputTag('source')
era       = eras.Run2_2018
GT        = "102X_dataRun2_Prompt_v7"
infile    = 'file:/eos/cms/store/group/dpg_hcal/comm_hcal/B904/run'+runNumber+'/B904_Integration_'+runNumber+'.root'



#-----------------------------------
# Standard CMSSW Imports/Definitions
#-----------------------------------
process = cms.Process('MyTree',era)

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = GT

#----------
process.es_ascii = cms.ESSource(
    'HcalTextCalibrations',
    input = cms.VPSet(
        cms.PSet(
            object = cms.string('ElectronicsMap'),
            file = cms.FileInPath("QWAna/QWNtrkOfflineProducer/run2018/ZDC904_emap_ext12.txt")
            )
        )
    )
process.es_prefer = cms.ESPrefer('HcalTextCalibrations', 'es_ascii')

#-----------
# Log output
#-----------
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = ''
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(False),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )


#-----------------
# Files to process
#-----------------
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.source = cms.Source(
    sourceTag,
    fileNames = cms.untracked.vstring(infile)
    )
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('293765:264-293765:9999')


#-----------------------------------------
# CMSSW/Hcal Related Module import
#-----------------------------------------
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")


#set digi and analyzer
process.hcalDigis.InputLabel = rawTag
#process.hcalDigis.saveQIE10DataNSamples = cms.untracked.vint32(10)
#process.hcalDigis.saveQIE10DataTags = cms.untracked.vstring( "UNUSED10" )

process.zdcdigi = cms.EDProducer('QWZDC2018Producer',
                Src = cms.untracked.InputTag('hcalDigis', 'ZDC'),
                HardCode = cms.untracked.bool(True),
                Debug = cms.untracked.bool(False)
                )

process.zdcana = cms.EDAnalyzer('QWZDC2018Analyzer',
		srcADC = cms.untracked.InputTag('zdcdigi', 'ADC'),
		srcfC = cms.untracked.InputTag('zdcdigi', 'regularfC'),
		srcDetId = cms.untracked.InputTag('zdcdigi', 'DetId')
		)
process.zdcped = cms.EDAnalyzer('QWZDC2018Calib',
		srcCapId = cms.untracked.InputTag('zdcdigi', 'CapId'),
		srcfC = cms.untracked.InputTag('zdcdigi', 'regularfC'),
		srcDetId = cms.untracked.InputTag('zdcdigi', 'DetId')
		)

process.digiPath = cms.Path(
    process.hcalDigis *
    process.zdcdigi *
    process.zdcped
)

process.TFileService = cms.Service("TFileService",
		fileName = cms.string('zdcped_'+runNumber+'.root')
		)

process.output = cms.OutputModule(
		'PoolOutputModule',
		outputCommands = cms.untracked.vstring("drop *",
			"keep *_*_*_MyTree"
			),
		SelectEvents = cms.untracked.PSet(
			SelectEvents = cms.vstring('digiPath')
			),
		fileName = cms.untracked.string('digis904_'+runNumber+'.root')
		)

process.outpath = cms.EndPath(process.output)
