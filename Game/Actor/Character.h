#pragma once
#include "Actor.h"

class CharacterStatus;

//キャラクターとなる要素を持ったクラス
class Character : public Actor
{
protected:
	//キャラクターコントローラー
	CharacterController m_characterController;

	//アニメーションクリップのリスト
	std::vector<AnimationClip*> m_animationClipList;

public:
	//コンストラクタ
	Character() = default;
	//デストラクタ
	~Character() = default;

	//スタート関数
	virtual bool Start() override;
	//更新関数
	virtual void Update() override;
	//描画関数
	virtual void Render(RenderContext& rc) override;
public:
	//キャラクターのステータス取得関数
	CharacterStatus* GetStatus()
	{
		return dynamic_cast<CharacterStatus*>(m_actorStatus);
	}

	//当たり判定チェック用関数
	CharacterController* GetCharacterController()
	{
		return &m_characterController;
	}
};

