#-------------------------------------
#	Hcal DQM Application using New DQM Sources/Clients
#-------------------------------------

#-------------------------------------
#	Standard Python Imports
#-------------------------------------
import os, sys, socket, string

#-------------------------------------
#	Standard CMSSW Imports/Definitions
#-------------------------------------
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
process      = cms.Process('HCALDQM', eras.Run2_2018_pp_on_AA)
subsystem    = 'Hcal'
cmssw        = os.getenv("CMSSW_VERSION").split("_")
debugstr     = "### HcalDQM::cfg::DEBUG: "
warnstr      = "### HcalDQM::cfg::WARN: "
errorstr     = "### HcalDQM::cfg::ERROR:"
useOfflineGT = False
useFileInput = False
useMap       = True 

#-------------------------------------
#	Central DQM Stuff imports
#-------------------------------------
from DQM.Integration.config.online_customizations_cfi import *
if useOfflineGT:
	process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
	process.GlobalTag.globaltag = '100X_dataRun2_HLT_v1'
	#process.GlobalTag.globaltag = '100X_dataRun2_HLT_Candidate_2018_01_31_16_04_35'
else:
	process.load('DQM.Integration.config.FrontierCondition_GT_cfi')
if useFileInput:
	process.load("DQM.Integration.config.fileinputsource_cfi")
else:
	process.load('DQM.Integration.config.inputsource_cfi')
process.load('DQM.Integration.config.environment_cfi')


#-------------------------------------
#	Central DQM Customization
#-------------------------------------
process.dqmEnv.subSystemFolder = subsystem
process.dqmSaver.tag = subsystem
referenceFileName = '/dqmdata/dqm/reference/hcal_reference.root'
process.DQMStore.referenceFileName = referenceFileName
process = customise(process)
process.DQMStore.verbose = 0
process.source.minEventsPerLumi=100

#-------------------------------------
#	CMSSW/Hcal non-DQM Related Module import
#-------------------------------------
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.load('EventFilter.CastorRawToDigi.CastorRawToDigi_cff')
process.load("SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff")

#-------------------------------------
#	CMSSW/Hcal non-DQM Related Module Settings
#	-> runType
#	-> Generic Input tag for the Raw Collection
#	-> cmssw version
#	-> Turn off default blocking of dead channels from rechit collection
#	-> Drop Channel Status Bits (had benn 'HcalCellOff', "HcalCellDead")
#	-> For Trigger Primitives Emulation
#	-> L1 GT setting
#	-> Rename the hbheprereco to hbhereco
#-------------------------------------
runType			= process.runType.getRunType()
runTypeName		= process.runType.getRunTypeName()
isCosmicRun		= runTypeName=="cosmic_run" or runTypeName=="cosmic_run_stage1"
isHeavyIon		= runTypeName=="hi_run"
cmssw			= os.getenv("CMSSW_VERSION").split("_")
rawTag			= cms.InputTag("rawDataCollector")
rawTagUntracked = cms.untracked.InputTag("rawDataCollector")
if isHeavyIon:
	rawTag = cms.InputTag("rawDataRepacker")
	rawTagUntracked = cms.untracked.InputTag("rawDataRepacker")
	process.castorDigis.InputLabel = rawTag

# Enable ZS on emulated TPs, to match what is done in data
process.hcalDigis.InputLabel = rawTag

# For sent-received comparison
process.load("L1Trigger.Configuration.L1TRawToDigi_cff")

# Exclude the laser FEDs. They contaminate the QIE10/11 digi collections. 
#from Configuration.Eras.Modifier_run2_HCAL_2017_cff import run2_HCAL_2017
#run2_HCAL_2017.toModify(process.hcalDigis, FEDs=cms.untracked.vint32(724,725,726,727,728,729,730,731,1100,1101,1102,1103,1104,1105,1106,1107,1108,1109,1110,1111,1112,1113,1114,1115,1116,1117,1118,1119,1120,1121,1122,1123))

