# Development Specifications

## Goal 1 用Python调用C代码实现模型加载, 渲染, 输出

### Spec 1 数据类型与ctypes兼容

1. 所有成员变量, 函数参数, 返回值只能包含Eigen矩阵, 向量类型或C++原始类型
2. 所有函数参数和返回值不能使用左右值引用
3. 方法参数, 返回值可以使用任意类型

C++原始类型与ctypes兼容. Eigen类型拥有等价的C Struct, 详见[Eigen文档](https://eigen.tuxfamily.org/dox-devel/classEigen_1_1Matrix.html#ac59ab3932980f113533eaed7fc651756)


## Goal 2 在思路清晰的前提下, 降低编码思维负担

### Spec 1 必须为每个成员变量提供初始值, 且在未来开发不能更改

### Spec 2 所有几何数据结构都使用32位浮点数

### Spec 3 变量命名区分顶点和顶点坐标

1. 顶点叫做Vertex
2. 顶点坐标叫做Vertex Coordinate