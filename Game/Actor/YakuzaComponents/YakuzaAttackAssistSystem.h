#pragma once
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"

//シングルトンパターンで作成

//ターゲット条件まとめ構造体
struct TargetingParam
{
	//最大補正距離
	float m_maxDistance = 0.0f;
	//視野角(cos値)
	float m_fovCos = 0.0f;
	//視野角を考慮するか？
	bool m_isUseFov = true;
	//距離優先の重み(1に近いほど近くにいる敵を選定)
	float m_distanceWeight = 0.0f;
	//前方優先の重み(1に近いほど真正面にいる敵を選定)
	float m_forwardWeight = 0.0f;
	//座標
	Vector3 m_yakuzaPos = Vector3::Zero;
	//正面値
	Vector3 m_yakuzaForward = Vector3::Zero;
	//陣営設定
	YakuzaCamp m_yakuzaCamp = en_campNone;

	//コンストラクタ
	TargetingParam() {};
	//引数付きコンストラクタ
	TargetingParam(
		float maxDistance,
		float fovCos,
		float distanceWeight,
		float forwardWeight,
		Vector3 pos,
		Vector3 foward,
		YakuzaCamp yakuzaCamp,
		bool isUseFov = true
	) : m_maxDistance(maxDistance),
		m_fovCos(fovCos),
		m_distanceWeight(distanceWeight),
		m_forwardWeight(forwardWeight),
		m_yakuzaPos(pos),
		m_yakuzaForward(foward),
		m_yakuzaCamp(yakuzaCamp),
		m_isUseFov(isUseFov)
	{}
};

class YakuzaCharacter;

class YakuzaAttackAssistSystem
{
private:
	//シングルトーンパターンを採用するため、コンストラクタをprivateにする
	//インスタンス
	static YakuzaAttackAssistSystem* m_instance;
	//コンストラクタ
	YakuzaAttackAssistSystem() = default;
	//コピー禁止
	YakuzaAttackAssistSystem(const YakuzaAttackAssistSystem&) = delete;
	//代入禁止にする
	YakuzaAttackAssistSystem& operator=(const YakuzaAttackAssistSystem) = delete;
public:
	//デストラクタ
	~YakuzaAttackAssistSystem() = default;
	//インスタンス取得
	static YakuzaAttackAssistSystem* GetIstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new YakuzaAttackAssistSystem();
		}

		return m_instance;
	}
	//初期化
	void InitAttackAssistSystem(YakuzaCharacter* playerYakuzaCharacter);
	//解除
	void RemoveAttackAssistSystem();

	//近くのヤクザのポインタを取得(陣営によって処理が変わる)
	YakuzaCharacter* GetNearYakuza(const TargetingParam& param);

	//敵から一番近いプレイヤー座標を取得する
	YakuzaCharacter* GetPlayerYakuzaPointer(const TargetingParam& param);
	//プレイヤーから一番近い敵座標を取得する
	YakuzaCharacter* GetEnemyYakuzaPointer(const TargetingParam& param);
private:
	//プレイヤーのYakuzaCharacterのポインタ格納変数
	YakuzaCharacter* m_playerYakuzaCharacterPtr = nullptr;

};

