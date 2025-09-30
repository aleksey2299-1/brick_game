package domain

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/mock"
)

type MockedRepo struct {
	mock.Mock
}

func (m *MockedRepo) GetRecord() (int, error) {
	args := m.Called()
	return args.Int(0), args.Error(1)
}

func (m *MockedRepo) SaveScore(score int) error {
	return nil
}

// Test private methods

func TestModelInitGame(t *testing.T) {
	assert := assert.New(t)
	repo := new(MockedRepo)
	repo.On("GetRecord").Return(123, nil)
	game := NewRace(repo)
	game.state = starting
	game.fsm()

	assert.Equal(123, game.highScore)
	assert.Equal(0, game.score)
	assert.Equal(Speed, game.speed)
	assert.Equal(false, game.pause)
	assert.Equal(moving, game.state)
	assert.Equal(1, game.level)
	assert.Equal(0, game.nextSpawn)
	assert.Equal(0, len(game.rivals))
	if assert.NotNil(game.playerCar) {
		assert.Equal(4, game.playerCar.pos.x, "player car should spawn at x = 4")
		assert.Equal(16, game.playerCar.pos.y, "player car should spawn at y = 16")
	}
}

func TestModelPlaying(t *testing.T) {
	assert := assert.New(t)
	repo := new(MockedRepo)
	repo.On("GetRecord").Return(123, nil)
	game := NewRace(repo)
	game.state = starting
	game.fsm()
	assert.Equal(moving, game.state)
	game.fsm()
	assert.True(len(game.rivals) > 0)
	rival := game.rivals[0]
	positionY := rival.pos.y
	for i := range 10 {
		game.lastTime = 0
		game.fsm()
		assert.Equal(positionY+i+1, rival.pos.y)
	}
}

func TestModelGameOver(t *testing.T) {
	assert := assert.New(t)
	repo := new(MockedRepo)
	repo.On("GetRecord").Return(123, nil)
	game := NewRace(repo)
	game.state = starting
	game.fsm()
	assert.Equal(moving, game.state)
	game.fsm()
	game.rivals[0].pos.x = 4
	game.rivals[0].pos.y = 10
	game.lastTime = 0
	game.fsm()
	assert.Equal(moving, game.state)
	game.lastTime = 0
	game.fsm()
	assert.Equal(gameover, game.state)
	game.lastTime = 0
	game.fsm()
	assert.Equal(waiting, game.state)
	assert.Equal(0, game.speed)
	assert.Equal(false, game.pause)
}

func TestModelPause(t *testing.T) {
	assert := assert.New(t)
	repo := new(MockedRepo)
	repo.On("GetRecord").Return(123, nil)
	game := NewRace(repo)
	game.state = starting
	game.fsm()
	assert.Equal(moving, game.state)
	game.fsm()
	game.rivals[0].pos.x = 4
	game.rivals[0].pos.y = 5
	game.pause = true
	for range 10 {
		game.lastTime = 0
		game.fsm()
		assert.Equal(4, game.rivals[0].pos.x)
		assert.Equal(5, game.rivals[0].pos.y)
	}
}

func TestModelScore(t *testing.T) {
	assert := assert.New(t)
	repo := new(MockedRepo)
	repo.On("GetRecord").Return(123, nil)
	game := NewRace(repo)
	game.state = starting
	game.fsm()
	game.playerCar.pos.x = 15
	assert.Equal(moving, game.state)
	assert.Equal(0, game.score)
	game.fsm()
	for range 200 {
		game.lastTime = 0
		game.fsm()
	}
	assert.True(len(game.rivals) < 8)
	assert.True(game.score > 0)
	var expectedLevel int
	if game.score > 45 {
		expectedLevel = 10
	} else {
		expectedLevel = game.score/5 + 1
	}
	assert.Equal(expectedLevel, game.level)
	assert.Equal(Speed-SpeedBoost*(game.level-1), game.speed)
}

func TestFieldDrawing(t *testing.T) {
	assert := assert.New(t)
	repo := new(MockedRepo)
	repo.On("GetRecord").Return(123, nil)
	game := NewRace(repo)
	game.state = starting
	game.fsm()
	game.fsm()
	field := game.formField()
	assert.True(len(game.rivals) > 0)
	for y := range 16 {
		for x := range field[y] {
			assert.Equal(0, field[y][x])
		}
	}
	for y := range playerCar {
		for x := range playerCar[y] {
			if playerCar[y][x] != 0 {
				assert.Equal(playerCar[y][x], field[game.playerCar.pos.y+y][game.playerCar.pos.x-1+x])
			}
		}
	}
	game.playerCar = nil
	for range 50 {
		game.lastTime = 0
		game.fsm()
		field := game.formField()
		for _, v := range game.rivals {
			for y := range field {
				for x := range field[y] {
					if y == v.pos.y && x == v.pos.x {
						for i := range rival {
							if v.pos.y+i < 0 {
								continue
							}
							if v.pos.y+i >= Height {
								break
							}
							for j := range rival[i] {
								if rival[i][j] != 0 {
									assert.Equal(v.color, field[v.pos.y+i][v.pos.x-1+j])
								}
							}
						}
					}
				}
			}
		}
	}
}

