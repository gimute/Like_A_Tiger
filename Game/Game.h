#pragma once
class Game : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	/// <summary>
	/// エフェクトの準備
	/// 主にEffectEngineへの登録
	/// </summary>
	void InitEffect();

	/// <summary>
	/// サウンドの準備
	/// 主にSoundEngineへの登録
	/// </summary>
	void InitSound();

	ModelRender m_testmodel;
	AnimationClip m_anim[2];

	std::shared_ptr<Effect> testeffect;

	Bone* m_effectBone = nullptr;
};

