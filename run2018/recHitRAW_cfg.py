import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
import os
import sys

runNumber = '321730'
if len(sys.argv) > 2:
	runNumber = sys.argv[2]

#---------------
# My definitions
#---------------

sourceTag = "PoolSource"         # for global runs
rawTag    = cms.InputTag('rawDataCollector')
era       = eras.Run2_2018
GT        = "101X_dataRun2_Prompt_v11"
filedir = '/eos/cms/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/'+runNumber[:3]+'/'+runNumber[3:]+'/00000'
infile    = cms.untracked.vstring()
for f in os.listdir(filedir):
	infile.append('file:'+filedir+'/'+f)

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
            file = cms.FileInPath("HcalElectronicsMap_2018_v3.0_data_ZDCRPD_mockup.txt")
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
    wantSummary = cms.untracked.bool(True),
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
    fileNames = cms.untracked.vstring(*infile)
    )
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('293765:264-293765:9999')


#-----------------------------------------
# CMSSW/Hcal Related Module import
#-----------------------------------------
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")


#set digi and analyzer
process.hcalDigis.InputLabel = rawTag

process.digiPath = cms.Path(
    process.hcalDigis
)

process.output = cms.OutputModule(
		'PoolOutputModule',
		outputCommands = cms.untracked.vstring("drop *",
			"keep *_*_ZDC_MyTree"
			),
		SelectEvents = cms.untracked.PSet(
			SelectEvents = cms.vstring('digiPath')
			),
		fileName = cms.untracked.string('digisRAW_'+runNumber+'.root')
		)

process.outpath = cms.EndPath(process.output)
