// (c) 2023 Pururum LLC. All rights reserved.


#include "AttributeBar/FineAttributeBar.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AttributeBar/FineAttributeType.h"
#include "AttributeBar/FineAttributeIcon.h"
#include "Actor/FineCharacterGameplay.h"
#include "Actor/FineCharacterAttributeSet.h"
#include "Components/Image.h"
#include "FineWidgetLog.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

UFineAttributeBar::UFineAttributeBar(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UFineAttributeBar::SetStatType(EFineAttributeType InStatType)
{
	// if new type is different from current type
	if (StatType != InStatType)
	{
		EndTrackingValueChanges();
		// set new type
		StatType = InStatType;
		// update bar image
		UpdateAfterStatTypeChanged();
		BeginTrackingValueChanges();

		if (StatIcon)
		{
			StatIcon->SetStatType(StatType);
		}

		FW_LOG("StatType changed to %d", (int32)StatType);
	}
}

void UFineAttributeBar::SetCharacterGameplay(UFineCharacterGameplay* InCharacterGameplay)
{
	if (CharacterGameplay == InCharacterGameplay)
	{
		return;
	}
	EndTrackingValueChanges();
	CharacterGameplay = InCharacterGameplay;
	InitializePercent();
	BeginTrackingValueChanges();
}

void UFineAttributeBar::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateAfterStatTypeChanged();
	if (IsValid(StatCurrentValueBlock))
	{
		StatCurrentValueBlock->SetText(FText::FromString(TEXT("0")));
	}
	if (IsValid(StatMaxValueBlock))
	{
		StatMaxValueBlock->SetText(FText::FromString(TEXT("0")));
	}
}

void UFineAttributeBar::NativeConstruct()
{
	Super::NativeConstruct();
	if (CharacterGameplay.IsValid())
	{
		InitializePercent();
	}
}

void UFineAttributeBar::UpdateAfterStatTypeChanged() const
{
	auto BarImage = GetBarImage();
	// early exit if bar image is not valid.
	if (!BarImage)
	{
		return;
	}
	switch (StatType)
	{
	case EFineAttributeType::Health:
		BarImage->SetBrushFromSoftTexture(HealthBarImage);
		break;
	case EFineAttributeType::Mana:
		BarImage->SetBrushFromSoftTexture(ManaBarImage);
		break;
	case EFineAttributeType::Stamina:
		BarImage->SetBrushFromSoftTexture(StaminaBarImage);
		break;
	default:
		break;
	}

	if (StatIcon)
	{
		StatIcon->SetStatType(StatType);
	}
}

void UFineAttributeBar::OnStatValueChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	// early exit if character gameplay is not valid.
	if (!CharacterGameplay.IsValid())
	{
		return;
	}
	float Percent = 0.0f;
	switch (StatType)
	{
	case EFineAttributeType::Health:
		{
			Percent = OnAttributeChangeData.NewValue / CharacterGameplay->GetMaxHealth();
		}
		break;
	case EFineAttributeType::Mana:
		{
			Percent = OnAttributeChangeData.NewValue / CharacterGameplay->GetMaxMana();
		}
		break;
	case EFineAttributeType::Stamina:
		{
			Percent = OnAttributeChangeData.NewValue / CharacterGameplay->GetMaxStamina();
		}
		break;
	default:
		break;
	}
	SetPercent(Percent);
	TryShowingIconShakeAnimation();
	UpdateStatValueTextBlock();
}

void UFineAttributeBar::BeginTrackingValueChanges()
{
	// early exit if character gameplay is not valid.
	if (!CharacterGameplay.IsValid())
	{
		return;
	}
	const auto Delegate = GetAttributeChangeDelegate();
	DelegateHandle = Delegate->AddUObject(this, &UFineAttributeBar::OnStatValueChanged);
	FW_LOG("Begin tracking value changes");
}

