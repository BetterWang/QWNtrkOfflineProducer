import FWCore.ParameterSet.Config as cms

# Manual Pedestal calibration

# 904 uHTR slot 12, QIE #5 in slot 4, #6 in slot 10
# emap: ZDC904_emap_ext12.txt
# run: 1000031435

ZDC2018Pedestal_904_ext12 = cms.VPSet(
		cms.PSet(
			object = cms.untracked.string('hZDCM_EM1'),
			ped = cms.untracked.vdouble(15.606, 15.3491, 14.7818, 13.4043)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_EM2'),
			ped = cms.untracked.vdouble(9.5454, 12.3147, 8.26609, 9.93686)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_EM3'),
			ped = cms.untracked.vdouble(13.5441, 12.8479, 14.571, 14.9084)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_EM4'),
			ped = cms.untracked.vdouble(14.7521, 14.4579, 17.1729, 14.8872)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_EM5'),
			ped = cms.untracked.vdouble(14.6007, 14.7184, 14.2629, 16.7196)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_HAD1'),
			ped = cms.untracked.vdouble(15.036, 12.4895, 14.713, 14.2131)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_HAD2'),
			ped = cms.untracked.vdouble(14.9643, 16.1604, 12.3311, 14.3595)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_HAD3'),
			ped = cms.untracked.vdouble(10.9024, 12.1848, 11.3597, 10.8579)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_EM1'),
			ped = cms.untracked.vdouble(15.982, 10.4949, 12.7527, 11.575)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_EM2'),
			ped = cms.untracked.vdouble(  14.8, 14.5702, 11.2533, 13.5095)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_EM3'),
			ped = cms.untracked.vdouble(17.0116, 14.5225, 11.5343, 15.5811)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_EM4'),
			ped = cms.untracked.vdouble(13.6508, 11.987, 11.2885, 16.4723)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_EM5'),
			ped = cms.untracked.vdouble(14.3426, 13.0859, 10.5663, 14.6305)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_HAD1'),
			ped = cms.untracked.vdouble(13.9172, 11.6089, 11.3117, 15.7986)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_HAD2'),
			ped = cms.untracked.vdouble(14.5024, 12.9188, 14.8549, 14.5081)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_HAD3'),
			ped = cms.untracked.vdouble(9.16921, 8.10088, 8.15071, 8.6963)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_RPD0'),
			ped = cms.untracked.vdouble(20.3504, 18.3011, 20.3966, 18.8418)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_RPD1'),
			ped = cms.untracked.vdouble(15.5619, 16.7027, 14.796, 16.5082)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_RPD2'),
			ped = cms.untracked.vdouble(14.3656, 9.97622, 13.5149, 10.5256)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_RPD3'),
			ped = cms.untracked.vdouble(18.5245, 17.9001, 16.9661, 15.3701)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_RPD4'),
			ped = cms.untracked.vdouble(15.1588, 14.6709,  17.87, 14.5689)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_RPD5'),
			ped = cms.untracked.vdouble(14.0652, 14.5502, 14.0321, 11.2417)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_RPD6'),
			ped = cms.untracked.vdouble(15.1462, 13.624, 14.9373, 14.5511)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_RPD7'),
			ped = cms.untracked.vdouble(15.0989, 14.7611, 13.9193, 17.6621)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_RPD8'),
			ped = cms.untracked.vdouble(17.8855, 14.0973, 15.6661, 17.4102)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_RPD9'),
			ped = cms.untracked.vdouble(20.1679, 19.4383, 14.9966, 20.4873)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_RPD10'),
			ped = cms.untracked.vdouble(11.9747, 14.4168, 11.9135, 16.1451)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_RPD11'),
			ped = cms.untracked.vdouble(16.2777, 14.4478, 15.6334, 14.147)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_RPD12'),
			ped = cms.untracked.vdouble(15.3498, 15.1837, 14.552, 14.4021)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_RPD13'),
			ped = cms.untracked.vdouble(11.5927, 11.8388, 10.4409, 14.7141)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_RPD14'),
			ped = cms.untracked.vdouble(18.0928, 20.3203, 20.5187, 17.7369)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCM_RPD15'),
			ped = cms.untracked.vdouble(20.2375, 21.2191, 17.9609, 20.6106)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_RPD0'),
			ped = cms.untracked.vdouble(15.5897, 17.5748, 18.0703, 14.7896)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_RPD1'),
			ped = cms.untracked.vdouble(12.9444, 14.7272, 11.5537, 13.1901)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_RPD2'),
			ped = cms.untracked.vdouble(19.3295, 16.9701, 17.412, 18.4778)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_RPD3'),
			ped = cms.untracked.vdouble(15.9374, 14.3689, 14.6512, 13.9714)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_RPD4'),
			ped = cms.untracked.vdouble(17.3638, 13.9387, 17.6703, 18.1531)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_RPD5'),
			ped = cms.untracked.vdouble(15.9989, 16.2604, 16.3655, 17.1847)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_RPD6'),
			ped = cms.untracked.vdouble(14.5833, 14.6036, 12.9906, 14.6842)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_RPD7'),
			ped = cms.untracked.vdouble(14.7257, 14.8689, 14.9295, 14.7159)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_RPD8'),
			ped = cms.untracked.vdouble(8.22027, 11.5532, 8.3668, 12.5143)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_RPD9'),
			ped = cms.untracked.vdouble(12.0765, 10.8223, 11.1155, 9.72326)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_RPD10'),
			ped = cms.untracked.vdouble(14.5587, 16.2962, 17.6888, 16.0731)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_RPD11'),
			ped = cms.untracked.vdouble(14.6351, 11.0675, 11.5787, 11.4281)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_RPD12'),
			ped = cms.untracked.vdouble(13.1163, 11.9869, 11.3521, 12.7243)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_RPD13'),
			ped = cms.untracked.vdouble(17.3509, 16.9257, 17.6498, 15.9125)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_RPD14'),
			ped = cms.untracked.vdouble(17.9158, 15.1911, 17.6415, 17.8971)
		),
		cms.PSet(
			object = cms.untracked.string('hZDCP_RPD15'),
			ped = cms.untracked.vdouble(14.6875, 15.7009, 14.0207, 16.6754)
		)
	)
