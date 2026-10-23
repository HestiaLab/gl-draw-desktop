# third_party

本目录只存放第三方源码及其编译胶水，不参与项目警告、格式化与静态分析扫描。

| 目录 | 组件 | 版本/来源 | 许可证 |
| --- | --- | --- | --- |
| `glad/` | GLAD OpenGL 3.3 加载器 | 由 glad 生成器产出（compatibility profile，GL 3.3） | MIT |
| `KHR/` | Khronos API 注册表头文件 | 随 GLAD/GLFW 分发 | Apache-2.0 / MIT |
| `nuklear/` | Nuklear 即时模式 GUI | 4.12.8 单头文件发行版 | MIT / Public Domain |

`nuklear/nuklear_impl.c` 是本项目的编译单元，只定义实现宏并包含 vendored 头文件；
修改它属于构建配置变更，不属于第三方代码维护。
