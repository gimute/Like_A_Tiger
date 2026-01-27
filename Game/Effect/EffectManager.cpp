#include "stdafx.h"
#include "EffectManager.h"

EffectManager* EffectManager::m_instance = nullptr; //初期化

EffectManager::EffectManager()
{
	m_effectList.clear();
}

EffectManager::~EffectManager()
{
}

void EffectManager::Update()
{
	//エフェクトリストから再生していないものがあれば削除する
	std::vector<EffectHandle> eraseList;
	for (auto& it : m_effectList)
	{
		const auto key = it.first;
		auto effect = it.second;

		//再生が終わっているなら削除
		if (!effect->IsPlay())
		{
			eraseList.push_back(key);
		}
	}

	for (const auto& key : eraseList)
	{
		m_effectList.erase(key);
	}

	//エフェクトの更新
	for (auto effect : m_effectList)
	{
		effect.second->Update();
	}
}

EffectHandle EffectManager::PlayEffect(const int kind, Vector3 pos, Vector3 scale, Quaternion rot)
{
	//ハンドルが最大数になったら使えない
	if (m_effectHandleCount == INVALID_EFFECT_HANDLE)
	{
		K2_ASSERT(false, "エフェクトの数が多いです。\n");
		return INVALID_EFFECT_HANDLE;
	}

	auto effect = std::make_shared<Effect>();
	effect->Init(kind);
	effect->SetPosition(pos);
	effect->SetScale(scale);
	effect->SetRotation(rot);
	effect->Play();

	m_effectList.emplace(m_effectHandleCount, effect);

	return m_effectHandleCount++;
}

void EffectManager::StopEffect(const EffectHandle handle)
{
	auto effect = FindEffect(handle);
	if (effect == nullptr)
	{
		return;
	}
	effect->Stop();
}
