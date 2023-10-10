# vmfpp
C++ library for manipulating Hammer Editor .vmf map files

This library constructs a simple node tree that you can query any structure(s) and any property(s) of.

### Usage
```cpp

func main() {
	file,_ := os.Open("de_dust2.vmf")

	reader := vmf.NewReader(file)
	f,_ := reader.Read()

	fmt.Println(f.Entities.GetChildrenByKey("entity")[0].GetProperty("classname"))
	fmt.Println(f.Entities.GetChildrenByKey("entity")[0].GetChildrenByKey("solid"))
}
```
