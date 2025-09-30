package web

import "app/brick_game/server/internal/domain"

type GameService interface {
	GetGames() []domain.GameInfo
	StartGameByID(id int) error
	GetCurrentGameState() (domain.GameState, error)
	UserAction(action int, hold bool) error
}
