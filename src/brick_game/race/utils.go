package main

/*
#include <stdlib.h>
*/
import "C"
import "unsafe"

func ToCMatrix(field [][]int) **C.int {
	rows := len(field)
	if rows == 0 {
		return nil
	}

	cRows := C.malloc(C.size_t(rows) * C.size_t(unsafe.Sizeof(uintptr(0))))

	rowPtrs := (*[1 << 30]*C.int)(cRows)[:rows:rows]

	for i := range rows {
		cols := len(field[i])
		cCols := C.malloc(C.size_t(cols) * C.size_t(unsafe.Sizeof(C.int(0))))
		colSlice := (*[1 << 30]C.int)(cCols)[:cols:cols]

		for j := range cols {
			colSlice[j] = C.int(field[i][j])
		}

		rowPtrs[i] = (*C.int)(cCols)
	}

	return (**C.int)(cRows)
}

func FreeCMatrix(matrix **C.int, rows int) {
	if matrix == nil || rows <= 0 {
		return
	}
	rowPtrs := (*[1 << 30]*C.int)(unsafe.Pointer(matrix))[:rows:rows]
	for i := range rows {
		C.free(unsafe.Pointer(rowPtrs[i]))
	}
	C.free(unsafe.Pointer(matrix))
}
