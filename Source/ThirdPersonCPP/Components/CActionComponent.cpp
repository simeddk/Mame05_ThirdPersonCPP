#include "CActionComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Actions/CActionData.h"
#include "Actions/CActionData_Spawned.h"
#include "Actions/CEquipment.h"
#include "Actions/CAttachment.h"
#include "Actions/CDoAction.h"


UCActionComponent::UCActionComponent()
{

}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* ownerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(ownerCharacter);

	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!DataAssets[i])
			DataAssets[i]->BeginPlay(ownerCharacter, &Datas[i]);
	}
}

void UCActionComponent::SetUnaremdMode()
{
	if (!!Datas[(int32)Type] && !!Datas[(int32)Type]->GetEquipment())
		Datas[(int32)Type]->GetEquipment()->Unequip();
	
	if (!!Datas[(int32)EActionType::Unarmed] && !!Datas[(int32)EActionType::Unarmed]->GetEquipment())
		Datas[(int32)EActionType::Unarmed]->GetEquipment()->Equip();

	ChangeType(EActionType::Unarmed);
}

void UCActionComponent::SetFistMode()
{
	SetMode(EActionType::Fist);
}

void UCActionComponent::SetOneHandMode()
{
	SetMode(EActionType::OneHand);
}

void UCActionComponent::SetTwoHandMode()
{
	SetMode(EActionType::TwoHand);
}

void UCActionComponent::SetWarpMode()
{
	SetMode(EActionType::Warp);
}

void UCActionComponent::SetMagicBallMode()
{
	SetMode(EActionType::MagicBall);
}

void UCActionComponent::SetTornadoMode()
{
	SetMode(EActionType::Tornado);
}

void UCActionComponent::OffAllCollisions()
{
	for (UCActionData_Spawned* data : Datas)
	{
		if (!!data == false)
			continue;

		if (!!data->GetAttachment() == false)
			continue;
		
		data->GetAttachment()->OffCollision();
	}
}

void UCActionComponent::End_Dead()
{
	for (int32 i  = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!Datas[i] && Datas[i]->GetAttachment())
			Datas[i]->GetAttachment()->Destroy();
		
		if (!!Datas[i] && Datas[i]->GetEquipment())
			Datas[i]->GetEquipment()->Destroy();

		if (!!Datas[i] && Datas[i]->GetDoAction())
			Datas[i]->GetDoAction()->Destroy();
	}
}

void UCActionComponent::AbortedByDamaged()
{
	CheckNull(Datas[(int32)Type]);
	CheckNull(Datas[(int32)Type]->GetEquipment());

	Datas[(int32)Type]->GetEquipment()->Begin_Equip();
	Datas[(int32)Type]->GetEquipment()->End_Equip();

	CheckNull(Datas[(int32)Type]->GetDoAction());
	Datas[(int32)Type]->GetDoAction()->Abort();
}

void UCActionComponent::SetMode(EActionType InType)
{
	if (Type == InType)
	{
		SetUnaremdMode();
		return;
	}
	else if (IsUnarmedMode() == false)
	{
		if (!!Datas[(int32)Type] && !!Datas[(int32)Type]->GetEquipment())
			Datas[(int32)Type]->GetEquipment()->Unequip();
	}

	if (!!Datas[(int32)InType] && !!Datas[(int32)InType]->GetEquipment())
		Datas[(int32)InType]->GetEquipment()->Equip();

	ChangeType(InType);
}

void UCActionComponent::ChangeType(EActionType InNewType)
{
	EActionType prev = Type;
	Type = InNewType;

	if (OnActionTypeChanged.IsBound())
		OnActionTypeChanged.Broadcast(prev, InNewType);
}

void UCActionComponent::DoAction()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type] && !!Datas[(int32)Type]->GetDoAction())
	{
		Datas[(int32)Type]->GetDoAction()->DoAction();
	}
}

void UCActionComponent::DoSubAction(bool bEnable)
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type] && !!Datas[(int32)Type]->GetDoAction())
	{
		bEnable
			? Datas[(int32)Type]->GetDoAction()->BeginSubAction()
			: Datas[(int32)Type]->GetDoAction()->EndSubAction();
	}
}
