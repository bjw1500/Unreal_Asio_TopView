// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/CharacterBase.h"
#include "Interaction/ObjectInterface.h"
#include "Monster.generated.h"


UCLASS()
class MMORPG2_API AMonster : public ACharacterBase, public IObjectInterface
{
	GENERATED_BODY()

public:
	AMonster();

	virtual void BeginPlay() override;
	


	public:
	//인터페이스
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

};
