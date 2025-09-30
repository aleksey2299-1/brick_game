package domain

type GameState struct {
	Field     [][]int
	HighScore int
	Level     int
	Next      [][]int
	Pause     bool
	Score     int
	Speed     int
}

type GameInfo struct {
	Id   int
	Name string
}
