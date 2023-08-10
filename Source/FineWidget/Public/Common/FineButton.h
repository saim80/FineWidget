// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UObject/Object.h"
#include "FineButton.generated.h"

/**
 * The base class to wrap a button for override additional style in blueprint.
 */
UCLASS(Blueprintable, BlueprintType)
class FINEWIDGET_API UFineButton : public UUserWidget
{
	GENERATED_BODY()

public:
	/// Event relay to InnerButton.
	UPROPERTY(BlueprintAssignable)
	FOnButtonClickedEvent OnClicked;
	/// Event relay to InnerButton.
	UPROPERTY(BlueprintAssignable)
	FOnButtonPressedEvent OnPressed;
	/// Event relay to InnerButton.
	UPROPERTY(BlueprintAssignable)
	FOnButtonReleasedEvent OnReleased;
	/// Event relay to InnerButton.
	UPROPERTY(BlueprintAssignable)
	FOnButtonHoverEvent OnHovered;
	/// Event relay to InnerButton.
	UPROPERTY(BlueprintAssignable)
	FOnButtonHoverEvent OnUnhovered;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	FORCEINLINE UButton* GetInnerButton() const { return InnerButton; }
private:
	UPROPERTY(BlueprintReadOnly, Category="FineWidget", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* InnerButton;

	UFUNCTION(meta = (AllowPrivateAccess = true))
	void InternalOnClicked();
	UFUNCTION(meta = (AllowPrivateAccess = true))
	void InternalOnPressed();
	UFUNCTION(meta = (AllowPrivateAccess = true))
	void InternalOnReleased();
	UFUNCTION(meta = (AllowPrivateAccess = true))
	void InternalOnHovered();
	UFUNCTION(meta = (AllowPrivateAccess = true))
	void InternalOnUnhovered();
};