#-------------------------------------
#	Hcal DQM Tasks and Harvesters import
#	New Style
#-------------------------------------
process.load("DQM.HcalTasks.DigiTask")
process.load('DQM.HcalTasks.TPTask')
process.load('DQM.HcalTasks.RawTask')
process.load('DQM.HcalTasks.NoCQTask')
#process.load('DQM.HcalTasks.ZDCTask')
#process.load('DQM.HcalTasks.QIE11Task') # 2018: integrate QIE11Task into DigiTask
process.load('DQM.HcalTasks.HcalOnlineHarvesting')
process.load('DQM.HcalTasks.HcalQualityTests')

#-------------------------------------
#	To force using uTCA
#	Will not be here for Online DQM
#-------------------------------------
if useMap:
    process.es_ascii = cms.ESSource(
        'HcalTextCalibrations',
        input = cms.VPSet(
            cms.PSet(
                object = cms.string('ElectronicsMap'),
                file = cms.FileInPath("QWAna/QWNtrkOfflineProducer/run2018/HcalElectronicsMap_2018_v3.0_data_ZDCRPD_mockup.txt")
                )
            )
        )
    process.es_prefer = cms.ESPrefer('HcalTextCalibrations', 'es_ascii')

#-------------------------------------
#	For Debugginb
#-------------------------------------
#process.hcalDigiTask.moduleParameters.debug = 10

#-------------------------------------
#	Some Settings before Finishing up
#	New Style Modules
#-------------------------------------
oldsubsystem = subsystem

#-------------------------------------
#	Hcal DQM Tasks/Clients Sequences Definition
#-------------------------------------
# event info
process.QWInfo = cms.EDProducer('QWEventInfoProducer')

# ZDC info
process.load('QWZDC2018Producer_cfi')
process.zdcdigi.SOI = cms.untracked.int32(1)

process.QWzdcQIE10Task = DQMEDAnalyzer('QWZDCQIE10Task',
		srcADC = cms.untracked.InputTag('zdcdigi', 'ADC'),
		srcfC = cms.untracked.InputTag('zdcdigi', 'regularfC'),
		srcDetId = cms.untracked.InputTag('zdcdigi', 'DetId'),
		srcHigh = cms.untracked.InputTag('zdcdigi', 'chargeHigh'),
		srcLow = cms.untracked.InputTag('zdcdigi', 'chargeLow'),
		srcSum = cms.untracked.InputTag('zdcdigi', 'chargeSum'),
		)

process.QWBXTask = DQMEDAnalyzer('QWZDC2018BXTask',
		srcfC = cms.untracked.InputTag('zdcdigi', 'regularfC'),
		srcDid = cms.untracked.InputTag('zdcdigi', 'DetId'),
		BX = cms.untracked.InputTag('QWInfo', 'BX'),
		SOI = cms.untracked.int32(1),
		BXShift = cms.untracked.int32(0),
		)

process.tasksPath = cms.Path(
    process.hcalDigis 
    * process.zdcdigi
    * process.QWInfo
    * process.QWBXTask
)

process.harvestingPath = cms.Path(
	process.hcalOnlineHarvesting
)

#-------------------------------------
#	Paths/Sequences Definitions
#-------------------------------------
process.preRecoPath = cms.Path(
		process.hcalDigis
)

process.dqmPath = cms.EndPath(
		process.dqmEnv)
process.dqmPath1 = cms.EndPath(
		process.dqmSaver
)
process.qtPath = cms.Path(process.hcalQualityTests)

process.schedule = cms.Schedule(
	process.tasksPath,
#	process.harvestingPath,
#	process.dqmPath,
#	process.dqmPath1
)

#-------------------------------------
#	Scheduling and Process Customizations
#-------------------------------------
process.options = cms.untracked.PSet(
		Rethrow = cms.untracked.vstring(
			"ProductNotFound",
			"TooManyProducts",
			"TooFewProducts"
		)
)
process.options.wantSummary = cms.untracked.bool(True)

# tracer
#process.Tracer = cms.Service("Tracer")
process = customise(process)
