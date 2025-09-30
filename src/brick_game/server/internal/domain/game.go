package domain

type Game interface {
	GetAllGames() []GameInfo
	LoadLibrary(id int) error
	UpdateCurrentState() (GameState, error)
	UserInput(action int, hold bool) error
}
