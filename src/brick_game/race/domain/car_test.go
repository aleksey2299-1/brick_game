package domain

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestSpawnRival(t *testing.T) {
	assert := assert.New(t)
	var car Car
	validX := []int{1, 4, 7}
	for range 20 {
		car.spawnRival()
		assert.Equal(car.pos.y, -5, "rival should spawn at y = -5")
		assert.Contains(validX, car.pos.x, "rival spawned between lines")
		assert.True(car.color >= 2 && car.color <= 4)
	}
}

func TestMoveRight(t *testing.T) {
	assert := assert.New(t)
	var car Car
	for i := range Width - 3 {
		car.pos.x = i
		car.moveRight()
		assert.Equal(car.pos.x, i+3)
		for range 3 {
			car.moveRight()
		}
		assert.True(car.pos.x < Width)
	}
}

func TestMoveLeft(t *testing.T) {
	assert := assert.New(t)
	var car Car
	for i := 4; i < Width; i++ {
		car.pos.x = i
		car.moveLeft()
		assert.Equal(car.pos.x, i-3)
		for range 3 {
			car.moveLeft()
		}
		assert.True(car.pos.x > 0)
	}
}

func TestMoveDown(t *testing.T) {
	assert := assert.New(t)
	var car Car
	car.pos.y = 0
	car.moveDown()
	assert.Equal(car.pos.y, 1)
}
