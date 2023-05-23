#include "ustack.h"

struct buffer buffer_stack[MAX_BUFFERS];
int stack_ptr = -1; // empty stack

int get_ptr(){
    return stack_ptr;
}


struct header {
    uint len;
    uint page_flag;
    struct header* prev;
};

struct header *current = 0; // Pointer to the current header

static struct header *curr = 0;
static char* stack_pointer;

//static header *freep ;


void* ustack_malloc(uint len){
   
    char *p;
    struct header *new_header;

    
    if (len > MAX_BUFFER_SIZE){
        return (void*)-1; // Exceeded maximum allowed size
    }

    if(curr == 0){ //initialization
        p = sbrk(PGSIZE);
        if(p == (char*)-1)
            return (void*)-1;
        new_header = (struct header*)p;
    }
    else if (PGROUNDDOWN((uint64)(stack_pointer + len)) !=  (uint64)sbrk(0)) /*need to alloc new page*/
    {
        p = sbrk(PGSIZE);
        if(p == (char*)-1)
            return (void*)-1;
        new_header = (struct header*)p;
        new_header->page_flag = 1;
    }
    else{
        new_header = (struct header*)(stack_pointer );
    }

    new_header->prev = curr;
    new_header->len = len;
    curr = new_header;
    stack_pointer =(char*) new_header + len;
    return (void*)stack_pointer;
}



int ustack_free(void){
    if (curr){
        char *p;
        int del_size = curr->len;
        curr = curr->prev;
        stack_pointer = stack_pointer - del_size;
        if (curr->page_flag == 1)
        {
            p = sbrk(-PGSIZE);
            if(p == (char*)-1)
                return -1;
        }
        return del_size; 
    }
    return -1;
}

// void *ustack_malloc3(uint len) {
//     if(len > MAX_BUFFER_SIZE) {
//         return (void *)-1;
//     }

//     if(current) {
//         new_header->prev = current;
//     } else {
//         printf("fuck 13\n");
//         new_header->prev = 0;
//     }
//     printf("fuck2\n");
//     new_header->len = len;
//     new_header->dealloc_page = PGROUNDDOWN(((uint64)sbrk(0) + sizeof(struct header) + len + PGSIZE - 1));
//         printf("fuck 4\n");
//     if(sbrk(PGROUNDDOWN(sizeof(struct header) + len + PGSIZE - 1) - (uint64)sbrk(0)) == (void *)-1) 
//         printf("fuck3\n");
//     {
//         return (void *)-1;
//     }
//     current = new_header;
//     printf("fuck5\n");

//     return (void *)(new_header + 1);
// }

// int ustack_free3() {
//     // Check if there's a buffer to free, if not return -1
//     if(current == 0) {
//         return -1;
//     }

//     // Store the length of the buffer to return later
//     int len = current->len;

//     // Check if we need to deallocate a page
//     if((uint64)sbrk(0) > current->dealloc_page) {
//         // Decrease the program break by a page size, effectively deallocating the page
//         sbrk(-PGSIZE);
//     }

//     // Decrease the program break to the start of the previous buffer, effectively deallocating the buffer
//     sbrk((uint64)current->prev - (uint64)sbrk(0));

//     // Set the current buffer to the previous buffer
//     current = current->prev;

//     // Return the length of the deallocated buffer
//     return len;
// }

// void* ustack_malloc2(uint len) {
//     if (len > MAX_BUFFER_SIZE) 
//       return (void*)-1;
    
//     void *current_break = sbrk(0);
//     void *new_break = (void*)((uint64)current_break + len);
    
//     // Check if new allocation would cross page boundary
//     if (PGROUNDDOWN((uint64)new_break) > PGROUNDDOWN((uint64)current_break)) {
//         void *new_page = sbrk(PAGE_SIZE);
//         if (new_page == (void*)-1 || new_page != current_break)
//           return (void*)-1;
//         current_break = new_page;
//     }
    
//     // Update buffer stack
//     if (stack_ptr >= MAX_BUFFERS - 1)
//       return (void*)-1;
//     stack_ptr++;
//     buffer_stack[stack_ptr].start = current_break;
//     buffer_stack[stack_ptr].len = len;
    
//     // Update program break
//     void *updated_break = sbrk(len);
//     if (updated_break == (void*)-1 || updated_break != current_break)
//       return (void*)-1;

//     return current_break;
// }

// int ustack_free2() {
//     if (stack_ptr < 0) 
//       return -1; // stack is empty

//     // Pop the buffer
//     struct buffer last_buffer = buffer_stack[stack_ptr];
//     stack_ptr--;

//     printf("l buff  : %p\n",last_buffer);
//     printf("l buff len  : %d\n",last_buffer.len);
//     // If freeing the buffer crosses a page boundary, release the page
//     void *current_break = sbrk(0);
//     printf("fuck 1\n");
//     void *new_break = (void*)((uint64)last_buffer.start - 1);
//     if (PGROUNDDOWN((uint64)new_break) < PGROUNDDOWN((uint64)current_break)) {
//             printf("fuck 2\n");
//         void *updated_break = sbrk(-PAGE_SIZE);
//                     printf("fuck 3\n");
//         if (updated_break == (void*)-1)
//           return -1;
//     }
    
//     // Update program break
//                 printf("fuck 4\n");
//     void *updated_break = sbrk(-last_buffer.len);
//                 printf("fuck 5\n");
//     if (updated_break == (void*)-1 )
//         return -1;

//     return last_buffer.len;
// }
