# 添加新驱动程序

1. 将文件夹 guy_template 整个拷贝到 guy_epaper 文件夹下

2. 根据里面的模板文件, 添加自定义驱动.

3. 使用查找工具搜索 src 下的以下文本.

```
//添加新屏幕型号 add displays here
```

然后把新加入的屏幕驱动 define 给加上

4. 更新 `EPD_DRIVERS_NUM_MAX` 变量, 表明一共有多少个驱动可用.

5. 加好了之后, 应该就可以使用这个驱动了.

6. 记得更改项目根目录的 readme 文件

# 禁用驱动程序

前往 guy_epaper_config.h 文件, 注释掉不需要编译的驱动文件即可
