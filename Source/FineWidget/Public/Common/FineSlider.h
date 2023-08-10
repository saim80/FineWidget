// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "FineBaseWidget.h"
#include "Components/Slider.h"
#include "UObject/Object.h"
#include "FineSlider.generated.h"

/**
 * Slider widget wrapper to expose event callbacks.
 */
UCLASS(Blueprintable, BlueprintType)
class FINEWIDGET_API UFineSlider : public UFineBaseWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnFloatValueChangedEvent OnValueChanged;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(BlueprintReadOnly, Category="FineWidget", meta = (BindWidget, AllowPrivateAccess = true))
	USlider* InnerSlider;

	UFUNCTION()
	void InternalOnValueChanged(float Value);
};
