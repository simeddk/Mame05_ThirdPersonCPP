#include "CHUD.h"
#include "Global.h"
#include "Engine/Canvas.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Engine/Texture2D.h"

ACHUD::ACHUD()
{
	CHelpers::GetAsset<UTexture2D>(&CrossHairTexture, "Texture2D'/Game/Materials/T_Crosshair.T_Crosshair'");
}

void ACHUD::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	CheckNull(playerCharacter);

	StateComp = CHelpers::GetComponent<UCStateComponent>(playerCharacter);
	StateTypeAsUEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EStateType"), true);
}

void ACHUD::DrawHUD()
{
	Super::DrawHUD();

	//Visible Player StateType
	CheckNull(StateComp);
	CheckNull(StateTypeAsUEnum);

	FString typeStr = StateTypeAsUEnum->GetNameStringByValue((int64)StateComp->GetType());
	DrawText("Current State : " + typeStr, FLinearColor::Red, 10, Canvas->ClipY - 50, nullptr, 2.f);

	typeStr = StateTypeAsUEnum->GetNameStringByValue((int64)StateComp->GetPrevType());
	DrawText("Previous State : " + typeStr, FLinearColor::Green, 10, Canvas->ClipY - 75, nullptr, 2.f);

	//Visible Aim(R-Button)
	CheckNull(CrossHairTexture);
	CheckFalse(bVisbleCrossHair);

	FVector2D center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	FVector2D imageHalfSize(CrossHairTexture->GetSizeX() * 0.5f, CrossHairTexture->GetSizeY() * 0.5f);
	center -= imageHalfSize;

	FCanvasTileItem imageItem(center, CrossHairTexture->Resource, FLinearColor::White);
	imageItem.BlendMode = ESimpleElementBlendMode::SE_BLEND_Translucent;
	Canvas->DrawItem(imageItem);
}