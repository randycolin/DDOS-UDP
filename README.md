
### 经过长期收集反馈我们得知很多测试者用的都是1C1G的VPS在进行发包测试，所以我们增加了一个把硬件资源利用到极致的C方案，对比GO方案差距在于C的硬件效率更高，占用更低，发包效率更高。


### GoFuck声明：

- Gofuck是一个简单暴力的UDP发包程序，能快速利用全部带宽。
- 禁止将Gofuck用于违法活动，代码仅用于本地测试参考，如违规使用，责任自负。

### C版本使用方法：（Go版本需要自行编译）
```shell
// 下载程序
wget https://github.com/randycolin/DDOS-UDP/releases/download/v.1.0.1/GoFuck
// 赋予执行权限
chmod +x GoFuck
// 输入相应参数开始执行例如：./GoFuck 127.0.0.1 80
./GoFuck <IP地址> <端口>
- 按 CTRL+C 停止程序
```

## Star History

[![Star History Chart](https://api.star-history.com/svg?repos=randycolin/DDOS-UDP&type=Date)](https://star-history.com/#randycolin/DDOS-UDP&Date)
