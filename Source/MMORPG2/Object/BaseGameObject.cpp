// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/BaseGameObject.h"

// Sets default values
ABaseGameObject::ABaseGameObject()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseGameObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseGameObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseGameObject::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

