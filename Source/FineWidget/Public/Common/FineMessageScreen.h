// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "FineBaseWidget.h"
#include "UObject/Object.h"
#include "FineMessageScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllMessagesDismissed);

USTRUCT(BlueprintType)
struct FFineMessageQueueItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Message")
	FText Message;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Message")
	float Duration = 2.f;
};

class UOverlay;
class UFineMessageBanner;
/**
 * 
 */
UCLASS()
class FINEWIDGET_API UFineMessageScreen : public UFineBaseWidget
{
	GENERATED_BODY()

public:
	UFineMessageScreen(const FObjectInitializer& ObjectInitializer);

	void ShowMessage(const FText NewMessage, const float NewDuration = 2.f);

	UPROPERTY(BlueprintAssignable)
	FOnAllMessagesDismissed OnAllMessagesDismissed;
protected:
	UFUNCTION()
	void OnBannerDismissed();
	
	UFineMessageBanner* DequeueBanner();
	void EnqueueBanner(UFineMessageBanner* Banner);

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Message", meta = (AllowPrivateAccess = true))
	TArray<FFineMessageQueueItem> MessageQueue;

	UPROPERTY()
	TArray<UFineMessageBanner*> FreeBanners;
	UPROPERTY(BlueprintReadOnly, Category = "Message", meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<UFineMessageBanner> CurrentBanner;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Message", meta = (AllowPrivateAccess = true))
	TSubclassOf<UFineMessageBanner> BannerClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Message", meta = (AllowPrivateAccess = true, BindWidget))
	UOverlay* BannerOverlay;
};
