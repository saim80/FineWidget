// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "FineButton.h"
#include "UObject/Object.h"
#include "FineTabButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTabButtonSelected, class UFineTabButton*, Button, bool, bIsSelected);

/**
 * Tab button has `selected` state in addition to the common button states.
 */
UCLASS(Blueprintable, BlueprintType)
class FINEWIDGET_API UFineTabButton : public UFineButton
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnTabButtonSelected OnTabButtonSelected;

	UFUNCTION(BlueprintCallable, Category="FineWidget")
	void SetSelected(const bool& IsSelected);
	FORCEINLINE const bool& IsSelected() const { return bIsSelected; }

	UFUNCTION(BlueprintCallable, Category="FineWidget")
	void ToggleSelected();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(BlueprintReadOnly, Category="FineWidget", meta = (AllowPrivateAccess = true))
	bool bIsSelected;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="FineWidget", meta = (AllowPrivateAccess = true))
	bool bTogglesOnClick;
};
