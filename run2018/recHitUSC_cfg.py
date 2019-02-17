import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
import os
import sys

runNumber = '320574'
if len(sys.argv) > 2:
	runNumber = sys.argv[2]

#---------------
# My definitions
#---------------

sourceTag = "HcalTBSource"         # for global runs
rawTag    = cms.InputTag('source')
era       = eras.Run2_2018
GT        = "101X_dataRun2_Prompt_v11"
infile    = 'file:/eos/cms/store/group/dpg_hcal/comm_hcal/USC/run'+runNumber+'/USC_'+runNumber+'.root'



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
            file = cms.FileInPath("HcalElectronicsMap_2018_v3.0_data.txt")
            )
        )
    )
process.es_prefer = cms.ESPrefer('HcalTextCalibrations', 'es_ascii')

#-----------
#process.load("CondCore.DBCommon.CondDBSetup_cfi")
#process.es_pool = cms.ESSource("PoolDBESSource",
#                              process.CondDBSetup,
#                              timetype = cms.string('runnumber'),
#                              toGet = cms.VPSet(
#                                     cms.PSet(record = cms.string("HcalRespCorrsRcd"),
#                                     tag = cms.string("HcalElectronicsMap_2018_v3.0_data.txt")
#                                     )
#                             ),
# connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
# authenticationMethod = cms.untracked.uint32(0)
#)
#process.es_prefer_es_pool = cms.ESPrefer( "PoolDBESSource", "es_pool" )
#

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
    input = cms.untracked.int32(1)
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
process.hcalDigis.saveQIE10DataNSamples = cms.untracked.vint32(6, 10)
process.hcalDigis.saveQIE10DataTags = cms.untracked.vstring( "UNUSED6", "UNUSED10" )

process.digiPath = cms.Path(
    process.hcalDigis
)

process.output = cms.OutputModule(
		'PoolOutputModule',
		outputCommands = cms.untracked.vstring("drop *",
			"keep *_*_*_MyTree"
			),
		SelectEvents = cms.untracked.PSet(
			SelectEvents = cms.vstring('digiPath')
			),
		fileName = cms.untracked.string('digis_USC'+runNumber+'.root')
		)

process.outpath = cms.EndPath(process.output)
