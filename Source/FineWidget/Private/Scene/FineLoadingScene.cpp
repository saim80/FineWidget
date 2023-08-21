// (c) 2023 Pururum LLC. All rights reserved.


#include "Scene/FineLoadingScene.h"

#include "FineWidgetScreen.h"

AFineLoadingScene::AFineLoadingScene(): Super()
{
	LoadingScreen = CreateDefaultSubobject<UFineWidgetScreen>(TEXT("LoadingScreen"));
	LoadingScreen->SetAutoActivate(false);
}

void AFineLoadingScene::OnLoadingStarted_Implementation()
{
	Super::OnLoadingStarted_Implementation();
	LoadingScreen->SetActive(true, true);
}

void AFineLoadingScene::OnLoadingFinished_Implementation()
{
	Super::OnLoadingFinished_Implementation();
	LoadingScreen->SetActive(false);
}
