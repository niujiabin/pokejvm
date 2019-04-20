#include<fstream>
#include<iostream>
#include<cstring>
#include <cstdint>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>

using namespace std;

#define CONSTANT_Methodref 10
#define CONSTANT_Fieldref 9
#define CONSTANT_InterfaceMethodref 11

struct cp_info {
    uint8_t tag;
    uint16_t name_index;
};

/**
 *
 *
00000000: cafe babe 0000 0034 0029 0a00 0900 1a09  .......4.)......
00000010: 0003 001b 0700 1c0a 0003 001a 0a00 0300  ................
00000020: 1d09 001e 001f 0a00 0300 200a 0021 0022  .......... ..!."
00000030: 0700 2301 0004 6e61 6d65 0100 124c 6a61  ..#...name...Lja
00000040: 7661 2f6c 616e 672f 5374 7269 6e67 3b01  va/lang/String;.
00000050: 0003 6167 6501 0001 4901 0006 3c69 6e69  ..age...I...<ini
00000060: 743e 0100 0328 2956 0100 0443 6f64 6501  t>...()V...Code.
00000070: 000f 4c69 6e65 4e75 6d62 6572 5461 626c  ..LineNumberTabl
00000080: 6501 0006 6765 7441 6765 0100 0328 2949  e...getAge...()I
00000090: 0100 0673 6574 4167 6501 0004 2849 2956  ...setAge...(I)V
000000a0: 0100 046d 6169 6e01 0016 285b 4c6a 6176  ...main...([Ljav
000000b0: 612f 6c61 6e67 2f53 7472 696e 673b 2956  a/lang/String;)V
000000c0: 0100 0a53 6f75 7263 6546 696c 6501 0009  ...SourceFile...
000000d0: 5465 7374 2e6a 6176 610c 000e 000f 0c00  Test.java.......
000000e0: 0c00 0d01 0004 5465 7374 0c00 1400 1507  ......Test......
000000f0: 0024 0c00 2500 260c 0012 0013 0700 270c  .$..%.&.......'.
00000100: 0028 0015 0100 106a 6176 612f 6c61 6e67  .(.....java/lang
00000110: 2f4f 626a 6563 7401 0010 6a61 7661 2f6c  /Object...java/l
00000120: 616e 672f 5379 7374 656d 0100 036f 7574  ang/System...out
00000130: 0100 154c 6a61 7661 2f69 6f2f 5072 696e  ...Ljava/io/Prin
00000140: 7453 7472 6561 6d3b 0100 136a 6176 612f  tStream;...java/
00000150: 696f 2f50 7269 6e74 5374 7265 616d 0100  io/PrintStream..
00000160: 0570 7269 6e74 0021 0003 0009 0000 0002  .print.!........
00000170: 0000 000a 000b 0000 0000 000c 000d 0000  ................
00000180: 0004 0001 000e 000f 0001 0010 0000 001d  ................
00000190: 0001 0001 0000 0005 2ab7 0001 b100 0000  ........*.......

 * @return
 */


int parseTestClass() {
    const char *classFileName = "/home/jiabinniu/CLionProjects/pokejvm/java/Test.class";

    ifstream inFile((classFileName), ios::binary);
    if (!inFile) {
        cerr << endl << "Read File error" << endl;
        return -1;
    }

    //获取魔术

    uint32_t real_magic = 0xCAFEBABE;

    uint32_t magic = 0;
    uint16_t minor_version = 0;
    uint16_t major_version = 0;
    uint16_t constant_pool_count = 0;
    cp_info **constant_pool = nullptr;

    inFile.read((char *) &magic, sizeof(magic));
    magic = htonl(magic);

    //检查魔术
    if (magic == real_magic) {
        cout << "check magic succeed" << endl;
    } else {
        cout << "check magic failed" << endl;
    }

    //判断major_version 和　minor_version
    inFile.read((char *) &minor_version, sizeof(minor_version));
    minor_version = htons(minor_version);
    // a = htonl(minor_version);

    inFile.read((char *) &major_version, sizeof(major_version));
    major_version = htons(major_version);

    cout << "minor version:" << minor_version << endl;
    cout << "major version:" << major_version << endl;

    //验证版本号

    //常量池数量
    inFile.read((char *) &constant_pool_count, sizeof(constant_pool_count));
    constant_pool_count = htons(constant_pool_count);
    cout << "constant_pool_count:" << constant_pool_count << endl;

    if (constant_pool_count != -1) {
        constant_pool = new cp_info *[constant_pool_count - 1];
        //常量池信息
        for (int i = 0; i < 1; ++i) {
            //获取常量池的tag
            uint8_t tag = inFile.peek();
            cout << "tag:" << (int) tag << endl;
            switch (tag) {
                case CONSTANT_Fieldref:
                    cout << "parse constant pool match CONSTANT_Fieldref" << endl;
                case CONSTANT_Methodref:
                    cout << "parse constant pool match CONSTANT_Methodref" << endl;
                case CONSTANT_InterfaceMethodref:
                    cout << "parse constant pool match CONSTANT_InterfaceMethodref" << endl;
                    //class_index
                    break;
            }
        }
    }
}

