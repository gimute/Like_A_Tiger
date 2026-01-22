#include "stdafx.h"
#include "Player.h"
#include "Actor\Player\PlayerAttackComboState.h"
#include "Actor\YakuzaComponents\YakuzaType.h"
#include "Actor\YakuzaComponents\YakuzaTypeSetFactory.h"
#include "Actor\YakuzaComponents\YakuzaCharacterDamageManager.h"
#include "GameScene\UpdateOrder.h"

//スタート関数
bool Player::Start()
{
	//タイプセットからプレイヤーの物を取得
	auto typeSet = YakuzaTypeSetFactory::GetInstance().Create(OthersYakuzaType::en_playerYakuza);
	//タイプセットを使用してプレイヤーの攻撃ステートマシンを初期化
	GetYakuzaStateMachine().InitAttackStateMachine(typeSet.get()->GetFirstAttackID(), typeSet.get()->GetFirstFinishBrowID());
	//タイプセットを利用して初期化
	InitYakuzaModel(typeSet.get()->GetModelFilePath(), typeSet.get()->GetAnimationDataList());
	//初期化した内容でモデルを初期化
	InitAnimationClipList(m_maxAnimationNum, m_animationData.data());
	//TypeSet攻撃ステートリスト作成
	typeSet.get()->CreateActions(GetYakuzaStateMachine().GetAttackStateMachine());
	//タイプセットをステートマシンに登録
	GetYakuzaStateMachine().SetTypeSet(std::move(typeSet));
	//ステートマシンのパラメータを初期化
	GetYakuzaStateMachine().InitStateMachineParam();

	//モデルレンダー初期化
	InitModelRender(m_modelFilePath);

	//ダメージ判定登録
	YakuzaCharacterDamageManager::GetInstance()->SetSendDamagePlayer(this);

	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) { GetYakuzaStateMachine().OnAnimationEvent(clipName, eventName); });

	m_characterController.Init(10.0f, 40.0f, m_position);

	InitBodyCollision(this, "playerBodyCollision");

	SetAttackCollisionName("PlayerAttack");

	SetGrabCollisionName("GrabPlayer");

	//HPゲージ
	m_hpGauge = NewGO<HPGauge>(UpdateOrder::UI, "UI");

	m_hpGauge->Init();

	m_hpGauge->SetVisible(true);

	m_hpGauge->SetPosition(Vector3{ -750,400,0.0 });

	m_hpGauge->SetMaxHP(GetYakuzaMaxHp());

	m_hpGauge->SetHP(GetYakuzaMaxHp());
	
	return true;
}

//アップデート関数
void Player::Update()
{
	//ステートマシン更新
	GetYakuzaStateMachine().UpdateStateMachine();

	m_characterController.SetPosition(m_position);

	GetModelRender()->SetPosition(m_position);
	GetModelRender()->SetRotation(m_rotation);
	GetModelRender()->Update();

	//当たり判定の位置調整
	Vector3 positionCorrection = GetPosition();
	positionCorrection.y += 60.0f;

	m_bodyCollision->SetPosition(positionCorrection);

	//無敵時間のタイマー
	UpdateInvincibleTime();
}

//描画関数
void Player::Render(RenderContext& rc)
{
	GetModelRender()->Draw(rc);
}

void Player::OnHit(const char* hitCollisionName, CollisionObject* pairCollision)
{
	if (hitCollisionName == "enemyBodyCollision" &&
		pairCollision == m_attackCollision)
	{
		DeleteAttackCollision();
	}
	
	if (hitCollisionName == "enemyBodyCollision" &&
		pairCollision == m_grabCollision)
	{
		DeleteGrabCollision();
	}
}

void Player::YakuzaCharacterDeadProcces()
{
	m_isDeadProccesEnd = true;
}