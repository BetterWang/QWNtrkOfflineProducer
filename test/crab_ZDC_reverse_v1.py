from CRABAPI.RawCommand import crabCommand
from CRABClient.UserUtilities import config, getUsernameFromSiteDB
from CRABClient.ClientExceptions import ClientException
from httplib import HTTPException

config = config()

config.General.requestName = 'PAMB1_ZDC_286178_v5'
config.General.workArea = 'CrabArea'
config.General.transferOutputs = True
config.General.transferLogs = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'qw_pPb16_ZDC_v1.py'
config.Data.inputDataset = '/PAMinimumBias1/PARun2016C-PromptReco-v1/AOD'
#config.Data.inputDBS = 'phys03'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 200
config.Data.outLFNDirBase = '/store/group/phys_heavyions/qwang/ZDC/'
config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/HI/Cert_285952-286496_HI8TeV_PromptReco_Pbp_Collisions16_JSON_NoL1T.txt'
config.Data.publication = False
config.Data.useParent = False
config.Data.runRange = '286178'
config.Site.storageSite = 'T2_CH_CERN'
config.Site.ignoreGlobalBlacklist = True
config.Data.allowNonValidInputDataset = True
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)

#####
config.General.requestName = 'PAMB1_ZDC_286200_v5'
config.Data.runRange = '286200'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)


#####
config.General.requestName = 'PAMB1_ZDC_286201_v5'
config.Data.runRange = '286201'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)


#####
config.General.requestName = 'PAMB1_ZDC_286288_v5'
config.Data.runRange = '286288'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)



#####
config.General.requestName = 'PAMB1_ZDC_286301_v5'
config.Data.runRange = '286301'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)



#####
config.General.requestName = 'PAMB1_ZDC_286302_v5'
config.Data.runRange = '286302'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)


#####
config.General.requestName = 'PAMB1_ZDC_286309_v5'
config.Data.runRange = '286309'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)



#####
config.General.requestName = 'PAMB1_ZDC_286314_v5'
config.Data.runRange = '286314'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)



#####
config.General.requestName = 'PAMB1_ZDC_286327_v5'
config.Data.runRange = '286327'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)




#####
config.General.requestName = 'PAMB1_ZDC_286329_v5'
config.Data.runRange = '286329'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)



#####
config.General.requestName = 'PAMB1_ZDC_286365_v5'
config.Data.runRange = '286365'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)



#####
config.General.requestName = 'PAMB1_ZDC_286420_v5'
config.Data.runRange = '286420'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)



#####
config.General.requestName = 'PAMB1_ZDC_286422_v5'
config.Data.runRange = '286422'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)



#####
config.General.requestName = 'PAMB1_ZDC_286425_v5'
config.Data.runRange = '286425'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)



#####
config.General.requestName = 'PAMB1_ZDC_286441_v5'
config.Data.runRange = '286441'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)




#####
config.General.requestName = 'PAMB1_ZDC_286442_v5'
config.Data.runRange = '286442'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)



#####
config.General.requestName = 'PAMB1_ZDC_286450_v5'
config.Data.runRange = '286450'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)



#####
config.General.requestName = 'PAMB1_ZDC_286471_v5'
config.Data.runRange = '286471'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)



#####
config.General.requestName = 'PAMB1_ZDC_286496_v5'
config.Data.runRange = '286496'
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)

