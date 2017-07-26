from CRABAPI.RawCommand import crabCommand
from CRABClient.UserUtilities import config, getUsernameFromSiteDB
from CRABClient.ClientExceptions import ClientException
from httplib import HTTPException

config = config()

config.General.requestName = 'PAMB1_ZDC_NegOff_Skim_v1'
config.General.workArea = 'CrabArea'
config.General.transferOutputs = True
config.General.transferLogs = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'qw_pPb16_MB_NegOff_Skim_v1.py'
config.Data.inputDataset = '/PAMinimumBias1/PARun2016C-PromptReco-v1/AOD'
#config.Data.inputDBS = 'phys03'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 50
#config.Data.outLFNDirBase = '/store/user/qwang/ZDC/'
config.Data.outLFNDirBase = '/store/group/phys_heavyions/qwang/ZDC/'
config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/HI/Cert_285952-286496_HI8TeV_PromptReco_Pbp_Collisions16_JSON_NoL1T.txt'
config.Data.publication = False
config.Data.useParent = False
config.Data.runRange = '286178-286496'
config.Data.publication = True
config.Data.outputDatasetTag = 'PAMB1_NegOff_v1'
config.Site.storageSite = 'T2_CH_CERN'
#config.Site.storageSite = 'T2_US_MIT'
config.Site.ignoreGlobalBlacklist = True
config.Data.allowNonValidInputDataset = True
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)