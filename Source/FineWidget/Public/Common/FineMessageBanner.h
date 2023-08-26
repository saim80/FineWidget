// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "FineBaseWidget.h"
#include "UObject/Object.h"
#include "FineMessageBanner.generated.h"

class UButton;
class URichTextBlock;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBannerDismissed);

/**
 * 
 */
UCLASS()
class FINEWIDGET_API UFineMessageBanner : public UFineBaseWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetMessage(const FText NewText, const float NewDuration = 2.f);
	FORCEINLINE FText GetMessage() const;

	UFUNCTION(BlueprintCallable)
	void Dismiss();
	UFUNCTION(BlueprintCallable)
	void ClearMessage();

	UPROPERTY(BlueprintAssignable)
	FOnBannerDismissed OnBannerDismissed;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Message", meta = (AllowPrivateAccess = true, BindWidget))
	UButton* DismissButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Message", meta = (AllowPrivateAccess = true, BindWidget))
	URichTextBlock* MessageBlock;

	FTimerHandle DismissTimerHandle;
};
