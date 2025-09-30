package datasource

import "C"
import "unsafe"

func convertCMatrixToGoSlice(matrix **C.int, rows, cols int) [][]int {
	if matrix == nil {
		return nil
	}
	rowPtrs := unsafe.Slice(matrix, rows)
	field := make([][]int, rows)
	for i := range rows {
		row := unsafe.Slice(rowPtrs[i], cols)
		field[i] = make([]int, cols)
		for j := range cols {
			field[i][j] = int(row[j])
		}
	}
	return field
}
