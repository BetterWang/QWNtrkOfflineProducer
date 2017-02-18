import FWCore.ParameterSet.Config as cms

process = cms.Process("GEN")

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('GeneratorInterface.HiGenCommon.AfterBurnerGenerator_cff')


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(200))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

process.source = cms.Source("EmptySource")

process.generator = cms.EDFilter("HijingGeneratorFilter",
		rotateEventPlane = cms.bool(True),
		frame = cms.string('CMS     '),
		targ = cms.string('A       '),
		izp = cms.int32(82),
		bMin = cms.double(0),
		izt = cms.int32(82),
		proj = cms.string('A       '),
		comEnergy = cms.double(5023.0),
		iat = cms.int32(208),
		bMax = cms.double(15),
		iap = cms.int32(208)
		)

configurationMetadata = cms.untracked.PSet(
		version = cms.untracked.string('$Revision: 1.3 $'),
		annotation = cms.untracked.string('HIJING generator'),
		name = cms.untracked.string('$Source: QW $')
		)

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring('keep *'),
    fileName = cms.untracked.string('Hijing.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

process.AftBurner.modv1 = cms.InputTag("0.0")
#process.AftBurner.modv2 = cms.InputTag("fEllP_8pct_v2")
#process.AftBurner.modv2 = cms.InputTag("pBG_4pct_v2")
process.AftBurner.modv3 = cms.InputTag("pBG_2pct_v3")
process.AftBurner.modmethod = cms.int32(2)

################ comment this out to run without QWNtrkOfflineProducer
process.QWGenEvent = cms.EDProducer("QWGenEventProducer",
		trackSrc  = cms.untracked.InputTag("genParticles")
		)

process.vectPhi = cms.EDAnalyzer('QWVectorAnalyzer',
		src = cms.untracked.InputTag("QWGenEvent", "phi"),
		hNbins = cms.untracked.int32(5000),
		hstart = cms.untracked.double(0),
		hend = cms.untracked.double(5000),
		cNbins = cms.untracked.int32(1000),
		cstart = cms.untracked.double(-3.14159265358979323846),
		cend = cms.untracked.double(3.14159265358979323846),
		)

process.TFileService = cms.Service("TFileService",
		fileName = cms.string('cumu.root')
		)

process.pgen = cms.Sequence(cms.SequencePlaceholder("randomEngineStateProducer")+process.AfterBurner+process.GeneInfo + process.QWGenEvent + process.vectPhi)
################ comment END
#process.pgen = cms.Sequence(cms.SequencePlaceholder("randomEngineStateProducer")+process.AfterBurner+process.GeneInfo)

from IOMC.RandomEngine.RandomServiceHelper import RandomNumberServiceHelper
randSvc = RandomNumberServiceHelper(process.RandomNumberGeneratorService)
randSvc.populate()

process.generation_step = cms.Path(process.pgen)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)


process.schedule = cms.Schedule(
	process.generation_step,
	process.RAWSIMoutput_step
)


for path in process.paths:
                getattr(process,path)._seq = process.generator * getattr(process,path)._seq

