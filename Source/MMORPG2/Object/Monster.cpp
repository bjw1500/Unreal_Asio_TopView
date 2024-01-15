// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Monster.h"

AMonster::AMonster()
{
	//Collision 설정은 전부 Custom으로 변경
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

}

void AMonster::BeginPlay()
{
	Super::BeginPlay();


}

void AMonster::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	if (IsValid(Weapon) == true)
	{
		Weapon->SetRenderCustomDepth(true);
		Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}

}

void AMonster::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	if (IsValid(Weapon) == true)
	{
		Weapon->SetRenderCustomDepth(false);
	}
}
