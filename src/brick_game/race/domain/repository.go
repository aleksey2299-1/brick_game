package domain

type RaceRepository interface {
	GetRecord() (int, error)
	SaveScore(score int) error
}
