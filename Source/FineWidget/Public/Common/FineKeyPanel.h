// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "FineBaseWidget.h"
#include "InputCoreTypes.h"
#include "UObject/Object.h"
#include "FineKeyPanel.generated.h"

class USpacer;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class FINEWIDGET_API UFineKeyPanel : public UFineBaseWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetKey(const FKey& NewKey, const bool& RequiresHold);
	FORCEINLINE const FKey& GetKey() const { return Key; }

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	virtual void UpdateKeyPresentation();

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FineKeyPanel", meta = (AllowPrivateAccess = "true"))
	FKey Key;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FineKeyPanel", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UTexture2D> KeyIconTexture;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FineKeyPanel", meta = (AllowPrivateAccess = "true"))
	FText KeyTextValue;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FineKeyPanel",
		meta = (AllowPrivateAccess = "true", BindWidgetOptional))
	UTextBlock* KeyText;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FineKeyPanel",
		meta = (AllowPrivateAccess = "true", BindWidgetOptional))
	USpacer* KeySpacer;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FineKeyPanel",
		meta = (AllowPrivateAccess = "true", BindWidgetOptional))
	UImage* KeyImage;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FineKeyPanel",
		meta = (AllowPrivateAccess = "true", BindWidgetOptional))
	UImage* HoldImage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FineKeyPanel",
		meta = (AllowPrivateAccess = "true", BindWidgetOptional))
	bool bRequiresHold = false;
};
