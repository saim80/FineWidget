// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "UObject/Object.h"
#include "FineFlipbookImage.generated.h"

class UPaperFlipbook;
/**
 * Image that supports rendering flipbook animation.
 */
UCLASS()
class FINEWIDGET_API UFineFlipbookImage : public UImage
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetFlipbook(UPaperFlipbook* NewFlipbook);

	UFUNCTION(BlueprintCallable)
	void Reset();

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	UPROPERTY(BlueprintReadWrite, BlueprintSetter=SetFlipbook, EditAnywhere, Category = "Appearance",
		meta = (AllowPrivateAccess = "true"))
	UPaperFlipbook* Flipbook = nullptr;

	FTimerHandle AnimTimerHandle;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	bool bFlipbookLoop = true;
	int32 CurrentFrameIndex = 0;

	void UpdateAnimation();
};
