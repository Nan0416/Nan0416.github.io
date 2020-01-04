# Swift - Functions

Created: 2019-12-19
Modified: 2019-12-19

* [Introduction](#intro)
* [Examples](#examples)
* [References](#reference)
***
### I. <a id="intro">Introduction</a>
1. Functions in swift is a value. A function has its type, can be passed into another function as argument.
2. Functions can have positional arguments, labeled arguments, and arguments with default values.
3. Functions' arguments can have in-out modifiers.
4. Functions can be written in another functions and swift support closure.

##### in-out parameters
By default, arguments are passed as constant (let) so that the functions cannot update arguments (e.g. function body cannot reset the reference to struct/class, but it can modify the content of class object.)

```swift
class MyC{
    var arr: [Int] = []
    func MyC(){}
}
func test(arg1: MyC, arg2: inout [Int]){ // int array is a struct.
    arg2.append(100)
    arg1.arr.append(100)
}
let asss: MyC = MyC()
test(arg1: asss, arg2: &ia)
```

the *intout* keyword cannot works with let variable and immediate value (literal value).

In addition, *inout* is not pass-by-reference.
##### function types
Same as STLC (simply typed lambda calculus), A->B->C, where types are right associative. In other words, A->B->C == A->(B->C)
For example (Int, Int)->Int is a function type that takes two integer and return an integer.

```swift
func functionCreator()->(Int, Int)->Int{
    func add(_ arg1: Int, _ arg2: Int) -> Int{
        return arg1 + arg2
    }
    return add // function as a value.
}
functionCreator()(1,1)
```

### II. <a id="examples">Examples</a>
1. A swift function syntax is **_func function-name([label-name argument-name: type, ...]) [ -> return type { body }]_**.

```swift
func hello(name message: String) -> Void{
    print("Hello, "  + message)
}
hello(name: "Qinnan")
```

The above function is named as hello(name:), and _-> Void_ can be ignored. Void is a special type that only has one inhabitant (unit type).
Swift put type at then end of argument and function name because of providing type inference.

2. Function label name and argument name, and function invocation

Different from other programming languages, a swift function's argument has argument name that used inside the function body, and labeled name used for invocation. If only argument name is provided, then label name is same as the argument name. 

Moreover, function must be invoked with labeled names unless the label is explicitly set _.

```swift
func hello(_ name: String, _ message: String){
    print("hello, " + name + ": " + message)
}
hello("qinnan", "how are you?") // label is not required since the argument's label is set _.
```

3. Implicit return
If a function only contains a single statement, then this statement is implicity returned. 

```swift
func hello(_ name: String) -> String{
    "hello, " + name // returned even thought no return is given.
}
```

4. Default Arguments and Overloading
Swift allows default arguments and recommends to put arguments without default values first but they can be in any order.
```swift
func test(_ arg1:String = "default value", _ arg2: String){
    print(arg1, arg2)
}
test("hello", "what") // with all positional arguments, the default value is ignored.
test("hello") // error: missing value for arg2. 

func test2(_ arg1:String = "default value", arg2: String){
    print(arg1, arg2)
}
test2(arg2:"qinnan")
```

### III. <a id="reference">References</a>
1. <a href="" target="_blank">Swift 5.1 - Language Guide (Function)</a>
2. <a href="https://stackoverflow.com/questions/39569114/swift-3-0-error-escaping-closures-can-only-capture-inout-parameters-explicitly" target="_blank">swift inout is not pass-by-reference.</a>