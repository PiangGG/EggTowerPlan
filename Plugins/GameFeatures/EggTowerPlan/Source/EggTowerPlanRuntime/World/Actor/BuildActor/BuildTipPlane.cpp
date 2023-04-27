// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildTipPlane.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "EggTowerPlanRuntime/Tool/StructLib.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABuildTipPlane::ABuildTipPlane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	Meshs.SetNum(Maxlength*Maxlength);
	// MeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("MeshComponent"));
	// MeshComponent->SetupAttachment(RootSceneComponent);

	
}

// Called when the game starts or when spawned
void ABuildTipPlane::BeginPlay()
{
	Super::BeginPlay();
	
	InitUnit();
	//ShowUnit(length);
}

void ABuildTipPlane::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void ABuildTipPlane::InitUnit()
{
	for (int32 i = 0;i<Maxlength*Maxlength;i++)
	{
		Meshs[i] = NewObject<UStaticMeshComponent>(this,TEXT("Mesh%d"+i));
		Meshs[i]->RegisterComponent();
		if (StaticMesh&&Meshs[i])
		{
			Meshs[i]->SetStaticMesh(StaticMesh);
			AddInstanceComponent(Meshs[i]);
			Meshs[i]->AttachToComponent(RootSceneComponent,FAttachmentTransformRules::KeepRelativeTransform);
			Meshs[i]->SetRelativeLocation(FVector(0.0,0.0,-100.0f));
			Meshs[i]->SetCollisionProfileName(FName("ETP_BuildUnit"));
			Meshs[i]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			// Meshs[i]->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::ABuildTipPlane::UnitBeginOverlap);
			// Meshs[i]->OnComponentEndOverlap.AddDynamic(this,&ThisClass::ABuildTipPlane::UnitEndOverlap);
			Meshs[i]->SetMaterial(0,Material_G);
			Meshs[i]->SetHiddenInGame(true);
			FBuildUnit BuildUnit;
			BuildUnit.MeshComponent = Meshs[i];
			BuildUnit.OverlapSize = 0;
			MeshsOverlapMap.Add(BuildUnit);
		}
	}
}

void ABuildTipPlane::ShowUnit(int32 Size)
{
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
		int32 QuadrantLength = Size/2-1;
		int32 i = 0;
		for (int32 x=0;x<Size;x++)
		{
			for (int32 y=0;y<Size;y++)
			{
				if (Meshs[i])
				{
					FVector PlaneStartLocation = FVector((50+QuadrantLength*100),-(50+QuadrantLength*100),1.0);
					FVector CurrrentPlaneLocation = FVector(PlaneStartLocation.X-100*x,PlaneStartLocation.Y+100*y,PlaneStartLocation.Z);
					// FTransform Transform = GetActorTransform();
					// Transform.SetLocation(CurrrentPlaneLocation);
					// MeshComponent->AddInstance(Transform);
					Meshs[i]->SetRelativeLocation(CurrrentPlaneLocation);
					Meshs[i]->SetHiddenInGame(false);
					i++;
				}
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
		int32 QuadrantLength = Size/2;
		int32 i = 0;
		for (int32 x=0;x<Size;x++)
		{
			for (int32 y=0;y<Size;y++)
			{
				if (Meshs[i])
				{
					FVector PlaneStartLocation = FVector(QuadrantLength*100,-(QuadrantLength*100),20.0);
					FVector CurrrentPlaneLocation = FVector(PlaneStartLocation.X-100*x,PlaneStartLocation.Y+100*y,PlaneStartLocation.Z);
					FTransform Transform = GetActorTransform();
					Transform.SetLocation(CurrrentPlaneLocation);
					Meshs[i]->SetRelativeLocation(CurrrentPlaneLocation);
					Meshs[i]->SetHiddenInGame(false);
					i++;
				}
			}
		}
	}
}

void ABuildTipPlane::HideUnit()
{
	for (int32 i = 0;i<Maxlength*Maxlength;i++)
	{
		if (Meshs[i])
		{
			Meshs[i]->SetRelativeLocation(FVector(0.0,0.0,-100.0f));
			Meshs[i]->SetMaterial(0,Material_G);
			Meshs[i]->SetHiddenInGame(true);
		}
	}
}

void ABuildTipPlane::UnitBeginOverlap(UPrimitiveComponent* OtherComp)
{
	UStaticMeshComponent* TempStatic = Cast<UStaticMeshComponent>(OtherComp);
	if (TempStatic)
	{
		bCanBuild = false;
		OverlapNumber++;
		for (FBuildUnit& unit : MeshsOverlapMap)
		{
			if (TempStatic==unit.MeshComponent)
			{
				unit.OverlapSize+=1;
				unit.MeshComponent->SetMaterial(0,Material_R);
			}
		}
	}
}

void ABuildTipPlane::UnitEndOverlap(UPrimitiveComponent* OtherComp)
{
	UStaticMeshComponent* TempStatic = Cast<UStaticMeshComponent>(OtherComp);
	if (TempStatic)
	{
		OverlapNumber--;
		for (FBuildUnit& unit : MeshsOverlapMap)
		{
			if (TempStatic==unit.MeshComponent)
			{
				unit.OverlapSize-=1;
				if (unit.OverlapSize <= 0)
				{
					unit.MeshComponent->SetMaterial(0,Material_G);
				}
			}
		}
		if (OverlapNumber<=0)
		{
			bCanBuild = true;
		}
	}
}

// Called every frame
void ABuildTipPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

