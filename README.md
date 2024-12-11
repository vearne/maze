# maze
使用Kruskal算法实现的迷宫生成算法

## 注意:BMP文件有对齐规则
Windows规定一个扫描行所占的字节数必须是4的倍数

## 编译
```
sh build.sh
```
## 运行
```
./maze {n} {imgPath}
```
`n`: 指定了迷宫的大小，**注意**，它必须为奇数

`imgPath`: 迷宫图片的输出路径
### 示例
```
./maze 61 /tmp/maze.bmp
```

## 效果图
![maze.bmp](./maze.jpeg)
