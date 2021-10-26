import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
import os
import sys
import FWCore.ParameterSet.VarParsing as VarParsing

#jsonfile = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/HI/DCSOnly/json_DCSONLY_HI.txt'
#/eos/cms/tier0/store/data/Commissioning2021/MinimumBias/RAW/v1/000/345/683/00000
# 345823, 345828

options = VarParsing.VarParsing('analysis')

options.register('runNumber',
		345683,
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.int,
		"Run number.")

options.register('skipEvents',
		0,
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.int,
		"Skip first N events.")

options.register('runInputDir',
		'/eos/cms/tier0/store/data/Commissioning2021/MinimumBias/RAW/v1/000/',
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
		'PoolSource', # default value
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
		"Source type. Can be either PoolSource, or Streamer")

options.register('rawTag',
		'rawDataCollector', # default value
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
		"RAW input tag. Can be either rawDataCollector, or rawDataRepacker, or rawDataReducedFormat, or virginRawDataRepacker")

options.register('Json',
		'/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/HI/DCSOnly/json_DCSONLY_HI.txt', # default value
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
		"Json.")

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
GT        = "120X_dataRun2_v2"

#-----------------------------------
# Standard CMSSW Imports/Definitions
#-----------------------------------
process = cms.Process('MyTree')

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = GT

from CondCore.CondDB.CondDB_cfi import *
process.es_pool = cms.ESSource("PoolDBESSource",
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(
        cms.PSet(
            record = cms.string("HcalElectronicsMapRcd"),
            tag = cms.string("HcalElectronicsMap_2021_v2.0_data")
        )
    ),
    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
        authenticationMethod = cms.untracked.uint32(0)
    )

#-----------------------------------
# Input source
#-----------------------------------
if options.source=='PoolSource':
    filedir = options.runInputDir+runNumber[:3]+'/'+runNumber[3:]+'/00000'
    print(filedir)
    infile    = cms.untracked.vstring()
    for f in reversed(os.listdir(filedir)):
        if f[-5:] == '.root' :
            infile.append('file:'+filedir+'/'+f)
    process.source = cms.Source('PoolSource',
        fileNames = infile,
        skipEvents=cms.untracked.uint32(options.skipEvents)
        )
    if options.Json != '':
       import FWCore.PythonUtilities.LumiList as LumiList
       process.source.lumisToProcess = LumiList.LumiList(filename = options.Json).getVLuminosityBlockRange()
elif options.source=='B904':
    infile = "file:"+options.runInputDir+"/run"+runNumber+"/B904_Integration_"+runNumber+'.root'
    print(infile)
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
    'emap_ext'  : "QWAna/QWNtrkOfflineProducer/run2021/ZDCemap_2021_pilotRun_M_only.v2.txt"
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
else:
    process.es_prefer_es_pool = cms.ESPrefer( "PoolDBESSource", "es_pool" )



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
process.load('Configuration.StandardSequences.MagneticField_38T_cff')

#set digi and analyzer
process.hcalDigis.InputLabel = rawTag

process.eventSelection = cms.Sequence()

process.digis = cms.Sequence(process.hcalDigis)

# ZDC info
process.load('QWZDC2018Producer_cfi')
process.load('ZDC2018Pedestal_cfg')
process.zdcdigi.SOI = cms.untracked.int32(4)

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

process.zdcADCFilter = cms.EDFilter('QWZDC2018ADCFilter',
		srcDetId = cms.untracked.InputTag('zdcdigi', 'DetId'),
		srcCapId = cms.untracked.InputTag('zdcdigi', 'CapId'),
		srcADC = cms.untracked.InputTag('zdcdigi', 'ADC'),
		)

process.tree = cms.EDAnalyzer('QWTreeMaker',
    Vtags = cms.untracked.VPSet(
        cms.PSet(
            tag = cms.untracked.InputTag('zdcdigi', 'ADC'),
            name = cms.untracked.string('ADC')
            ),
        cms.PSet(
            tag = cms.untracked.InputTag('zdcdigi', 'CapId'),
            name = cms.untracked.string('CapId')
            ),
        ),
        Dtags = cms.untracked.VPSet()
    )

process.digiPath = cms.Path(
    process.hltSelect *
    process.digis *
    process.zdcdigi *
    process.zdcADCFilter *
#    process.QWInfo *
#    process.zdcBX *
#    process.zdccalibana *
#    process.tree *
    process.zdcana
)


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
