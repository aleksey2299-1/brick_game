package web

import (
	"context"
	"log"
	"time"

	"github.com/gofiber/fiber/v3"
	"github.com/gofiber/fiber/v3/middleware/logger"
	"go.uber.org/fx"
)

func NewServer() *fiber.App {
	app := fiber.New(fiber.Config{
		IdleTimeout: 30 * time.Second,
	})

	app.Use(logger.New())

	return app
}

func StartServer(lc fx.Lifecycle, app *fiber.App) {
	lc.Append(fx.Hook{
		OnStart: func(ctx context.Context) error {
			go func() {
				log.Println("Starting server on :8080")
				if err := app.Listen(":8080"); err != nil {
					log.Fatalf("Fiber Listen error: %v", err)
				}
			}()
			return nil
		},
		OnStop: func(ctx context.Context) error {
			log.Println("Shutting down server...")
			return app.Shutdown()
		},
	})
}
