package di

import (
	"app/brick_game/server/internal/datasource"
	"app/brick_game/server/internal/domain"
	"app/brick_game/server/internal/web"

	"go.uber.org/fx"
)

func InitFx() {
	app := fx.New(
		fx.Provide(
			fx.Annotate(
				datasource.NewGame,
				fx.As(new(domain.Game)),
			),
			fx.Annotate(
				domain.NewGameService,
				fx.As(new(web.GameService)),
			),
			web.NewServer,
		),
		fx.Invoke(web.Register, web.StartServer),
	)

	app.Run()
}
