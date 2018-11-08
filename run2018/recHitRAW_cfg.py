import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
import os
import sys
import FWCore.ParameterSet.VarParsing as VarParsing


options = VarParsing.VarParsing('analysis')

options.register('runNumber',
		111,
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.int,
		"Run number.")

options.register('skipEvents',
		0,
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.int,
		"Skip first N events.")

options.register('runInputDir',
		'/eos/cms/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/',
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
                "Directory where the RAW files will appear. lxplus: /eos/cms/store/t0streamer/Data/HIPhysicsMinimumBiasReducedFormat0/ or ...")

options.register('outputTag',
		'',
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
                "Output file name tag.")

options.register('hlt',
		'',
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
                "HLT path")

options.register('source',
		'Streamer', # default value
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
		"Source type. Can be either PoolSource, or Streamer")

options.register('rawTag',
		'rawDataRepacker', # default value
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
		"RAW input tag. Can be either rawDataCollector, or rawDataRepacker, or rawDataReducedFormat, or virginRawDataRepacker")

options.register('normed',
		False, # default value
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.bool,
		"Normed.")

options.register('bTree',
		False, # default value
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.bool,
		"Generate TTree.")

options.register('emap',
		'', # default value
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
		"Emap used for ZDC. Can be 'mockup', 'real', or 'FCD'")

options.parseArguments()


#---------------
# My definitions
#---------------

rawTag    = cms.InputTag(options.rawTag)
runNumber = str(options.runNumber)
GT        = "103X_dataRun2_HLT_v1"


#-----------------------------------
# Standard CMSSW Imports/Definitions
#-----------------------------------
process = cms.Process('MyTree',eras.Run2_2018_pp_on_AA)

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = GT

#-----------------------------------
# Input source
#-----------------------------------
if options.source=='PoolSource':
    filedir = options.runInputDir+runNumber[:3]+'/'+runNumber[3:]+'/00000'
    print filedir
    infile    = cms.untracked.vstring()
    for f in reversed(os.listdir(filedir)):
    	if f[-5:] == '.root' :
    		infile.append('file:'+filedir+'/'+f)
    process.source = cms.Source('PoolSource',
        fileNames = infile,
	skipEvents=cms.untracked.uint32(options.skipEvents)
        )
elif options.source=='B904':
    infile = "file:"+options.runInputDir+"/run"+runNumber+"/B904_Integration_"+runNumber+'.root'
    print infile
    process.source = cms.Source('HcalTBSource',
        fileNames = cms.untracked.vstring(infile),
        skipEvents=cms.untracked.uint32(options.skipEvents)
        )
elif options.source=='HcalTBSource':
    infile    = 'file:'+options.runInputDir+'/run'+runNumber+'/USC_'+runNumber+'.root'
    process.source = cms.Source(
        'HcalTBSource',
        fileNames = cms.untracked.vstring(infile),
        skipEvents=cms.untracked.uint32(options.skipEvents)
        )
else:
    filedir = options.runInputDir+'000/'+runNumber[:3]+'/'+runNumber[3:]
    infile    = cms.untracked.vstring()
    for f in reversed(os.listdir(filedir)):
    	if f[-4:] == '.dat' :
    		infile.append('file:'+filedir+'/'+f)
    process.source = cms.Source('NewEventStreamFileReader',
        fileNames = infile,
	skipEvents=cms.untracked.uint32(options.skipEvents)
        )

#----------
_emap = {
    'mockup'     : "QWAna/QWNtrkOfflineProducer/run2018/HcalElectronicsMap_2018_v3.0_data_ZDCRPD_mockup.txt",
    'real'       : "QWAna/QWNtrkOfflineProducer/run2018/HcalElectronicsMap_2018_v3.0_data.txt",
    'reduced'    : "QWAna/QWNtrkOfflineProducer/run2018/HcalElectronicsMap_2018_v3.0_data_reduced.txt",
    'ext'        : "QWAna/QWNtrkOfflineProducer/run2018/HcalElectronicsMap_2018_v3.0_data_ext.txt",
    'FCD'        : "QWAna/QWNtrkOfflineProducer/run2018/HcalElectronicsMap_2018_v3.0_data_FCD.txt",
    '904ext12'   : "QWAna/QWNtrkOfflineProducer/run2018/ZDC904_emap_ext12.txt",
}

