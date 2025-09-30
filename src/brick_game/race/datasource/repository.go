package datasource

import (
	"context"
	"database/sql"
	"log"

	_ "modernc.org/sqlite"
)

type SqliteRepository struct {
	db *sql.DB
}

func (repo *SqliteRepository) initDb() error {
	ctx := context.Background()
	conn, err := repo.db.Conn(ctx)
	if err != nil {
		return err
	}
	defer conn.Close()
	if _, err := conn.ExecContext(
		ctx,
		`CREATE TABLE IF NOT EXISTS race(id INTEGER PRIMARY KEY AUTOINCREMENT, score INTEGER NOT NULL)`,
	); err != nil {
		return err
	}
	return nil
}

func NewSqliteRepository() *SqliteRepository {
	db, err := sql.Open("sqlite", "score.db")
	if err != nil {
		log.Fatalf("failed to connect: %v", err)
	}
	repo := SqliteRepository{db}
	err = repo.initDb()
	if err != nil {
		log.Fatalf("failed to initialize: %v", err)
	}
	return &repo
}

func (repo *SqliteRepository) GetRecord() (int, error) {
	ctx := context.Background()
	conn, err := repo.db.Conn(ctx)
	if err != nil {
		return 0, err
	}
	defer conn.Close()
	row := conn.QueryRowContext(ctx, "SELECT MAX(score) FROM race")
	var highScore int
	err = row.Scan(&highScore)
	if err != nil {
		return 0, err
	}
	return highScore, nil
}

func (repo *SqliteRepository) SaveScore(score int) error {
	ctx := context.Background()
	conn, err := repo.db.Conn(ctx)
	if err != nil {
		return err
	}
	defer conn.Close()
	if _, err := conn.ExecContext(ctx, "INSERT INTO race(score) VALUES (?)", score); err != nil {
		return err
	}
	return nil
}
