// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/FineBaseWidget.h"

#include "FineAttributeIcon.generated.h"

class UImage;
enum class EFineAttributeType : uint8;
/**
 * Show the icon for a stat, health, mana, stamina.
 */
UCLASS(Blueprintable, BlueprintType)
class FINEWIDGET_API UFineAttributeIcon : public UFineBaseWidget
{
	GENERATED_BODY()

public:
	UFineAttributeIcon(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Perilous")
	void SetStatType(EFineAttributeType InStatType);

	UFUNCTION(BlueprintCallable, Category = "Perilous")
	void BeginShaking();
	UFUNCTION(BlueprintCallable, Category = "Perilous")
	void EndShaking();

protected:
	virtual void NativePreConstruct() override;

	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

	UFUNCTION(BlueprintCallable, Category = "Perilous")
	void UpdateIconByStatType();

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintSetter=SetStatType, meta = (AllowPrivateAccess = "true"))
	EFineAttributeType StatType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true", BindWidget))
	UImage* Image;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UTexture2D> HealthIcon;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UTexture2D> ManaIcon;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UTexture2D> StaminaIcon;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, meta = (AllowPrivateAccess = "true", BindWidgetAnim))
	UWidgetAnimation* ShakeAnimation;
};
