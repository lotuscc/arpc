一个异步RPC框架


简单异步RPC框架。
关键特性：异步、协程、Reactor、Protobuf、RPC。
使用ucontext实现了基本的协程支持，结合Reactor架构实现Connector的异步读写接口，写业务代码时可以直接使用同步的写法，而不用写各种回调函数。
在异步读写接口中如果发生了阻塞事件，会自动yield掉当前协程，让出CPU使用权以提高系统性能。


系统的整体架构如图所示![](https://github.com/lotuscc/arpc/blob/dev/img/arpc.drawio.png)

每个Connector提供异步读写接口。
在异步读写的过程中，如果发生阻塞操作（比如需要读64个字节，但是网络中只发送了32个字节），
此时当前Connector会自动yeild掉，让出占用的线程，使其可以完成其他事件例如accept新连接、读写另一个Connector的数据等等。

从整体上来看，虽然每一个Connector是同步的，但是整体系统是异步的，可以充分利用当前系统的资源。
并且Connector的yeild操作也是由于网络数据未达等客观原因，此时如果不yeild，客观上还是需要等待网络数据到达，白白浪费了等待时间。