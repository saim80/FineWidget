// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/FineBaseWidget.h"
#include "Components/PanelWidget.h"
#include "UObject/Object.h"
#include "FineTabBar.generated.h"

class UFineTabButton;
class UPanelWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTabBarSelectionChanged, int32, NewIndex);

class UHorizontalBox;
/**
 * Tab bar is a container of UFineTabButton instances. The buttons instances are laid out in horizontal box.
 */
UCLASS(Blueprintable, BlueprintType)
class FINEWIDGET_API UFineTabBar : public UFineBaseWidget
{
	GENERATED_BODY()

public:
	UFineTabBar(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category="TabBar")
	void SetSelectedIndex(const int32& NewIndex);
	FORCEINLINE const int32& GetSelectedIndex() const { return SelectedIndex; }

	UPROPERTY(BlueprintAssignable, Category="TabBar")
	FOnTabBarSelectionChanged OnSelectionChanged;

	UFUNCTION(BlueprintCallable, Category="TabBar")
	void AddButton(UFineTabButton* Button);
	UFUNCTION(BlueprintCallable, Category="TabBar")
	void InsertButtonAt(const int32& Index, UFineTabButton* Button);
	UFUNCTION(BlueprintCallable, Category="TabBar")
	void RemoveButtonAt(const int32& Index);
	UFUNCTION(BlueprintCallable, Category="TabBar")
	void RemoveButton(UFineTabButton* Button);
	UFUNCTION(BlueprintCallable, Category="TabBar")
	void ClearButtons();
	UFUNCTION(BlueprintCallable, Category="TabBar")
	void RefreshButtons(TArray<UFineTabButton*> Buttons);

	FORCEINLINE const int32 GetButtonCount() const { return ButtonContainer->GetChildrenCount(); }
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(BlueprintReadOnly, Category="TabBar", meta = (BindWidget, AllowPrivateAccess = true))
	UPanelWidget* ButtonContainer;

	UPROPERTY(BlueprintReadWrite, BlueprintSetter=SetSelectedIndex, Category="TabBar",
		meta = (AllowPrivateAccess = true))
	int32 SelectedIndex;

	void PrepareButton(UFineTabButton* Button);
	void CleanUpButton(UFineTabButton* Button);
	UFUNCTION()
	void UpdateSelectedTab(UFineTabButton* FineTabButton, bool IsSelected);
};
