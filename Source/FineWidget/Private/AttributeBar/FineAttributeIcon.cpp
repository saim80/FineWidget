// (c) 2023 Pururum LLC. All rights reserved.


#include "AttributeBar/FineAttributeIcon.h"
#include "AttributeBar/FineAttributeType.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"

UFineAttributeIcon::UFineAttributeIcon(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UFineAttributeIcon::SetStatType(EFineAttributeType InStatType)
{
	if (StatType != InStatType)
	{
		StatType = InStatType;
		UpdateIconByStatType();
	}
}

void UFineAttributeIcon::BeginShaking()
{
	// if not animating shake animation already, play shake animation.
	if (!IsAnimationPlaying(ShakeAnimation))
	{
		PlayAnimation(ShakeAnimation, 0.f, 0, EUMGSequencePlayMode::Forward, 1.f);
	}
}

void UFineAttributeIcon::EndShaking()
{
	// if animating shake animation, stop it.
	if (IsAnimationPlaying(ShakeAnimation))
	{
		StopAnimation(ShakeAnimation);
	}
}

void UFineAttributeIcon::NativePreConstruct()
{
	Super::NativePreConstruct();

	UpdateIconByStatType();
}

void UFineAttributeIcon::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	if (Animation == ShakeAnimation)
	{
		// Reset Transform for the icon image.
		Image->SetRenderTransform({});
		// Reset render opacity for the icon image.
		Image->SetRenderOpacity(1.f);
	}
}

void UFineAttributeIcon::UpdateIconByStatType()
{
	TSoftObjectPtr<UTexture2D> IconTexture;
	switch (StatType)
	{
	case EFineAttributeType::Health:
		IconTexture = HealthIcon;
		break;
	case EFineAttributeType::Mana:
		IconTexture = ManaIcon;
		break;
	case EFineAttributeType::Stamina:
		IconTexture = StaminaIcon;
		break;
	default:
		break;
	}
	if (!IconTexture.IsNull())
	{
		Image->SetBrushFromSoftTexture(IconTexture);
	}
}
