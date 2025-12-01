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
	void RoleDetermination(EnemyAiInfoGroupe* groupePtr) override;
	//Às‚Å‚«‚é‚©‚Ç‚¤‚©
	bool IsMetaAiProces(EnemyAiInfoGroupe* groupePtr) override;
};

