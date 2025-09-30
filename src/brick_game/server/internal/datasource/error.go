package datasource

const (
	RuntimeErrorCode = iota + 5
	GameNotFoundErrorCode
	GameStartedErrorCode
)

type DatasourceError struct {
	message    string
	statusCode int
}

func (e *DatasourceError) Error() string {
	return e.message
}

func (e *DatasourceError) Code() int {
	return e.statusCode
}
