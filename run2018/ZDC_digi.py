import os
import FWCore.ParameterSet.Config as cms
from getpass import getuser as user

process = cms.Process("QWTest")

process.load("FWCore.MessageService.MessageLogger_cfi")
#process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

#
#   Command Line Input(Copied from DQM for now)
#
import sys

#
#   Change the filename to process
#
runNumber = sys.argv[2]

process.source = cms.Source("HcalTBSource",
    fileNames = cms.untracked.vstring(
        'file:/afs/cern.ch/user/q/qwang/work/public/ZDC2018/example/B904_Integration_'+runNumber+'.root'
    )
)

process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(False)
        )

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('zdc_'+runNumber+'.root')
)

process.hcalDigis = cms.EDProducer("HcalRawToDigi",
                                   #       UnpackHF = cms.untracked.bool(True),
                                   FilterDataQuality = cms.bool(False),
                                   InputLabel = cms.InputTag('source'),
                                   ComplainEmptyData = cms.untracked.bool(False),
                                   # To unpack ZDC
                                   UnpackZDC = cms.untracked.bool(True),
                                   # in Current B904 Setup, ZDC is readout from FED 1180
                                   FEDs = cms.untracked.vint32(1180),
                                   #FEDs = cms.untracked.vint32(14),
                                   # don't know what firstSample and lastSample are
                                   firstSample = cms.int32(0),
                                   lastSample = cms.int32(2)
                                   )

#
#   For Debugging: Create a Pool Output Module
#
process.output = cms.OutputModule(
        'PoolOutputModule',
        fileName = cms.untracked.string('HFanalysis_'+runNumber+'.root')
)

process.load('Configuration.Geometry.GeometryIdeal_cff')

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.autoCond import autoCond
#from CondCore.DBCommon.CondDBSetup_cfi import *
from CondCore.CondDB.CondDB_cfi import *

process.GlobalTag.globaltag = autoCond['startup']

#   EMAP Needed for H2 DATA, load from txt
process.es_ascii = cms.ESSource('HcalTextCalibrations',
        input = cms.VPSet(
               cms.PSet(
                object = cms.string('ElectronicsMap'),
		file = cms.FileInPath('ZDC904_emap_v2.txt')
                )
        )
)

process.es_prefer = cms.ESPrefer('HcalTextCalibrations', 'es_ascii')

process.zdcdigi = cms.EDProducer('QWZDC2018Producer',
		Src = cms.untracked.InputTag('hcalDigis', 'ZDC')
		)

process.zdcana = cms.EDAnalyzer('QWZDC2018Analyzer',
		srcADC = cms.untracked.InputTag('zdcdigi', 'ADC')
		)

process.p = cms.Path(process.hcalDigis
		* process.zdcdigi
		* process.zdcana
                     )

process.outpath = cms.EndPath(process.output)


