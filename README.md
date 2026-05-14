# CH32V307 嵌入式开发项目

基于 WCH CH32V307 RISC-V 微控制器的嵌入式项目，使用 EIDE (Embedded IDE) 进行管理。

## 目录

- [项目结构](#项目结构)
- [环境配置](#环境配置)
- [导入项目](#导入项目)
- [编译构建](#编译构建)
- [程序烧录](#程序烧录)
- [硬件连接](#硬件连接)
- [常见问题](#常见问题)

---

## 项目结构

```
ch32v307/
├── .eide/                  # EIDE 配置目录（项目设置）
├── .vscode/                # VS Code 配置
├── src/                    # 用户源代码
│   ├── main.c             # 主程序入口
│   └── isr.c/h            # 中断服务例程
├── sdk/                    # WCH MCU SDK
│   ├── Core/              # 核心寄存器定义
│   ├── Peripheral/        # 外设驱动
│   ├── Startup/          # 启动文件
│   └── Ld/                # 链接脚本
├── zf_common/             # 通用基础库
├── zf_driver/             # 硬件驱动层
├── zf_device/             # 外设设备库
├── zf_components/         # 应用组件
├── tools/                 # OpenOCD 配置文件
├── download.cmd           # 下载脚本
└── build/                 # 编译输出目录
```

---

## 环境配置

### 必需软件

| 软件 | 说明 | 下载地址 |
|------|------|----------|
| **VS Code** | 代码编辑器 | https://code.visualstudio.com/ |
| **EIDE 插件** | VS Code 嵌入式开发扩展 | VS Code 内搜索 "eide" 安装 |
| **RISC-V 工具链** | 编译器、调试器 | 见下方说明 |
| **OpenOCD** | 程序烧录工具 | 见下方说明 |

### 1. 安装 VS Code

从 https://code.visualstudio.com/ 下载并安装 VS Code。

### 2. 安装 EIDE 插件

1. 打开 VS Code
2. 按 `Ctrl+Shift+X` 打开扩展商店
3. 搜索 **"eide"**
4. 安装 **EIDE** 插件（作者: figo）

### 3. 安装 RISC-V 工具链

项目使用 **MounRiver Studio** 配套的 RISC-V GCC 工具链。

**方式一：安装 MounRiver Studio（推荐）**
- 下载地址：http://www.mounriver.com/download
- 安装时会同时安装工具链和 OpenOCD
- 工具链路径：
  ```
  C:\MounRiver\MounRiver_Studio\workspace\tools\OpenOCD\bin\
  ```
- OpenOCD 路径：
  ```
  C:\MounRiver\MounRiver_Studio\workspace\tools\OpenOCD\bin\openocd.exe
  ```
- OpenOCD 配置文件目录：
  ```
  C:\MounRiver\MounRiver_Studio\workspace\tools\OpenOCD\scripts\
  ```

**方式二：独立安装 RISC-V GCC**
- 下载地址：https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/releases
- 选择 `xpack-riscv-none-elf-gcc-xxx-win32-x64.zip`
- 解压到任意目录

**方式三：使用其他 RISC-V 工具链**
- 如 WCH 官方提供的工具链
- 确保 `riscv-none-elf-gcc` 可在命令行中执行

---

## 导入项目

### 方法一：直接打开文件夹

1. 打开 VS Code
2. `文件` → `打开文件夹`
3. 选择项目根目录 `ch32v307`
4. EIDE 会自动识别 `.eide/` 配置文件

### 方法二：通过 EIDE 菜单导入

1. VS Code 中按 `Ctrl+Shift+P` 打开命令面板
2. 输入 `EIDE: Open Project`
3. 选择项目文件夹

### 配置工具链路径

如果 EIDE 未能自动识别工具链，需要手动配置：

1. 按 `Ctrl+Shift+P`，输入 `EIDE: Project Settings`
2. 在 `Build` 选项卡中设置：
   - **工具链路径**: 指向 `riscv-none-elf-gcc` 所在目录
   - **OpenOCD 路径**: 指向 OpenOCD 可执行文件目录

---

## 编译构建

### 在 VS Code 中编译

**方式一：使用快捷键**
- `Ctrl+B` - 构建项目
- `Ctrl+Shift+B` - 重新构建

**方式二：使用命令面板**
1. `Ctrl+Shift+P`
2. 输入 `EIDE: Build`
3. 选择项目构建

**方式三：使用 Terminal**
```bash
# 在项目目录下执行
make
```

### 查看构建输出

- 构建结果输出在 `build/` 目录
- 可执行文件：`build/Debug/ch32v307.elf`
- 反汇编：`build/Debug/ch32v307.asm`
- 地图文件：`build/Debug/ch32v307.map`

---

## 程序烧录

### 硬件准备

| 项目 | 说明 |
|------|------|
| **开发板** | CH32V307 开发板 |
| **调试器** | WCH-Link 或兼容 OpenOCD 的调试器 |
| **连接线** | 2.54mm 杜邦线（SWD 接口） |

### 接线方式

```
调试器 ←→ 开发板
--------------------
TMS   ←→ SWDIO
TCK   ←→ SWCLK
TDO   ←→ SWO（可选）
TDI   ←→ （不连接）
GND   ←→ GND
3.3V  ←→ 3.3V（如果调试器支持供电）
```

### 使用 download.cmd 烧录

项目根目录下的 `download.cmd` 是预配置的烧录脚本：

```cmd
@echo off
cd /d "%~dp0"
D:\Tools\OpenOCD\bin\openocd.exe -f tools/wch-interface.cfg -f tools/wch-target.cfg -c "program build/Debug/ch32v307.elf verify reset exit"
pause
```

使用前请确保：
1. OpenOCD 路径正确
2. 配置文件路径正确
3. 已连接调试器和开发板

### 在 VS Code 中烧录

1. `Ctrl+Shift+P`
2. 输入 `EIDE: Upload`
3. EIDE 会自动执行烧录流程

---

## 硬件连接

### 调试接口

CH32V307 使用 SWD 接口调试：

| 引脚 | 功能 | 说明 |
|------|------|------|
| PA13 | SWDIO | 调试数据输入/输出 |
| PA14 | SWCLK | 调试时钟 |
| 3.3V | VCC | 电源 |
| GND | GND | 地线 |

### 电源

- **供电电压**: 3.3V
- **建议**: 使用调试器供电或开发板自身供电

---

## 常见问题

### Q: EIDE 提示找不到工具链

**解决：**
1. 确认已安装 RISC-V GCC 工具链
2. 在 VS Code 中打开 `Ctrl+Shift+P` → `EIDE: Project Settings`
3. 设置正确的工具链路径

### Q: 编译报错 "undefined reference"

**解决：**
1. 检查是否缺少必要的库文件
2. 确认 `zf_device/libzf_device_config.a` 存在
3. 检查链接脚本 `sdk/Ld/Link.ld`

### Q: 烧录失败

**解决：**
1. 检查调试器连接
2. 确认开发板已上电
3. 检查 OpenOCD 配置文件
4. 尝试重新安装驱动

### Q: 提示 "No source file named main.c"

**解决：**
这是正常的 GDB 调试信息，不影响实际运行。如需调试，请确保 `build/Debug/` 目录下有编译产物。

### Q: 如何清理构建产物

1. `Ctrl+Shift+P` → `EIDE: Clean`
2. 或手动删除 `build/` 目录

---

## 项目信息

| 项目 | 内容 |
|------|------|
| **MCU** | WCH CH32V307VCT6 |
| **架构** | RISC-V RV32IMAFCXW |
| **主频** | 144 MHz |
| **Flash** | 256 KB |
| **RAM** | 63 KB |
| **许可证** | Apache-2.0 / GPL-3.0 |
| **第三方库** | SEEKFREE 逐飞科技开源库 |

---

## 联系方式

如有问题，请检查：
- EIDE 文档：https://github.com/Swordfish90/EIDE
- CH32 官方资料：http://www.wch.cn/
- 逐飞科技：http://www.seekfree.cn/