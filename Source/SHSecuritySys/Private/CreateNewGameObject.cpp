// Fill out your copyright notice in the Description page of Project Settings.

#include "CreateNewGameObject.h"

UObject* UCreateNewGameObject::NewObjectFromBlueprint(UObject* owner, UClass* UC)
{
	UObject* tempObject = NewObject<UObject>(owner,UC);

	return tempObject;
}