void UFineAttributeBar::EndTrackingValueChanges()
{
	// If delegate handle is valid
	if (DelegateHandle.IsValid())
	{
		// get ability system from character gameplay's ability system component
		UAbilitySystemComponent* AbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
			CharacterGameplay->GetOwner());
		// early exit if ability system is not valid.
		if (!IsValid(AbilitySystem))
		{
			return;
		}
		// remove delegate from ability system
		const auto Delegate = GetAttributeChangeDelegate();
		Delegate->Remove(DelegateHandle);
		// invalidate delegate handle
		DelegateHandle.Reset();
		FW_LOG("End tracking value changes");
	}
}

FOnGameplayAttributeValueChange* UFineAttributeBar::GetAttributeChangeDelegate() const
{
	// get ability system from character gameplay's ability system component
	UAbilitySystemComponent* AbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
		CharacterGameplay->GetOwner());
	// early exit if ability system is not valid.
	if (!AbilitySystem)
	{
		return nullptr;
	}
	FOnGameplayAttributeValueChange* Delegate = nullptr;

	switch (StatType)
	{
	case EFineAttributeType::Health:
		Delegate = &AbilitySystem->GetGameplayAttributeValueChangeDelegate(
			UFineCharacterAttributeSet::GetHealthAttribute());
		break;
	case EFineAttributeType::Mana:
		Delegate = &AbilitySystem->GetGameplayAttributeValueChangeDelegate(
			UFineCharacterAttributeSet::GetManaAttribute());
		break;
	case EFineAttributeType::Stamina:
		Delegate = &AbilitySystem->GetGameplayAttributeValueChangeDelegate(
			UFineCharacterAttributeSet::GetStaminaAttribute());
		break;
	default:
		FW_FATAL("Unknown stat type.");
	}
	return Delegate;
}

void UFineAttributeBar::InitializePercent()
{
	// early exit if character gameplay is invalid.
	if (!CharacterGameplay.IsValid())
	{
		return;
	}
	// for each stat update percent from the attribute value.
	switch (StatType)
	{
	case EFineAttributeType::Health:
		SetPercent(CharacterGameplay->GetHealth() / CharacterGameplay->GetMaxHealth());
		break;
	case EFineAttributeType::Mana:
		SetPercent(CharacterGameplay->GetMana() / CharacterGameplay->GetMaxMana());
		break;
	case EFineAttributeType::Stamina:
		SetPercent(CharacterGameplay->GetStamina() / CharacterGameplay->GetMaxStamina());
		break;
	default:
		break;
	}
	TryShowingIconShakeAnimation();
	UpdateStatValueTextBlock();
}

void UFineAttributeBar::UpdateStatValueTextBlock() const
{
	float CurrentValue = 0.0f;
	float MaxValue = 0.0f;
	switch (StatType)
	{
	case EFineAttributeType::Health:
		CurrentValue = CharacterGameplay->GetHealth();
		MaxValue = CharacterGameplay->GetMaxHealth();
		break;
	case EFineAttributeType::Mana:
		CurrentValue = CharacterGameplay->GetMana();
		MaxValue = CharacterGameplay->GetMaxMana();
		break;
	case EFineAttributeType::Stamina:
		CurrentValue = CharacterGameplay->GetStamina();
		MaxValue = CharacterGameplay->GetMaxStamina();
		break;
	default:
		break;
	}

	const FText CurrentText =
		UKismetTextLibrary::Conv_DoubleToText(CurrentValue, HalfToZero, false, true, 1, 324, 0, 0);
	const FText MaxText = UKismetTextLibrary::Conv_DoubleToText(MaxValue, HalfToZero, false, true, 1, 324, 0, 0);

	StatCurrentValueBlock->SetText(CurrentText);
	StatMaxValueBlock->SetText(MaxText);
}

void UFineAttributeBar::TryShowingIconShakeAnimation() const
{
	if (!StatIcon)
	{
		return;
	}
	if (GetPercent() < 0.15f)
	{
		StatIcon->BeginShaking();
	}
	else
	{
		StatIcon->EndShaking();
	}
}
