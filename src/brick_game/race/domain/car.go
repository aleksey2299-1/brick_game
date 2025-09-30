package domain

import "math/rand"

type Position struct {
	x, y int
}

type Car struct {
	pos   Position
	color int
}

func (car *Car) moveDown() {
	car.pos.y++
}

func (car *Car) moveLeft() {
	if car.pos.x > 3 {
		car.pos.x -= 3
	}
}

func (car *Car) moveRight() {
	if car.pos.x < Width-3 {
		car.pos.x += 3
	}
}

func (car *Car) spawnRival() {
	car.pos.y = -5
	car.pos.x = rand.Intn(3)*3 + 1
	car.color = rand.Intn(3) + 2
}