#define NLOOP 5000;

//互斥量
static pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
static int counter;

//假设定义一个java对象　实际的对象并不会占用这么多空间
struct object {


    /**
      * Mark Word　 对象标记关键字
      * 如哈希码（HashCode）、GC分代年龄、锁状态标志、线程持有的锁、偏向线程 ID、偏向时间戳等等,它是实现轻量级锁和偏向锁的关键.
      * class oopDesc {
          friend class VMStructs;
         private:
          volatile markOop  _mark;//markOop:Mark Word标记字段
          union _metadata {
            Klass*      _klass;//对象类型元数据的指针
            narrowKlass _compressed_klass;
          } _metadata;

          // Fast access to barrier set.  Must be initialized.
          static BarrierSet* _bs;

         public:
          markOop  mark() const         { return _mark; }
          markOop* mark_addr() const    { return (markOop*) &_mark; }

          void set_mark(volatile markOop m)      { _mark = m;   }

          void    release_set_mark(markOop m);
          markOop cas_set_mark(markOop new_mark, markOop old_mark);

          // Used only to re-initialize the mark word (e.g., of promoted
          // objects during a GC) -- requires a valid klass pointer
          void init_mark();

          Klass* klass() const;
          Klass* klass_or_null() const volatile;
          Klass** klass_addr();
          narrowKlass* compressed_klass_addr();

          　1 ObjectMonitor* monitor() const {
            2     assert(has_monitor(), "check");
            3     // Use xor instead of &~ to provide one extra tag-bit check.
            4     return (ObjectMonitor*) (value() ^ monitor_value);
            5   }
        ....省略...
        }
      */

    //存在于对象头部
    int hashCode;

    //存在于对象头部
    int gc_generation;

    //锁状态　存在于对象头部　01　表示无锁状态　00轻量级锁　10　重量级锁
    int lock_status;

    //指向Class的指针
    //对象实例信息略
};
/**
 * 监视器锁对象
 */
