package web

import (
	"app/brick_game/server/internal/domain"
)

func toApiGameState(gs domain.GameState) GameState {
	field := make([][]bool, len(gs.Field))
	for i := range field {
		field[i] = make([]bool, len(gs.Field[i]))
		for j := range field[i] {
			if gs.Field[i][j] > 0 {
				field[i][j] = true
			}
		}
	}
	next := make([][]bool, len(gs.Next))
	for i := range next {
		next[i] = make([]bool, len(gs.Next[i]))
		for j := range next[i] {
			if gs.Next[i][j] > 0 {
				next[i][j] = true
			}
		}
	}
	return GameState{
		Field:     &field,
		HighScore: &gs.HighScore,
		Level:     &gs.Level,
		Next:      &next,
		Pause:     &gs.Pause,
		Score:     &gs.Score,
		Speed:     &gs.Speed,
	}
}

func toApiGameInfo(gi domain.GameInfo) GameInfo {
	return GameInfo{
		Id:   &gi.Id,
		Name: &gi.Name,
	}
}

func toApiGamesList(gi []domain.GameInfo) GamesList {
	gamesInfo := make([]GameInfo, len(gi))
	for i := range gamesInfo {
		gamesInfo[i] = toApiGameInfo(gi[i])
	}
	return GamesList{Games: &gamesInfo}
}
