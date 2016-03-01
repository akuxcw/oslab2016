#include "stdarg.h"

int vprintk(const char *, va_list);
int cputchar(char);

static void
putch(int ch, int *cnt)
{
	cputchar(ch);
//	*cnt++;
}


int printk(const char *fmt, ...){
	va_list args;
	int r;

	va_start(args, fmt);
	r = vprintk(fmt, args);
	va_end(args);
	
	return r;
}

int vprintk(const char * fmt, va_list args){
//	char str[] = "Hello\n";
	/*
	__asm__("cli\n\t"
			"mov $0x13, \%ah\n\t"     // # 设置模式：显式字符串
			"mov $0x01, \%al\n\t"     // # 设置显式模式：(1) 使用 BL 指定的样式 (2) 更新光标
			"mov $0x00, \%bh\n\t"     // # 设置代码页，与字体相关； QEMU 下仅第一个代码页可用
			"mov $0x4F, \%bl\n\t"     //# 设置字符样式：红底白字无高亮
			"mov $6, \%cx\n\t"// # 优雅地计算字符串的长度
			"mov \%0, \%dx\n\t"      // # 将 ES:BP 指向字符串 (Step 1)
			"mov \%dx, \%bp\n\t"      //  # 将 ES:BP 指向字符串 (Step 2)
			"mov $0x00, \%dh\n\t"   //   # 首字符输出在第 1 行
			"mov $0x00, \%dl\n\t" //     # 首字符输出在第 1 列
			"int $0x10\n\t"     //      # BIOS 中断，VGA 相关服务
			:"=r"(fmt)
			//			"loop: \n\t"        //         # loop 表示下面这条指令的首地址
//			"jmp loop \n\t"   //        # 无限循环，防止继续执行超出内存
								
//			"str:\n\t"      //            # str 表示字符串首地址
//			".ascii \"Hello, World!\" \n\t"
//			"end:\n\t"    //            # end 表示字符串之后的地址，用于计算长度);
	);
	*/
	int x = 1;
	putch(fmt[1], &x);
	return 0;
}
