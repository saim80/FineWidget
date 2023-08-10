// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/FineBaseWidget.h"
#include "UObject/Object.h"
#include "FineTabNavigation.generated.h"

class UFineContent;
class UWidgetSwitcher;
class UFineTabBar;
/**
 * Tab widget is a container widget that can hold multiple tab content widgets.
 */
UCLASS(Blueprintable, BlueprintType)
class FINEWIDGET_API UFineTabNavigation : public UFineBaseWidget
{
	GENERATED_BODY()

public:
	UFineTabNavigation(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void AddContent(UFineContent* ContentWidget);
	UFUNCTION(BlueprintCallable)
	void InsertContent(const int32& Index, UFineContent* ContentWidget);
	UFUNCTION(BlueprintCallable)
	void RemoveContent(UFineContent* ContentWidget);
	UFUNCTION(BlueprintCallable)
	void RemoveContentAt(int32 Index);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(BlueprintReadOnly, Category="TabNavigation", meta = (BindWidget, AllowPrivateAccess = true))
	UWidgetSwitcher* ContentSwitcher;
	UPROPERTY(BlueprintReadOnly, Category="TabNavigation", meta = (BindWidget, AllowPrivateAccess = true))
	UFineTabBar* TabBar;
	
	UFUNCTION()
	void OnTabSelectionChanged(int32 NewIndex);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="TabNavigation", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UFineContent>> ContentClasses;
};
