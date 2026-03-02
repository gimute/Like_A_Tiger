#pragma once

enum SoundId
{
	//ƒpƒ“ƒ`‰¹
	se_hittingLightA,
	se_hittingLightB,
	se_hittingHeavyA,
	se_hittingHeavyB,
	se_hittingDefenseA,
	//•—Ø‚è‰¹
	se_cuttingWindLigthA,
	se_cuttingWindHeavyA,
	//“®ì‰¹
	se_FootstepsA,
	se_kickingGroundA,
	se_GroundFrictionA,
	se_ChargeA,
	se_ChargeB,
	se_BigWindA,
	register_number,
};

enum BGMid
{
	bgm_inGame_Explore = SoundId::register_number,
	bgm_inGame_battle,
	bgm_inGame_battleBossHirota,
};

