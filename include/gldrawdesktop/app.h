#ifndef GLDRAWDESKTOP_APP_H
#define GLDRAWDESKTOP_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* 启动应用主循环，返回进程退出码。 */
int gldrawdesktop_run(void);

/* 返回编译期写入的版本字符串（形如 "3.0.0"）。 */
const char* gldrawdesktop_version(void);

#ifdef __cplusplus
}
#endif

#endif /* GLDRAWDESKTOP_APP_H */
