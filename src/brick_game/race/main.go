//go:build !cover
// +build !cover

package main

/*
#include "../../headers/game.h"
*/
import "C"

import (
	"app/brick_game/race/datasource"
	"app/brick_game/race/domain"
	"sync"
)

var (
	instance *Controller
	once     sync.Once
)

func getContorller() *Controller {
	once.Do(func() {
		repo := datasource.NewSqliteRepository()
		game := domain.NewRace(repo)
		instance = &Controller{game, nil}
	})
	return instance
}

//export UserInput
func UserInput(action C.UserAction_t, hold C.bool) {
	controller := getContorller()
	controller.UserInput(action, hold)
}

//export UpdateCurrentState
func UpdateCurrentState() C.GameInfo_t {
	controller := getContorller()
	return controller.UpdateCurrentState()
}

func main() {}