struct objectMonitor{

/**
 * ObjectMonitor() {
    _header       = NULL;//markOop对象头
    _count        = 0;
    _waiters      = 0,//等待线程数
    _recursions   = 0;//重入次数
    _object       = NULL;//监视器锁寄生的对象。锁不是平白出现的，而是寄托存储于对象中。
    _owner        = NULL;//指向获得ObjectMonitor对象的线程或基础锁
    _WaitSet      = NULL;//处于wait状态的线程，会被加入到wait set；
    _WaitSetLock  = 0 ;
    _Responsible  = NULL ;
    _succ         = NULL ;
    _cxq          = NULL ;
    FreeNext      = NULL ;
    _EntryList    = NULL ;//处于等待锁block状态的线程，会被加入到entry set；
    _SpinFreq     = 0 ;
    _SpinClock    = 0 ;
    OwnerIsThread = 0 ;// _owner is (Thread *) vs SP/BasicLock
    _previous_owner_tid = 0;// 监视器前一个拥有者线程的ID
  }


 多个线程在竞争共享数据执行到同步代码块时，会在_EntryList中排队，获得对象monitor的线程在进入_Owner区域时会将monitor的_owner设为当前线程，
 同时计数器_count加1。若持有monitor对象的线程调用了wait()方法会释放monitor，_owner为null，计数器_count减一，进入到_WaitSet集合中等待被唤醒。

 //synchronize　基本原理
 //jdk 1.6　之前　objectMonitor 的　enter 和　exit　直接和互斥量 mutex 映射　一旦发现同步代码块开始 就使用pthread_mutex_lock(object_monitor) 　退出执行unlock
//非常好的文章　https://www.cnblogs.com/dennyzhangdd/p/6734638.html

 //线程处于可偏向状态　且　是无所状态
 if(object->mark().bias == BIAS && object->mark().lockstatus == NO_LOCK){

        if(object->mark().thread == null){
                if(CAS(object->mark().thread_id = self_thread_id)){
                    return ;//执行同步代码块
                }

                //CAS失败　　说明有其它线程在竞争　　这时进行锁升级　偏向锁的升级必须等到全局安全点
                if(SAFE_POINT){
                //暂停偏向锁的线程（因为此时要锁升级了　先暂偏向锁中的线程）
                if(object.mark().biasThread().status != LOCK_STATUS ){
                          park(object.mark().biasThread());
                          //升级成轻量级锁
                          object.mark().lockstatus = LIGHT_LOCK;
                          //升级完成继续执行　偏向锁的线程
                          unpark(object.mark().biasThread());
                    }
                }
        }
        else if(object->mark().thread == self_thread){
               return ;
        }

        //说明这次进来的线程和之前的线程不一样　　　偏向锁升级
        if(SAFE_POINT){
                //暂停偏向锁的线程（因为此时要锁升级了　先暂偏向锁中的线程）
                if(object.mark().biasThread().status != LOCK_STATUS ){
                          park(object.mark().biasThread());
                          //升级成轻量级锁
                          object.mark().lockstatus = LIGHT_LOCK;
                          //升级完成继续执行　偏向锁的线程
                          unpark(object.mark().biasThread());
                    }
       }

 }

//不满足无锁　和　偏向锁标志位　　进入到轻量级锁中
slow_enter()　具体实现：





 else{

  monitor = object.createMonitorObject();

 if(mointor.owner == null){
     monitor.owner = self_thread;
     monitor.recursions = 1;
 }
 else{
    for(;;){
        if(monitor.owner == self_thread){
            monitor.recursions ++;
        }
        else{

            for(sometime){
                waiter = new ObjectWaiter(self_thread);
                node.entryList.addWaiter(waiter);
                addWaiterInto_cxq;
                self_thread.park;
            }
            //
        }
    }
    monitor.entryList.add(current)
 }
 }



 */

};
void *doit(void *);

/**
 * 线程相关学习
 */

int pthreadTest() {

    pthread_t tidA, tidB;

    pthread_create(&tidA, NULL, doit, NULL);
    pthread_create(&tidB, NULL, doit, NULL);

    pthread_join(tidA, NULL);
    pthread_join(tidB, NULL);

    return 0;

}

void *doit(void *arg) {
    int i, val;
    for (i = 0; i < 5000; i++) {
        /**
         * 互斥锁之间的代码　只有一个线程可以执行
         * 在java中  synchronize块起到了互斥锁的作用，同一时刻只有一个代码访问共享代码块
         * 而java　中的互斥量　可以是任意一个对象，为什么可以是任意一个对象
         * 需要将当前线程挂起，从用户态切换到内核态，这种切换代价是机器昂贵的
         *
         * 　java 在这里引入了锁的优化
         */
        pthread_mutex_lock(&counter_mutex);
        val = counter;
        //根据输出结果可以看出　多线程并发　大多时候都是连续同一个线程访问同步块，但是同一个；其次单线程环境下不涉及并发问题，每次都加锁代价也是机器昂贵的
        printf("%x: %d\n", (unsigned int) pthread_self(), val + 1);
        counter = val + 1;
        //互斥锁　解锁
        pthread_mutex_unlock(&counter_mutex);
    }
    return NULL;
}

int main() {
    //pthreadTest();
    //jvm　解析字节码文件　测试
    //parseTestClass();
}
