package web

import (
	"app/brick_game/server/internal/datasource"
	appErrors "app/brick_game/server/internal/domain/errors"
	"encoding/json"
	"errors"

	"github.com/gofiber/fiber/v3"
)

var codeMapping = map[int]int{
	datasource.GameNotFoundErrorCode: fiber.StatusNotFound,
	datasource.GameStartedErrorCode:  fiber.StatusConflict,
	datasource.RuntimeErrorCode:      fiber.StatusBadRequest,
}

func unmarshalJsonRequest[T any](body []byte) (T, error) {
	var jsonBody T

	if err := json.Unmarshal(body, &jsonBody); err != nil {
		return jsonBody, errors.New("invalid body")
	}
	return jsonBody, nil
}

func getStatusCode(err error) int {
	if cerr, ok := err.(appErrors.CodedError); ok {
		if code, ok := codeMapping[cerr.Code()]; ok {
			return code
		} else {
			return fiber.StatusInternalServerError
		}
	} else {
		return fiber.StatusInternalServerError
	}
}
