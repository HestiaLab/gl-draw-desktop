# 界面字体

本目录随程序分发界面使用的中文字体，满足项目仅提供中文界面的要求。

## 文件

| 文件 | 说明 |
| --- | --- |
| `NotoSansSC-Regular.ttf` | Noto Sans SC Regular 静态实例，SIL Open Font License 1.1 |
| `OFL.txt` | 字体许可证原文 |

## 来源与再生成

原始字库来自本机安装的 `NotoSansSC-VF.ttf`（Noto Sans SC 可变字体，glyf 轮廓）。
静态实例由 fonttools 生成：

```sh
python -m fontTools.varLib.instancer NotoSansSC-VF.ttf wght=400 -o NotoSansSC-Regular.ttf
```

生成结果 SHA-256：

```text
3e0954de2a02a487803a65716bb5244a36f5d83797432d84bf7cccc9962d948f
```

界面图集使用 `src/ui/ui_font_glyph_ranges.h` 中的 GB2312 常用字范围
（ASCII、拉丁补充、常用标点、CJK 标点、全角字符与 6763 个 GB2312 汉字，
共 7370 个字符），避免烘焙完整的 CJK 基本区。修改字体文件时请同步更新本页
记录与字形范围。

## 分发方式

构建时复制到可执行文件同级的 `fonts/` 目录；安装时随程序分发到
`share/gldrawdesktop/fonts/`。运行时可用 `GLDRAWDESKTOP_FONT` 环境变量
指定其他字体文件。
