package datasource

/*
#include <dlfcn.h>
#include <stdlib.h>
#include "../../../../headers/game.h"

typedef void (*UserInputFunc)(UserAction_t, bool);
typedef GameInfo_t (*UpdateCurrentStateFunc)(void);

GameInfo_t callUpdateCurrentState(void *f) {
    return ((UpdateCurrentStateFunc)f)();
}

void callUserInput(void *f, UserAction_t action, bool hold) {
    ((UserInputFunc)f)(action, hold);
}
*/
import "C"
import (
	"app/brick_game/server/internal/domain"
	"sync"
	"unsafe"
)

type Game struct {
	libraryName string
	name        string
}

var games = map[int]Game{
	0: {libraryName: "tetris", name: "Тетрис"},
	1: {libraryName: "snake", name: "Змейка"},
	2: {libraryName: "race", name: "Гонка"},
}

type GameImpl struct {
	mutex      sync.RWMutex
	handle     unsafe.Pointer
	inputFunc  unsafe.Pointer
	updateFunc unsafe.Pointer
	gameOver   bool
}

func NewGame() *GameImpl {
	return &GameImpl{}
}

func (g *GameImpl) GetAllGames() []domain.GameInfo {
	return mapMapIntStrToDomainGameInfoArray(games)
}

func (g *GameImpl) UpdateCurrentState() (domain.GameState, error) {
	g.mutex.RLock()
	defer g.mutex.RUnlock()
	if g.updateFunc == nil {
		return domain.GameState{}, &DatasourceError{"function \"UpdateCurrentState\" isnt loaded", RuntimeErrorCode}
	}
	info := C.callUpdateCurrentState(g.updateFunc)
	if info.field == nil {
		g.gameOver = true
	} else {
		g.gameOver = false
	}
	return mapDSGameInfoToDomainGameState(info), nil
}

func (g *GameImpl) UserInput(action int, hold bool) error {
	g.mutex.RLock()
	inputFunc := g.inputFunc
	gameOver := g.gameOver
	g.mutex.RUnlock()
	if inputFunc == nil {
		return &DatasourceError{"function \"UserInput\" isnt loaded", RuntimeErrorCode}
	}
	if action == C.Terminate && gameOver {
		g.closeLibrary()
		return nil
	}
	C.callUserInput(inputFunc, C.UserAction_t(action), C.bool(hold))
	return nil
}

func (g *GameImpl) LoadLibrary(id int) error {
	g.mutex.Lock()
	defer g.mutex.Unlock()

	if g.handle != nil {
		return &DatasourceError{"Game already started", GameStartedErrorCode}
	}
	game, ok := games[id]
	if !ok {
		return &DatasourceError{"wrong game id", GameNotFoundErrorCode}
	}
	name := C.CString("./lib_" + game.libraryName + ".so")
	defer C.free(unsafe.Pointer(name))
	g.handle = C.dlopen(name, C.RTLD_LAZY)
	if g.handle == nil {
		return &DatasourceError{"cannot load library", RuntimeErrorCode}
	}

	inputName := C.CString("UserInput")
	defer C.free(unsafe.Pointer(inputName))
	g.inputFunc = C.dlsym(g.handle, C.CString("UserInput"))
	if g.inputFunc == nil {
		C.dlclose(g.handle)
		return &DatasourceError{"cannot find \"UserInput\"", RuntimeErrorCode}
	}

	updateName := C.CString("UpdateCurrentState")
	defer C.free(unsafe.Pointer(updateName))
	g.updateFunc = C.dlsym(g.handle, updateName)
	if g.updateFunc == nil {
		C.dlclose(g.handle)
		return &DatasourceError{"cannot find \"UpdateCurrentState\"", RuntimeErrorCode}
	}
	return nil
}

func (g *GameImpl) closeLibrary() {
	g.mutex.Lock()
	defer g.mutex.Unlock()
	if g.handle != nil {
		C.dlclose(g.handle)
		g.handle = nil
		g.inputFunc = nil
		g.updateFunc = nil
	}
}
