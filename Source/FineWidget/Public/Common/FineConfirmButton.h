// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/FineBaseWidget.h"
#include "UObject/Object.h"
#include "InputCoreTypes.h"
#include "FineConfirmButton.generated.h"

class UButton;

UENUM()
enum class EPerilousConfirmMethod: uint8
{
	Click,
	Hold
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionConfirmed);

/**
 * Shows a UI with a button to confirm an interaction.
 */
UCLASS(Blueprintable, BlueprintType)
class FINEWIDGET_API UFineConfirmButton : public UFineBaseWidget
{
	GENERATED_BODY()

public:
	UFineConfirmButton(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FOnInteractionConfirmed OnInteractionConfirmed;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHoldProgress() const { return ElapsedTime / HoldingTime; }

protected:
	virtual void NativePreConstruct() override;

	UFUNCTION()
	void OnConfirmButtonClicked();
	void OnConfirmUpdate();
	UFUNCTION()
	void OnConfirmButtonPressed();
	UFUNCTION()
	void OnConfirmButtonReleased();

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "PerilousInteractionConfirmWidget",
		meta = (AllowPrivateAccess = "true"))
	EPerilousConfirmMethod ConfirmMethod;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "PerilousInteractionConfirmWidget",
		meta = (AllowPrivateAccess = "true", BindWidget))
	UButton* ConfirmButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "PerilousInteractionConfirmWidget",
		meta = (AllowPrivateAccess = "true"))
	float HoldingTime = 1.0f;
	bool bHeld = false;
	float ElapsedTime = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "PerilousInteractionConfirmWidget",
		meta = (AllowPrivateAccess = "true"))
	float UpdateInterval = 1.f / 30.f;

	FTimerHandle HoldTimerHandle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "PerilousInteractionConfirmWidget",
		meta = (AllowPrivateAccess = "true"))
	FKey ConfirmPadKey = EKeys::Gamepad_FaceButton_Bottom;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "PerilousInteractionConfirmWidget",
		meta = (AllowPrivateAccess = "true"))
	FKey ConfirmKey = EKeys::E;

	FDateTime LastKeyPressed;
	float ClickTimeout = 0.2f;

};
