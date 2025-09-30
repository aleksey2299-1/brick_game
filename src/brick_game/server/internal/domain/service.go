package domain

type GameServiceImpl struct {
	game Game
}

func NewGameService(game Game) *GameServiceImpl {
	return &GameServiceImpl{game}
}

func (s *GameServiceImpl) GetGames() []GameInfo {
	return s.game.GetAllGames()
}

func (s *GameServiceImpl) StartGameByID(id int) error {
	if err := s.game.LoadLibrary(id); err != nil {
		return err
	}
	return nil
}

func (s *GameServiceImpl) GetCurrentGameState() (GameState, error) {
	state, err := s.game.UpdateCurrentState()
	if err != nil {
		return GameState{}, err
	}
	return state, nil
}

func (s *GameServiceImpl) UserAction(action int, hold bool) error {
	if err := s.game.UserInput(action, hold); err != nil {
		return err
	}
	return nil
}
