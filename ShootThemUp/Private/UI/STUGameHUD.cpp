// Created by pyskonus.


#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"

void ASTUGameHUD::DrawHUD() {
  Super::DrawHUD();

  DrawCrosshair();
}

void ASTUGameHUD::DrawCrosshair() {
  const TInterval<float> Center(Canvas->SizeX * 0.5, Canvas->SizeY * 0.5);
  
  const float HalfLineSize = 10.0f;
  const float LineThickness = 2.0f;
  const FLinearColor LineColor = FLinearColor::Green;

  DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
  DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}