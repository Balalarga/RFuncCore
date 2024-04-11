// Fill out your copyright notice in the Description page of Project Settings.
#include "RnkGameMode.h"

#include "RanokVisualizer/Hud/RnkHud.h"


ARnkGameMode::ARnkGameMode()
{
	HUDClass = ARnkHud::StaticClass();
}
