package main

import (
	"app/brick_game/race/domain"
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

func TestController(t *testing.T) {
	assert := assert.New(t)
	var c Controller
	repo := new(MockedRepo)
	repo.On("GetRecord").Return(123, nil)
	c.game = domain.NewRace(repo)
	assert.Nil(c.UpdateCurrentState().field)
	c.UserInput(Start, false)
	gameInfo := c.UpdateCurrentState()
	assert.NotNil(gameInfo.field)
	assert.Equal(123, int(gameInfo.high_score))
	assert.Equal(0, int(gameInfo.score))
	assert.Equal(1, int(gameInfo.level))
	assert.Equal(0, int(gameInfo.pause))
	assert.Equal(500, int(gameInfo.speed))

	c.UserInput(Up, true)
	gameInfo = c.UpdateCurrentState()
	assert.NotNil(gameInfo.field)
	assert.Equal(123, int(gameInfo.high_score))
	assert.Equal(0, int(gameInfo.score))
	assert.Equal(1, int(gameInfo.level))
	assert.Equal(0, int(gameInfo.pause))
	assert.Equal(250, int(gameInfo.speed))

	c.UserInput(Up, false)
	gameInfo = c.UpdateCurrentState()
	assert.NotNil(gameInfo.field)
	assert.Equal(123, int(gameInfo.high_score))
	assert.Equal(0, int(gameInfo.score))
	assert.Equal(1, int(gameInfo.level))
	assert.Equal(0, int(gameInfo.pause))
	assert.Equal(500, int(gameInfo.speed))

	c.UserInput(Pause, false)
	gameInfo = c.UpdateCurrentState()
	assert.NotNil(gameInfo.field)
	assert.Equal(123, int(gameInfo.high_score))
	assert.Equal(0, int(gameInfo.score))
	assert.Equal(1, int(gameInfo.level))
	assert.Equal(1, int(gameInfo.pause))
	assert.Equal(500, int(gameInfo.speed))

	c.UserInput(Terminate, false)
	gameInfo = c.UpdateCurrentState()
	assert.NotNil(gameInfo.field)
	assert.Equal(123, int(gameInfo.high_score))
	assert.Equal(0, int(gameInfo.score))
	assert.Equal(1, int(gameInfo.level))
	assert.Equal(0, int(gameInfo.pause))
	assert.Equal(0, int(gameInfo.speed))

	c.UserInput(Terminate, false)
	gameInfo = c.UpdateCurrentState()
	assert.Nil(gameInfo.field)
	assert.Equal(0, int(gameInfo.high_score))
	assert.Equal(0, int(gameInfo.score))
	assert.Equal(0, int(gameInfo.level))
	assert.Equal(0, int(gameInfo.pause))
	assert.Equal(0, int(gameInfo.speed))

	// nothing to test
	c.UserInput(Left, false)
	c.UserInput(Right, false)
}
