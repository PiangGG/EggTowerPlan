// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildTipPlane.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABuildTipPlane::ABuildTipPlane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	Meshs.SetNum(length*length);
	//
	// for (int32 i = 0;i<length*length;++i)
	// {
	// 	Meshs[i] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	// 	if (StaticMesh&&Meshs[i])
	// 	{
	// 		Meshs[i]->SetStaticMesh(StaticMesh);
	// 		Meshs[i]->SetupAttachment(RootSceneComponent);
	// 	}
	// }
}

// Called when the game starts or when spawned
void ABuildTipPlane::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABuildTipPlane::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void ABuildTipPlane::InitUnit()
{
	for (int32 i = 0;i<Maxlength*Maxlength;++i)
	{
		Meshs[i] = NewObject<UStaticMeshComponent>(this,TEXT("Mesh"+i));
		Meshs[i]->RegisterComponent();
		if (StaticMesh&&Meshs[i])
		{
			Meshs[i]->SetStaticMesh(StaticMesh);
			AddInstanceComponent(Meshs[i]);
			Meshs[i]->AttachToComponent(RootSceneComponent,FAttachmentTransformRules::KeepRelativeTransform);
			Meshs[i]->SetRelativeLocation(FVector(0.0,0.0,100.0f));
		}
	}
}

void ABuildTipPlane::ShowUnit(int32 Size)
{
	if (Size*Size>Meshs.Num())
	{
		return;
	}
	double Remainder;
	UKismetMathLibrary::FMod(Size,2,Remainder);
	if (FMath::IsNearlyEqual(0,Remainder))
	{
		//双数最小不能小于2
		if (Size<2)
		{
			return;
		}
		//双数
		//单象限长度
		int32 QuadrantLength = Size/2;
		FVector PlaneStartLocation = FVector((50+QuadrantLength*100),-(50+QuadrantLength*100),1.0);

		for (int32 i=0;i<Size*Size;i++)
		{
			if (Meshs[i])
			{
				FVector CurrrentPlaneLocation = FVector(PlaneStartLocation.X,PlaneStartLocation.Y,PlaneStartLocation.Z);
				Meshs[i]->SetRelativeLocation(CurrrentPlaneLocation);
			}
		}
	}
	else if (FMath::IsNearlyEqual(1,Remainder))
	{
		//单数最小不能小于1
		if (Size<1)
		{
			return;
		}
		//单数
		
	}
}

void ABuildTipPlane::HideUnit()
{
}

// Called every frame
void ABuildTipPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

