#pragma once
#include "Actor\Enemy\EnemyMetaAi\IMetaAiProcess.h"

#include "CRC32.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }


class AttackRoleProcess : public IMetaAiProcess
{
	appState(AttackRoleProcess)
public:
	AttackRoleProcess() = default;
	~AttackRoleProcess() = default;

	//–ğŠ„‚ÌŒˆ’è
	void AssignRoles(MetaAiProccesInfo* groupePtr) override;
	//Às‚Å‚«‚é‚©‚Ç‚¤‚©
	bool IsApplicable(MetaAiProccesInfo* groupePtr) override;
	//ˆ—‚ğ‘±‚¯‚é‚©‚Ç‚¤‚©
	bool IsReady(MetaAiProccesInfo* groupePtr) override;
};

