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
	register_number,
};

enum BGMid
{
	bgm_Title = SoundId::register_number,
	bgm_InGame_normal,
	bgm_inGame_Explore,
	bgm_inGame_battle,
	bgm_Result,
	bgm_GameOver,
};

