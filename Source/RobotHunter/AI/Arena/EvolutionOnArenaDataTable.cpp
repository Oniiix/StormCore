// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionOnArenaDataTable.h"

UEvolutionOnArenaDataTable::UEvolutionOnArenaDataTable() 
	: UDataTable()
{
	RowStruct = FEvolutionOnArenaTableRow::StaticStruct();
}
