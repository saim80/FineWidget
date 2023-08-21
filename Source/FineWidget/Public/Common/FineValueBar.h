// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/FineBaseWidget.h"
#include "UObject/Object.h"
#include "FineValueBar.generated.h"

class UImage;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FINEWIDGET_API UFineValueBar : public UFineBaseWidget
{
	GENERATED_BODY()

public:
	UFineValueBar(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	virtual void SetPercent(float InPercent, bool bAnimate = false);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const float& GetPercent() const { return Percent; }

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateBarImage();

	FORCEINLINE UImage* GetBarImage() const { return BarImage; }

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true", BindWidget))
	UImage* BarImage;

	// ------------------------------
	// Animation
	// ------------------------------
	float Percent;
	FTimerHandle AnimationTimerHandle;

	float SourcePercent;
	float TargetPercent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float AnimationInterval;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float AnimationDuration;
	float AnimationElapsed;

	void BeginAnimation(const float& OldPercent, const float& NewPercent);
	void EndAnimation();
	bool IsAnimationEnded() const;
};
