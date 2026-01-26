#pragma once

//エフェクトを楽に使えるようにするためのエフェクト管理クラス
//サウンドマネージャーをほぼ丸パクリで作ったが、サウンドと違ってエフェクトはこまめに座標を更新することがあったり
//IGameObjectを継承していないのでshared_ptrでやったり、正直もっとエフェクト用に調整したほうが良い気がするけどわかんね

//エフェクト用ハンドル名
using EffectHandle = uint32_t;
//ハンドル無効値
static constexpr EffectHandle INVALID_EFFECT_HANDLE = 0xffffffff;

class EffectManager
{
private:
	EffectManager();
	~EffectManager();

	//エフェクト用
	std::map<EffectHandle, std::shared_ptr<Effect>> m_effectList;

	//マップで参照する用にハンドル数を保持
	EffectHandle m_effectHandleCount = 0;

public:
	//更新処理
	void Update();

	/// <summary>
	/// エフェクトの再生
	/// </summary>
	/// <param name="kind">再生するエフェクトの番号</param>
	/// <param name="pos">座標</param>
	/// <param name="scale">拡大率</param>
	/// <param name="rot">回転</param>
	/// <returns>ハンドル</returns>
	EffectHandle PlayEffect(const int kind = false, Vector3 pos = Vector3::Zero, Vector3 scale = Vector3::One, Quaternion rot = Quaternion::Identity);
	//エフェクト停止
	void StopEffect(const EffectHandle handle);

	std::shared_ptr<Effect> FindEffect(const EffectHandle handle)
	{
		auto it = m_effectList.find(handle);
		if (it != m_effectList.end())
		{
			return it->second;
		}

		K2_ASSERT(false, "削除済みか追加されていないエフェクトにアクセスしようとしています。\n");
		return nullptr;
	}

	//シングルトン用
private:
	//シングルトンインスタンス
	static EffectManager* m_instance;

public:
	//インスタンス生成
	static void CreateInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new EffectManager();
		}
	}

	//インスタンスを取得
	static EffectManager& Get()
	{
		return *m_instance;
	}

	//インスタンスを破棄
	static void DestroyInstance()
	{
		if (m_instance != nullptr)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}
};

