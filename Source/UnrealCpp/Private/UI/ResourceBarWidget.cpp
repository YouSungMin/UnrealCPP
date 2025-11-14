// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ResourceBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UResourceBarWidget::RefreshWidget(float InCurrent, float InMax)
{
	Bar->SetPercent(InCurrent/InMax);
	Current->SetText(FText::AsNumber(FMath::FloorToInt(InCurrent)));
	Max->SetText(FText::AsNumber(FMath::FloorToInt(InMax)));
}

#if WITH_EDITOR
// UObject 멤버 변수에 변화가 있으면 자동 실행되는 함수
void UResourceBarWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChagedEvent)
{
	Super::PostEditChangeProperty(PropertyChagedEvent);

	FName propertyName = (PropertyChagedEvent.Property != nullptr) ? PropertyChagedEvent.Property->GetFName() : NAME_None;
	//UResourceBarWidget의 FillColor가 변경되었을 때만 처리
	if (propertyName == GET_MEMBER_NAME_CHECKED(UResourceBarWidget, BarFillColor))
	{
		BarBackgroundFillColor = BarFillColor; //Background를 FillColor와 같은 색으로 변경하고 알파값만 0.2로 설정
		BarBackgroundFillColor.A = 0.2f;
	}
}
#endif

void UResourceBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	Bar->SetFillColorAndOpacity(BarFillColor);
	FProgressBarStyle style = Bar->GetWidgetStyle();
	style.BackgroundImage.TintColor = BarBackgroundFillColor;
	Bar->SetWidgetStyle(style);
}
