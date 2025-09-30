package web

import (
	"strconv"

	"github.com/gofiber/fiber/v3"
)

func GetGames(s GameService) fiber.Handler {
	return func(c fiber.Ctx) error {
		games := s.GetGames()
		c.Response().SetStatusCode(fiber.StatusOK)
		return c.JSON(toApiGamesList(games))
	}
}

func PostGameByID(s GameService) fiber.Handler {
	return func(c fiber.Ctx) error {
		gameIdString := c.Params("gameId")
		gameId, err := strconv.Atoi(gameIdString)
		if err != nil {
			c.Response().SetStatusCode(fiber.StatusBadRequest)
			return c.JSON(ErrorMessage{Message: err.Error()})
		}
		if err = s.StartGameByID(gameId); err != nil {
			c.Response().SetStatusCode(getStatusCode(err))
			return c.JSON(ErrorMessage{Message: err.Error()})
		}
		c.Response().SetStatusCode(fiber.StatusOK)
		return nil
	}
}

func GetCurrentGameState(s GameService) fiber.Handler {
	return func(c fiber.Ctx) error {
		state, err := s.GetCurrentGameState()
		if err != nil {
			c.Response().SetStatusCode(getStatusCode(err))
			return c.JSON(ErrorMessage{Message: err.Error()})
		}
		c.Response().SetStatusCode(fiber.StatusOK)
		return c.JSON(toApiGameState(state))
	}
}

func PostUserAction(s GameService) fiber.Handler {
	return func(c fiber.Ctx) error {
		userAction, err := unmarshalJsonRequest[UserAction](c.Body())
		if err != nil {
			c.Response().SetStatusCode(fiber.StatusBadRequest)
			return c.JSON(ErrorMessage{Message: err.Error()})
		}
		action := -1
		if userAction.ActionId != nil {
			action = *userAction.ActionId
		}
		hold := false
		if userAction.Hold != nil {
			hold = *userAction.Hold
		}
		if err = s.UserAction(action, hold); err != nil {
			c.Response().SetStatusCode(getStatusCode(err))
			return c.JSON(ErrorMessage{Message: err.Error()})
		}
		c.Response().SetStatusCode(fiber.StatusOK)
		return nil
	}
}
