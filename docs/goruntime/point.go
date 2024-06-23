package gom

import "os"

type Point struct {
	x float64
}

func (p Point) X() float64 {
	return p.x
}

func (p *Point) SetX(x float64) {
	p.x = x
}

type Point2d struct {
	Point
	y float64
}

func (p Point2d) Y() float64 {
	return p.y
}

func (p *Point2d) SetY(y float64) {
	p.y = y
}

type Point3d struct {
	*Point
	y float64
}

func (p Point3d) Y() float64 {
	return p.y
}

func (p *Point3d) SetY(y float64) {
	p.y = y
}

func ReadFile(f *os.File, b []byte) (n int, err error) {
	return f.Read(b)
}