// Test public methods

func TestStartGame(t *testing.T) {
	assert := assert.New(t)
	repo := new(MockedRepo)
	repo.On("GetRecord").Return(123, nil)
	game := NewRace(repo)
	game.StartGame()
	assert.Equal(starting, game.state)
}

func TestEndGame(t *testing.T) {
	assert := assert.New(t)
	repo := new(MockedRepo)
	repo.On("GetRecord").Return(123, nil)
	game := NewRace(repo)
	game.StartGame()
	assert.Equal(starting, game.state)
	game.EndGame()
	assert.Equal(gameover, game.state)
	game.EndGame()
	assert.Equal(gameover, game.state)
	game.fsm()
	assert.Equal(waiting, game.state)
	game.EndGame()
	assert.Equal(exit, game.state)
	game.EndGame()
	assert.Equal(exit, game.state)
}

func TestPauseGame(t *testing.T) {
	assert := assert.New(t)
	repo := new(MockedRepo)
	repo.On("GetRecord").Return(123, nil)
	game := NewRace(repo)
	game.StartGame()
	assert.Equal(starting, game.state)
	assert.Equal(false, game.pause)
	game.PauseGame()
	assert.Equal(starting, game.state)
	assert.Equal(false, game.pause)

	game.fsm()
	assert.Equal(moving, game.state)
	game.PauseGame()
	assert.Equal(moving, game.state)
	assert.Equal(true, game.pause)
	game.PauseGame()
	assert.Equal(moving, game.state)
	assert.Equal(false, game.pause)
}

func TestPlayerMoveRight(t *testing.T) {
	assert := assert.New(t)
	repo := new(MockedRepo)
	repo.On("GetRecord").Return(123, nil)
	game := NewRace(repo)
	game.StartGame()
	assert.Equal(starting, game.state)
	game.fsm()
	playerCar := game.playerCar
	for range 3 {
		positionX := playerCar.pos.x
		positionY := playerCar.pos.y
		game.PlayerMoveRight()
		if positionX < 7 {
			assert.Equal(positionX+3, playerCar.pos.x)
			assert.Equal(positionY, playerCar.pos.y)
		} else {
			assert.Equal(positionX, playerCar.pos.x)
			assert.Equal(positionY, playerCar.pos.y)
		}
	}

	assert.Equal(moving, game.state)
	playerCar.pos.x = 4
	game.rivals = []*Car{{Position{7, 11}, 0}}
	game.PlayerMoveRight()
	assert.Equal(moving, game.state)

	playerCar.pos.x = 4
	game.rivals = []*Car{{Position{7, 12}, 0}}
	game.PlayerMoveRight()
	assert.Equal(gameover, game.state)
}

func TestPlayerMoveLeft(t *testing.T) {
	assert := assert.New(t)
	repo := new(MockedRepo)
	repo.On("GetRecord").Return(123, nil)
	game := NewRace(repo)
	game.StartGame()
	assert.Equal(starting, game.state)
	game.fsm()
	playerCar := game.playerCar
	for range 3 {
		positionX := playerCar.pos.x
		positionY := playerCar.pos.y
		game.PlayerMoveLeft()
		if positionX > 3 {
			assert.Equal(positionX-3, playerCar.pos.x)
			assert.Equal(positionY, playerCar.pos.y)
		} else {
			assert.Equal(positionX, playerCar.pos.x)
			assert.Equal(positionY, playerCar.pos.y)
		}
	}

	assert.Equal(moving, game.state)
	playerCar.pos.x = 4
	game.rivals = []*Car{{Position{1, 20}, 0}}
	game.PlayerMoveLeft()
	assert.Equal(moving, game.state)

	playerCar.pos.x = 4
	game.rivals = []*Car{{Position{1, 19}, 0}}
	game.PlayerMoveLeft()
	assert.Equal(gameover, game.state)
}

func TestPlayerMoveUp(t *testing.T) {
	assert := assert.New(t)
	repo := new(MockedRepo)
	repo.On("GetRecord").Return(123, nil)
	game := NewRace(repo)
	game.StartGame()
	assert.Equal(starting, game.state)
	game.fsm()
	game.fsm()
	rival := game.rivals[0]
	positionY := rival.pos.y
	for i := range 3 {
		game.PlayerMoveUp()
		assert.Equal(positionY+i+1, rival.pos.y)
	}
}

func TestPlayerSpeedUp(t *testing.T) {
	assert := assert.New(t)
	repo := new(MockedRepo)
	repo.On("GetRecord").Return(123, nil)
	game := NewRace(repo)
	game.StartGame()
	assert.Equal(starting, game.state)
	game.fsm()
	game.fsm()
	game.PlayerSpeedUp()
	game.fsm()
	assert.Equal(true, game.boosted)
	assert.Equal(Speed/2, game.speed)

	game.PlayerSpeedUp()
	game.fsm()
	assert.Equal(true, game.boosted)
	assert.Equal(Speed/2, game.speed)
}

