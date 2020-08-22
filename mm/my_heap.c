/*
 * =====================================================================================
 *
 *       Filename:  heap.c
 *
 *    Description:  内核的简单堆管理
 *
 *        Version:  1.0
 *        Created:  2020年08月22日 
 *       Revision:  none
 *       Compiler:  gcc
 *

 * =====================================================================================
 */
#include "debug.h"
#include "pmm.h"
#include "vmm.h"
#include "heap.h"

// 申请内存块
static void alloc_chunk(uint32_t start, uint32_t len_byte);

// 释放内存块
static void free_chunk(header_t *chunk);

// 切分内存块
static void split_chunk(header_t *chunk, uint32_t len_byte);

// 合并内存块
static void glue_chunk(header_t *chunk);

static uint32_t heap_max = HEAP_START;

// 内存块管理头指针
static header_t *heap_first;

void init_heap()
{
	heap_first = 0;
}

void *kmalloc(uint32_t len_byte)
{
	len_byte = len_byte + sizeof(header_t);
	header_t *ptr_now = heap_first;
	header_t *ptr_pre = 0;
	while(ptr_now)
	{
		
		if(ptr_now->length >= len_byte && ptr_now -> allocated == 0)
		{
			//printf("%d %d\n", ptr_now->length,len_byte);
			split_chunk(ptr_now,len_byte);
			ptr_now -> allocated = 1;
			return  (void *)((uint32_t) ptr_now + sizeof(header_t));
		}
		ptr_pre = ptr_now ;
		ptr_now = ptr_now -> next;
	}
	//第一次分配就直接走这里
	uint32_t new_ptr;
	if (ptr_pre)
	{
		new_ptr = (uint32_t) ptr_pre + ptr_pre->length ;
	}else
	{
		new_ptr = HEAP_START;
		heap_first = (header_t *) new_ptr;
	}

	// printf("inkmalloc %d \n", new_ptr);
	//判断是否有足够空间，如不够则分配
	alloc_chunk(new_ptr,len_byte);
	
	ptr_now = (header_t *)new_ptr;
	ptr_now->allocated = 1;

	ptr_now->length = len_byte;

	ptr_now -> prev = ptr_pre;
	ptr_now -> next = 0;

	if (ptr_pre)
	{
		ptr_pre ->next = ptr_now;
	}
	

	return (void *)(new_ptr + sizeof(header_t));
}

void kfree(void *p)
{
	// 指针回退到管理结构，并将已使用标记置 0
	header_t *header = (header_t*)((uint32_t)p - sizeof(header_t));
	header->allocated = 0;
	//printf("header %d\n",header->length);
	// printf("header size %d\n",header->next);
	// 粘合内存块
	glue_chunk(header);
	// printf("after glue header size %d\n",header->next);
}

void alloc_chunk(uint32_t start, uint32_t len_byte)
{

	while (start + len_byte > heap_max) {
		uint32_t page = pmm_alloc_page();
		map(pgd_kern, heap_max, page, PAGE_PRESENT | PAGE_WRITE);
		heap_max += PAGE_SIZE;
	}

	return ;
}

void free_chunk(header_t *chunk)
{
	if (chunk->prev == 0) {
		heap_first = 0;
	} else {
		chunk->prev->next = 0;
	}

	// 空闲的内存超过 1 页的话就释放掉
	while ((heap_max - PAGE_SIZE) >= (uint32_t)chunk) {
		heap_max -= PAGE_SIZE;
		uint32_t page;
		get_mapping(pgd_kern, heap_max, &page);
		unmap(pgd_kern, heap_max);
		pmm_free_page(page);
	}
	return;
}
void split_chunk(header_t *chunk, uint32_t len_byte)
{
	//printf("address = %d %d\n",chunk, chunk->length - len_byte);
	//如果还能剩多于一个头的大小就对内存进行划分
	if ( (chunk->length - len_byte) > sizeof(header_t))
	{
		
		header_t *next_chunck =  (header_t *) ((uint32_t )chunk + len_byte);
		
		next_chunck -> length = chunk->length - len_byte;
		next_chunck ->allocated = 0;
		chunk->length = len_byte;

		next_chunck->prev = chunk;
		next_chunck->next = chunk -> next;
		chunk->next = next_chunck;

		//不一样
		if(next_chunck->next) {
			next_chunck->next->prev = next_chunck;
		}
	}
	return ;
}
void glue_chunk(header_t *chunk)
{
	//放置临时指针方便指针操作
	header_t *tmp;
	//如果后面有未分配的内存那么就合并
	if (chunk->next && chunk->next->allocated==0)
	{
		
		chunk->length += chunk->next->length;

		tmp = chunk->next;
		chunk->next = tmp->next; 
		if (tmp->next)
		{
			tmp->next->prev = chunk;
		}
		

	}
	//如果前面有未分配的内存那么就合并
	if (chunk->prev && chunk->prev->allocated == 0 )
	{
		
		chunk->prev->length += chunk->length ;

		tmp = chunk->prev;
		tmp->next = chunk->next;
		if (chunk->next)
		{
			chunk->next->prev = tmp;
		}
		
		chunk = chunk->prev;
	}
	if (chunk->next == 0)
	{
		free_chunk(chunk);
	}
	return;
}

void test_heap()
{
	printk_color(rc_black, rc_magenta, "Test kmalloc() && kfree() now ...\n\n");

	void *addr1 = kmalloc(50);
	printk("kmalloc    50 byte in 0x%X\n", addr1);
	void *addr2 = kmalloc(500);
	printk("kmalloc   500 byte in 0x%X\n", addr2);
	void *addr3 = kmalloc(5000);
	printk("kmalloc  5000 byte in 0x%X\n", addr3);
	void *addr4 = kmalloc(50000);
	printk("kmalloc 50000 byte in 0x%X\n\n", addr4);

	printk("free mem in 0x%X\n", addr1);
	kfree(addr1);
	printk("free mem in 0x%X\n", addr2);
	kfree(addr2);
	printk("free mem in 0x%X\n", addr3);
	kfree(addr3);
	printk("free mem in 0x%X\n\n", addr4);
	kfree(addr4);
}
