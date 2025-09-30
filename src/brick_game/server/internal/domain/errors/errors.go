package errors

type CodedError interface {
	error
	Code() int
}
