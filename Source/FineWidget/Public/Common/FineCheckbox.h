// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "FineBaseWidget.h"
#include "Components/CheckBox.h"
#include "UObject/Object.h"
#include "FineCheckbox.generated.h"

/**
 * A wrapper for a checkbox widget to expose event callbacks.
 */
UCLASS(Blueprintable, BlueprintType)
class FINEWIDGET_API UFineCheckbox : public UFineBaseWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnCheckBoxComponentStateChanged OnCheckStateChanged;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(BlueprintReadOnly, Category="FineWidget", meta = (BindWidget, AllowPrivateAccess = true))
	UCheckBox* InnerCheckbox;

	UFUNCTION()
	void InternalOnCheckStateChanged(bool bIsChecked);
};
