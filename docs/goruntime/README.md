# 深度探索Go语言对象模型与runtime的原理、特性及应用

## chap4 方法

**编译器对继承来的方法都是通过生成相应的包装方法来调用原始方法的方式实现的**(P96)。注意：对于多重继承情况下，即该类中内嵌了多个基类，某个方法在多个基类中都有实现，那Go编译器不会为该方法生成包装方法，需要程序要手动实现(P100)。

## chap5 接口

### 空接口

#### 空接口是一个增强版的void*

**面向对象编程中，接口用来对行为进行抽象，也就是定义对象需要支持的操作，这组操作对应的就是接口中列出的方法**。不包含任何方法的接口可以认为不要求对象支持任何操作，因此能够接受任意类型的赋值，所以Go语言的interface{}什么都能装(P102)。C语言中void* 只用来表示这是一个指针，并不限定指向的目标数据的类型，可以接受任意类型的地址，而interface{}通过_type字段给出了数据的类型信息，因此可以说inteface{}相当于一个增强版的void*(P103)。

对于interface{}类型的变量e，它的声明类型是interface{}，这一点也是不能改变的。变量e就像是一个容器，可以装载任意类型的数据，并通过_type字段记录数据的类型，无论装载什么类型的数据，容器本身的类型不会改变。因为_type会随着变量e装载不同类型的数据而发生改变，所以它称为**变量e的动态类型**，并相应地把变量e的声明类型称为静态类型(P104)。

#### 类型元数据

Go语言与C语言不同的是，在设计之初就支持面向对象编程，还有其他一些动态语言特性(类型断言，反射、内存管理），这些都要求运行阶段能够获取类型信息，所以语言的设计者就把类型信息用统一的数据结构来描述，并写入可执行文件中供运行阶段使用，这就是所谓的**类型元数据**(P104)。

类型元数据的结构是_type。_type提供了使用所有类型的最基本的描述，对于一些更复杂的类型，例如复合类型slice和map等，runtime中分别定义了maptype，slicetype等对应的结构(P105)。

```go
type slicetype struct{
    typ _type
    elem *_type
}
```

#### 逃逸与装箱

把任意一个变量赋值给空接口interface{}变量的过程称为装箱。interface{}在装载值的时候，必须要单独复制一份，而不是直接让data存储原始变量的地址，因为原始变量的值后续可能会发生改变，这就会造成逻辑错误。装箱不一定会进行堆分配，是否堆分配还是要经过逃逸分析。只有值类型装箱后又设计逃逸的情况，才会用到runtime中一系列convT()函数(p112)。ps个人理解：需要注意的是即使未发生逃逸，也需要单独复制一份值，只不过值一直存放栈上。

### 非空接口

与空接口对应，非空接口指的是至少包含一种方法的接口，就像io.Reader和io.Writeer。非空接口通过一组方法对行为进行抽象，从而隔离具体实现达到解耦的目的。Go的接口比Java等语言中的接口更加灵活，自定义类型不需要通过implements关键字显示地标明自己实现了某个接口，只要实现了接口中所有的方法就实现了该接口。

#### 动态派发

在面向对象编程中，接口的一个核心功能是支持多态，实际上就是方法的动态派发。调用接口的某个方法，调用者不需要知道背后对象的具体类型就能调用对象的指定方法。

**一个确定的接口类和一个确定的具体类型**，就能够唯一确定一个func数组，因此可以通过一个全局的map将func数组进行缓存，这样就能进一步减少方法集合查询，从而优化性能。

**类型的值只能实现值接收者的接口；指向类型的指针，既可以实现值接收者的接口，也可以实现指针接收者的接口**。

```go
type Mover interface {
    move()
}

type dog struct {}

func (d dog) move() {
    fmt.Println("狗会动")
}

func main() {
    var x Mover
    var wangcai = dog{} // 旺财是dog类型
    x = wangcai         // x可以接收dog类型
    var fugui = &dog{}  // 富贵是*dog类型
    x = fugui           // x可以接收*dog类型
    x.move()
}
```

```go
func (d *dog) move() {
    fmt.Println("狗会动")
}

func main() {
    var x Mover
    var wangcai = dog{} // 旺财是dog类型
    x = wangcai         // x不可以接收dog类型
    var fugui = &dog{}  // 富贵是*dog类型
    x = fugui           // x可以接收*dog类型
}
```

#### 类型断言

所谓类型断言，就是运行阶段根据元数据信息，来判断数据是否属于某种具体类型，或者是否实现了某个接口。既然要用到类型元数据，那么源操作数就必须是interface{}或某个接口类型的变量，也就是说底层是runtime.eface或者runtime.iface类型。

类型断言可以分为四种情况：

##### E To 具体类型

E指的是runtime.eface，也就是interface{}类型，而具体类型是相对于抽象类型来讲的，抽象类型指的是接口，接口通过方法列表对行为进行抽象，所以具体类型指的是除接口以外的内置类型和自定义类型。

```go
func normal(a interface{}) int {
    return a.(int)
}
```

##### E to I

E指的是runtime.eface, I指的runtime.iface，E to I也就是interface{}到某个自定义接口类型的断言。

```go
func normal(a interface{}) io.ReadWriter{
    return a.(io.ReaderWriter)
}
```

##### I to 具体类型

```go
func normal(i io.ReaderWriter) *os.File {
    return i.(*os.File)
}
```

##### I to I

```go
func normal(rw io.ReadWriter) io.Reader {
    return rw.(io.Reader)
}
```

##### 接口与nil指针

Go的接口类型是双指针结构，一个指向itab，另一个指向实际的数据对象。只有在两个指针都为nil时候，接口变量才等于nil(P147)。**nil指针也是有类型的(P148)**。

```go
type TestType struct {
	name string
}

func (t TestType) SetName(name string) {
	t.name = name
}

type Namer interface {
	SetName(name string)
}

func main() {
	fmt.Println(toType(nil) == nil) // false

    var rw io.ReadWriter
	fmt.Println(rw == nil) // true

	var f *os.File
	rw = f
	fmt.Println(rw == nil) // false
}

func toType(t *TestType) Namer {
	return t
}
```