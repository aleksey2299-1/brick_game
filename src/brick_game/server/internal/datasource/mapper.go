package datasource

/*
#include "../../../../headers/game.h"
*/
import "C"
import (
	"app/brick_game/server/internal/domain"
	"sort"
)

func mapDSGameInfoToDomainGameState(info C.GameInfo_t) domain.GameState {
	var gameState domain.GameState
	gameState.Field = convertCMatrixToGoSlice(info.field, C.HEIGHT, C.WIDTH)
	gameState.Next = convertCMatrixToGoSlice(info.next, 4, 4)
	gameState.HighScore = int(info.high_score)
	gameState.Score = int(info.score)
	gameState.Level = int(info.level)
	gameState.Speed = int(info.speed)
	if info.pause > 0 {
		gameState.Pause = true
	} else {
		gameState.Pause = false
	}
	return gameState
}

func mapMapIntStrToDomainGameInfoArray(dict map[int]Game) []domain.GameInfo {
	var info []domain.GameInfo
	for k, v := range dict {
		info = append(info, domain.GameInfo{Id: k, Name: v.name})
	}
	sort.Slice(info, func(i, j int) bool {
		return info[i].Id < info[j].Id
	})
	return info
}