if options.emap != '':
    process.es_ascii = cms.ESSource(
        'HcalTextCalibrations',
        input = cms.VPSet(
            cms.PSet(
                object = cms.string('ElectronicsMap'),
                file = cms.FileInPath(_emap[options.emap])
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
    input = cms.untracked.int32(options.maxEvents)
    )

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('293765:264-293765:9999')

#-----------------------------------------
# HLT selection -- Random
#-----------------------------------------

import HLTrigger.HLTfilters.hltHighLevel_cfi

if options.hlt!='':
    process.hltSelect = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
    process.hltSelect.HLTPaths = [
        options.hlt+"*",
    ]
    process.hltSelect.andOr = cms.bool(True)
    process.hltSelect.throw = cms.bool(False)
else:
    process.hltSelect = cms.Sequence()

#-----------------------------------------
# CMSSW/Hcal Related Module import
#-----------------------------------------
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")


#set digi and analyzer
process.hcalDigis.InputLabel = rawTag

if options.rawTag == '':
    process.digis = cms.Sequence()
else:
    process.digis = cms.Sequence(process.hcalDigis)

# event info
process.QWInfo = cms.EDProducer('QWEventInfoProducer')
process.QWBXTree = cms.EDAnalyzer('QWDTagTreeMaker',
		vTag = cms.untracked.VInputTag( cms.untracked.InputTag('QWInfo', 'BX') )
	)
process.BXTree = cms.Sequence( process.QWInfo * process.QWBXTree )

# ZDC info
process.load('QWZDC2018Producer_cfi')
#process.load('ZDC2018Pedestal_cfg')
process.zdcdigi.SOI = cms.untracked.int32(4)

process.zdcana = cms.EDAnalyzer('QWZDC2018Analyzer',
		srcADC = cms.untracked.InputTag('zdcdigi', 'ADC'),
		srcfC = cms.untracked.InputTag('zdcdigi', 'regularfC'),
		srcDetId = cms.untracked.InputTag('zdcdigi', 'DetId'),
		srcCapId = cms.untracked.InputTag('zdcdigi', 'CapId'),
		srcHigh = cms.untracked.InputTag('zdcdigi', 'chargeHigh'),
		srcLow = cms.untracked.InputTag('zdcdigi', 'chargeLow'),
		srcSum = cms.untracked.InputTag('zdcdigi', 'chargeSum'),
		Norm = cms.untracked.bool(options.normed),
		bTree = cms.untracked.bool(options.bTree)
		)

process.zdccalibana = cms.EDAnalyzer('QWZDC2018CalibAnalyzer',
		srcDetId = cms.untracked.InputTag('zdcdigi', 'DetId'),
		srcHigh = cms.untracked.InputTag('zdcdigi', 'chargeHigh'),
		srcLow = cms.untracked.InputTag('zdcdigi', 'chargeLow'),
		srcSum = cms.untracked.InputTag('zdcdigi', 'chargeSum'),
		)

process.zdcBX = cms.EDAnalyzer('QWZDC2018BXAnalyzer',
		BX = cms.untracked.InputTag('QWInfo', 'BX'),
		srcfC = cms.untracked.InputTag('zdcdigi', 'regularfC'),
		srcDetId = cms.untracked.InputTag('zdcdigi', 'DetId'),
		SOI = cms.untracked.int32(4)
		)

process.zdcADCFilter = cms.EDFilter('QWZDC2018ADCFilter',
		srcDetId = cms.untracked.InputTag('zdcdigi', 'DetId'),
		srcCapId = cms.untracked.InputTag('zdcdigi', 'CapId'),
		srcADC = cms.untracked.InputTag('zdcdigi', 'ADC'),
		)

process.digiPath = cms.Path(
    process.hltSelect *
    process.digis *
    process.zdcdigi *
    process.zdcADCFilter *
    process.QWInfo *
    process.zdcBX *
    process.zdccalibana *
    process.zdcana
)

if options.bTree:
	process.digiPath += process.BXTree

process.TFileService = cms.Service("TFileService",
		fileName = cms.string('zdc_'+runNumber+options.outputTag+'.root')
		)

process.output = cms.OutputModule(
		'PoolOutputModule',
		outputCommands = cms.untracked.vstring("drop *",
			"keep *_zdcdigi_*_MyTree",
			"keep *_*_ZDC_MyTree",
			"keep *_TriggerResults_*_MyTree",
			),
		SelectEvents = cms.untracked.PSet(
			SelectEvents = cms.vstring('digiPath')
			),
		fileName = cms.untracked.string('digisRAW_'+runNumber+'.root')
		)

#process.outpath = cms.EndPath(process.output)
