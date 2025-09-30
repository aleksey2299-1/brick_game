package main

/*
#include "../../headers/game.h"
*/
import "C"

import (
	"app/brick_game/race/domain"
)

const (
	Start     = C.Start
	Pause     = C.Pause
	Terminate = C.Terminate
	Left      = C.Left
	Right     = C.Right
	Up        = C.Up
)

type Controller struct {
	game      *domain.Race
	lastField **C.int
}

func (c *Controller) UserInput(action C.UserAction_t, hold C.bool) {
	switch action {
	case Start:
		c.game.StartGame()
	case Pause:
		c.game.PauseGame()
	case Terminate:
		c.game.EndGame()
	case Left:
		c.game.PlayerMoveLeft()
	case Right:
		c.game.PlayerMoveRight()
	case Up:
		if hold {
			c.game.PlayerSpeedUp()
		} else {
			c.game.PlayerMoveUp()
		}
	default:
		c.game.PlayerSlowDown()
	}
}

func (c *Controller) UpdateCurrentState() C.GameInfo_t {
	FreeCMatrix(c.lastField, C.HEIGHT)
	gameInfo := c.game.GetCurrentState()
	c.lastField = ToCMatrix(gameInfo.Field)
	var gi C.GameInfo_t
	gi.field = c.lastField
	gi.high_score = C.int(gameInfo.HightScore)
	gi.score = C.int(gameInfo.Score)
	gi.speed = C.int(gameInfo.Speed)
	gi.level = C.int(gameInfo.Level)
	gi.pause = C.int(gameInfo.Pause)
	return gi
}
