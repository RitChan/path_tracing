# Document Specifications

## Goal 1 降低编码思维负担

### Spec 1 完全地定义输入输出 (X)

1. 每个函数的文档必须分析所有可能的输入, 并定义相应的输出. 短期内这项Spec会增大思维负担. 但长期看, 编码者会因为清晰的定义变得轻松.
2. 考虑到分析的复杂性

### Spec 2 明确指针生命周期管理权责

1. 必须为每个自定义class或struct注明以下情况之一 (连同未来的代码一起考虑)
    - Has Pointer: 
        1. 超类或子类是Has Pointer
        2. 或有至少一个成员变量是指针
        3. 或有至少一个成员变量的类型也是Has Pointer
    - No Pointer:
        1. 不是Has Pointer
2. 对于每个函数, 若有至少一个函数输入是指针或函数参数的类型是Has Pointer, 必须在文档中为每一个这种输入注明以下情况之一 (连同未来的代码一起考虑, 输入包括函数的参数以及方法的成员变量)
    - Definite Free: 表示该函数或该函数的其它实现一定会释放参数指针
    - Sub Free: 表示该函数不会释放参数指针, 但会释放参数内部的指针
    - Not Free: 表示该函数或该函数的其它实现一定不会释放参数中包含的指针
3. 对于每个函数, 若返回值是Has Pointer, 必须在文档中注明以下情况之一
    - Allocate Ret: 表示该函数会为返回值分配内存
    - External Ret: 表示该函数不为返回值分配内存
4. 不可对指针使用引用类型

### Spec 3 变量声明时, 如有缩写, 必须在注释中指明全称

## Goal 2 提升代码审美价值

### Spec 1 所有文档标点符号使用英文标点

### Spec 2 每个documented文档必须包含文件级别的@file和@brief
