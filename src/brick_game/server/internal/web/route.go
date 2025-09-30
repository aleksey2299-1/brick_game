package web

import (
	"github.com/gofiber/fiber/v3"
	"github.com/gofiber/fiber/v3/middleware/static"
)

func Register(app *fiber.App, s GameService) {
	api := app.Group("/api")
	api.Post("/actions", PostUserAction(s))
	api.Get("/state", GetCurrentGameState(s))

	game := api.Group("/games")
	game.Get("/", GetGames(s))
	game.Post("/:gameId", PostGameByID(s))

	app.Get("/*", static.New("./brick_game_web"))
}
