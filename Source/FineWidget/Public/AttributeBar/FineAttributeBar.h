// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Common/FineValueBar.h"
#include "UObject/Object.h"
#include "FineAttributeBar.generated.h"

class UTextBlock;
enum class EFineAttributeType : uint8;
class UFineAttributeIcon;
class UFineCharacterGameplay;

/**
 * 
 */
UCLASS()
class FINEWIDGET_API UFineAttributeBar : public UFineValueBar
{
	GENERATED_BODY()

public:
	UFineAttributeBar(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetStatType(EFineAttributeType InStatType);

	UFUNCTION(BlueprintCallable)
	void SetCharacterGameplay(UFineCharacterGameplay* InCharacterGameplay);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	void UpdateAfterStatTypeChanged() const;

	void OnStatValueChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void BeginTrackingValueChanges();
	void EndTrackingValueChanges();

private:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	EFineAttributeType StatType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UTexture2D> HealthBarImage;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UTexture2D> ManaBarImage;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UTexture2D> StaminaBarImage;

	UPROPERTY(BlueprintReadWrite, Category = "StatBar", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UFineCharacterGameplay> CharacterGameplay;
	FDelegateHandle DelegateHandle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true", BindWidget))
	UTextBlock* StatCurrentValueBlock;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true", BindWidget))
	UTextBlock* StatMaxValueBlock;

	FOnGameplayAttributeValueChange* GetAttributeChangeDelegate() const;
	
	void InitializePercent();
	void UpdateStatValueTextBlock() const;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true", BindWidget))
	UFineAttributeIcon* StatIcon;

	void TryShowingIconShakeAnimation() const;
};