func TestPlayerSlowDown(t *testing.T) {
	assert := assert.New(t)
	repo := new(MockedRepo)
	repo.On("GetRecord").Return(123, nil)
	game := NewRace(repo)
	game.StartGame()
	assert.Equal(starting, game.state)
	game.fsm()
	game.fsm()
	game.PlayerSpeedUp()
	game.fsm()
	assert.Equal(true, game.boosted)
	assert.Equal(Speed/2, game.speed)

	game.PlayerSlowDown()
	game.fsm()
	assert.Equal(false, game.boosted)
	assert.Equal(Speed, game.speed)

	game.PlayerSlowDown()
	game.fsm()
	assert.Equal(false, game.boosted)
	assert.Equal(Speed, game.speed)
}

func TestPlayerCantMove(t *testing.T) {
	assert := assert.New(t)
	repo := new(MockedRepo)
	repo.On("GetRecord").Return(123, nil)
	game := NewRace(repo)
	game.state = starting
	game.initializeGame()
	assert.Equal(moving, game.state)
	assert.Equal(false, game.pause)
	game.state = starting

	positionX := game.playerCar.pos.x
	testStaticPosition := func() {
		game.PlayerMoveLeft()
		assert.Equal(positionX, game.playerCar.pos.x)
		game.PlayerMoveRight()
		assert.Equal(positionX, game.playerCar.pos.x)
		game.PlayerMoveUp()
		assert.Equal(positionX, game.playerCar.pos.x)
		assert.Equal(0, len(game.rivals))
	}

	testStaticPosition()

	game.state = gameover
	testStaticPosition()

	game.state = waiting
	testStaticPosition()

	game.state = exit
	testStaticPosition()

	game.state = moving
	game.pause = true
	testStaticPosition()
}

func TestCurrentState(t *testing.T) {
	assert := assert.New(t)
	repo := new(MockedRepo)
	repo.On("GetRecord").Return(123, nil)
	game := NewRace(repo)
	gameInfo := game.GetCurrentState()
	assert.Nil(gameInfo.Field)
	assert.Equal(0, gameInfo.Score)
	assert.Equal(0, gameInfo.HightScore)
	assert.Equal(0, gameInfo.Level)
	assert.Equal(0, gameInfo.Speed)
	assert.Equal(0, gameInfo.Pause)

	game.StartGame()
	gameInfo = game.GetCurrentState()
	assert.NotNil(gameInfo.Field)
	assert.Equal(0, gameInfo.Score)
	assert.Equal(123, gameInfo.HightScore)
	assert.Equal(1, gameInfo.Level)
	assert.Equal(Speed/1000, gameInfo.Speed)
	assert.Equal(0, gameInfo.Pause)

	game.score = 124
	gameInfo = game.GetCurrentState()
	assert.NotNil(gameInfo.Field)
	assert.Equal(124, gameInfo.Score)
	assert.Equal(124, gameInfo.HightScore)
	assert.Equal(1, gameInfo.Level)
	assert.Equal(Speed/1000, gameInfo.Speed)
	assert.Equal(0, gameInfo.Pause)

	game.score = 0

	game.PauseGame()
	gameInfo = game.GetCurrentState()
	assert.NotNil(gameInfo.Field)
	assert.Equal(0, gameInfo.Score)
	assert.Equal(123, gameInfo.HightScore)
	assert.Equal(1, gameInfo.Level)
	assert.Equal(Speed/1000, gameInfo.Speed)
	assert.Equal(1, gameInfo.Pause)

	game.PauseGame()
	gameInfo = game.GetCurrentState()
	assert.NotNil(gameInfo.Field)
	assert.Equal(0, gameInfo.Score)
	assert.Equal(123, gameInfo.HightScore)
	assert.Equal(1, gameInfo.Level)
	assert.Equal(Speed/1000, gameInfo.Speed)
	assert.Equal(0, gameInfo.Pause)

	game.PauseGame()
	game.EndGame()
	gameInfo = game.GetCurrentState()
	assert.NotNil(gameInfo.Field)
	assert.Equal(0, gameInfo.Score)
	assert.Equal(123, gameInfo.HightScore)
	assert.Equal(1, gameInfo.Level)
	assert.Equal(0, gameInfo.Speed)
	assert.Equal(0, gameInfo.Pause)

	game.EndGame()
	gameInfo = game.GetCurrentState()
	assert.Nil(gameInfo.Field)
	assert.Equal(0, gameInfo.Score)
	assert.Equal(0, gameInfo.HightScore)
	assert.Equal(0, gameInfo.Level)
	assert.Equal(0, gameInfo.Speed)
	assert.Equal(0, gameInfo.Pause)
}
