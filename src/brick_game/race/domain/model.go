package domain

/*
#include "../../../headers/game.h"
*/
import "C"
import (
	"math/rand"
	"time"
)

const (
	Height     = C.HEIGHT
	Width      = C.WIDTH
	Speed      = 500000
	SpeedBoost = 50000
)

var rival = [5][3]int{
	{0, 1, 0},
	{1, 1, 1},
	{0, 1, 0},
	{1, 1, 1},
	{0, 1, 0},
}

var playerCar = [4][3]int{
	{0, 1, 0},
	{1, 1, 1},
	{0, 1, 0},
	{1, 1, 1},
}

type Race struct {
	state        int
	pause        bool
	speed        int
	level        int
	score        int
	highScore    int
	repo         RaceRepository
	playerCar    *Car
	lastTime     int64
	pauseStarted int64
	rivals       []*Car
	nextSpawn    int
	boosted      bool
}

type GameInfo struct {
	Field      [][]int
	Score      int
	HightScore int
	Level      int
	Speed      int
	Pause      int
}

const (
	exit = iota
	waiting
	starting
	moving
	gameover
)

const maxSpawnTime = 19
const minSpawnTime = 12

// Public methods

func NewRace(repo RaceRepository) *Race {
	return &Race{repo: repo}
}

func (race *Race) StartGame() {
	if race.state == waiting || race.state == exit {
		race.state = starting
	}
}

func (race *Race) EndGame() {
	if race.state == exit {
		return
	}
	if race.state == waiting {
		race.state = exit
	} else {
		race.state = gameover
	}
}

func (race *Race) PauseGame() {
	if race.state != moving {
		return
	}
	if !race.pause {
		race.pauseStarted = time.Now().UnixMicro()
	} else {
		race.lastTime += time.Now().UnixMicro() - race.pauseStarted
	}
	race.pause = !race.pause
}

func (race *Race) PlayerMoveRight() {
	race.PlayerSlowDown()
	if race.state != moving || race.pause {
		return
	}
	race.playerCar.moveRight()
	race.checkRoadAccident()
}

func (race *Race) PlayerMoveLeft() {
	race.PlayerSlowDown()
	if race.state != moving || race.pause {
		return
	}
	race.playerCar.moveLeft()
	race.checkRoadAccident()
}

func (race *Race) PlayerMoveUp() {
	race.PlayerSlowDown()
	if race.state != moving || race.pause {
		return
	}
	race.moveRivals()
	race.checkRoadAccident()
}

func (race *Race) PlayerSpeedUp() {
	if race.state != moving || race.pause {
		return
	}
	if !race.boosted {
		race.boosted = true
		race.speed /= 2
	}
}

func (race *Race) PlayerSlowDown() {
	if race.boosted {
		race.boosted = false
		race.speed *= 2
	}
}

func (race *Race) GetCurrentState() GameInfo {
	if race.state == exit {
		return GameInfo{}
	}
	race.fsm()
	field := race.formField()
	pause := 0
	if race.pause {
		pause = 1
	}
	highScore := max(race.score, race.highScore)
	return GameInfo{
		Field:      field,
		Score:      race.score,
		HightScore: highScore,
		Level:      race.level,
		Speed:      race.speed / 1000,
		Pause:      pause,
	}
}

// Private methods

func (race *Race) fsm() {
	if race.pause && race.state != gameover {
		return
	}
	switch race.state {
	case starting:
		race.initializeGame()
	case moving:
		race.moveInterval()
	case gameover:
		race.gameOver()
	}
}

func (race *Race) initializeGame() {
	if race.state == starting {
		highScore, _ := race.repo.GetRecord()
		race.highScore = highScore
		race.score = 0
		race.pause = false
		race.state = moving
		race.speed = Speed
		race.level = 1
		race.nextSpawn = 0
		race.rivals = []*Car{}
		race.playerCar = &Car{Position{4, 16}, 0}
	}
}

func (race *Race) moveInterval() {
	now := time.Now().UnixMicro()
	if now-race.lastTime > int64(race.speed) {
		race.lastTime = now
		race.moveRivals()
		race.checkRoadAccident()
	}
}

func (race *Race) gameOver() {
	race.repo.SaveScore(race.score)
	race.state = waiting
	race.speed = 0
	race.pause = false
}

func (race *Race) moveRivals() {
	for _, v := range race.rivals {
		v.moveDown()
	}
	race.nextSpawn--
	if race.nextSpawn < 0 {
		race.spawnRivals()
		race.nextSpawn = rand.Intn(maxSpawnTime-minSpawnTime) + minSpawnTime
	}
	race.deleteRivals()
}

func (race *Race) checkRoadAccident() {
	if race.playerCar == nil {
		return
	}
	for _, v := range race.rivals {
		if v.pos.x == race.playerCar.pos.x {
			if v.pos.y > race.playerCar.pos.y-5 && v.pos.y < race.playerCar.pos.y+4 {
				race.state = gameover
			}
		}
	}
}

func (race *Race) spawnRivals() {
	num := rand.Intn(2) + 1
	for range num {
		ok := false
		var car Car
		car.spawnRival()
	Collision:
		for !ok {
			car.spawnRival()
			for _, v := range race.rivals {
				if v.pos.x == car.pos.x && car.pos.y < v.pos.y+5 && car.pos.y > v.pos.y-5 {
					continue Collision
				}
			}
			offsetY := rand.Intn(5)
			car.pos.y -= offsetY
			ok = true
		}
		race.rivals = append(race.rivals, &car)
	}
}

func (race *Race) deleteRivals() {
	var newRivals []*Car
	for _, v := range race.rivals {
		if v.pos.y < Height {
			newRivals = append(newRivals, v)
		} else {
			race.addScore()
		}
	}
	race.rivals = newRivals
}

func (race *Race) lvlUp() {
	race.level++
	race.speed = Speed - SpeedBoost*(race.level-1)
	if race.boosted {
		race.speed /= 2
	}
}

func (race *Race) addScore() {
	race.score++
	if race.score%5 == 0 && race.score < 50 {
		race.lvlUp()
	}
}

func (race *Race) formField() [][]int {
	field := make([][]int, Height)
	for y := range field {
		field[y] = make([]int, Width)
	}
	for _, v := range race.rivals {
		for y := range rival {
			if v.pos.y+y < 0 {
				continue
			}
			if v.pos.y+y >= Height {
				break
			}
			for x := range rival[y] {
				if rival[y][x] != 0 {
					field[v.pos.y+y][v.pos.x-1+x] = v.color
				}
			}
		}
	}
	if race.playerCar != nil {
		for y := range playerCar {
			for x := range playerCar[y] {
				if playerCar[y][x] != 0 {
					field[race.playerCar.pos.y+y][race.playerCar.pos.x-1+x] = playerCar[y][x]
				}
			}
		}
	}
	return field
}
